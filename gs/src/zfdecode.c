/* Portions Copyright (C) 2001 artofcode LLC.
   Portions Copyright (C) 1996, 2001 Artifex Software Inc.
   Portions Copyright (C) 1988, 2000 Aladdin Enterprises.
   This software is based in part on the work of the Independent JPEG Group.
   All Rights Reserved.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/ or
   contact Artifex Software, Inc., 101 Lucas Valley Road #110,
   San Rafael, CA  94903, (415)492-9861, for further information. */

/*$RCSfile$ $Revision$ */
/* Additional decoding filter creation */
#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gsparam.h"
#include "gsstruct.h"
#include "ialloc.h"
#include "idict.h"
#include "idparam.h"
#include "ilevel.h"		/* for LL3 test */
#include "iparam.h"
#include "store.h"
#include "stream.h"		/* for setting is_temp */
#include "strimpl.h"
#include "sfilter.h"
#include "sa85x.h"
#include "scfx.h"
#include "scf.h"
#include "slzwx.h"
#include "spdiffx.h"
#include "spngpx.h"
#include "ifilter.h"
#include "ifilter2.h"
#include "ifrpred.h"

/* ------ ASCII85 filters ------ */

/* We include both encoding and decoding filters here, */
/* because it would be a nuisance to separate them. */

/* <target> ASCII85Encode/filter <file> */
/* <target> <dict> ASCII85Encode/filter <file> */
private int
zA85E(i_ctx_t *i_ctx_p)
{
    return filter_write_simple(i_ctx_p, &s_A85E_template);
}

/* <source> ASCII85Decode/filter <file> */
/* <source> <dict> ASCII85Decode/filter <file> */
private int
zA85D(i_ctx_t *i_ctx_p)
{
    return filter_read_simple(i_ctx_p, &s_A85D_template);
}

/* ------ CCITTFaxDecode filter ------ */

/* Common setup for encoding and decoding filters. */
extern stream_state_proc_put_params(s_CF_put_params, stream_CF_state);
int
zcf_setup(os_ptr op, stream_CF_state *pcfs, gs_ref_memory_t *imem)
{
    dict_param_list list;
    int code = dict_param_list_read(&list, op, NULL, false, imem);

    if (code < 0)
	return code;
    s_CF_set_defaults_inline(pcfs);
    code = s_CF_put_params((gs_param_list *)&list, pcfs);
    iparam_list_release(&list);
    return code;
}

/* <source> <dict> CCITTFaxDecode/filter <file> */
/* <source> CCITTFaxDecode/filter <file> */
private int
zCFD(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    os_ptr dop;
    stream_CFD_state cfs;
    int code;

    if (r_has_type(op, t_dictionary)) {
	check_dict_read(imemory, *op);
	dop = op;
    } else
	dop = 0;
    code = zcf_setup(dop, (stream_CF_state *)&cfs, iimemory);
    if (code < 0)
	return code;
    return filter_read(i_ctx_p, 0, &s_CFD_template, (stream_state *)&cfs, 0);
}

/* ------ Common setup for possibly pixel-oriented decoding filters ------ */

int
filter_read_predictor(i_ctx_t *i_ctx_p, int npop,
		      const stream_template * template, stream_state * st)
{
    os_ptr op = osp;
    int predictor, code;
    stream_PDiff_state pds;
    stream_PNGP_state pps;

    if (r_has_type(op, t_dictionary)) {
	if ((code = dict_int_param(imemory, op, "Predictor", 0, 15, 1, &predictor)) < 0)
	    return code;
	switch (predictor) {
	    case 0:		/* identity */
		predictor = 1;
	    case 1:		/* identity */
		break;
	    case 2:		/* componentwise horizontal differencing */
		code = zpd_setup(op, &pds);
		break;
	    case 10:
	    case 11:
	    case 12:
	    case 13:
	    case 14:
	    case 15:
		/* PNG prediction */
		code = zpp_setup(op, &pps);
		break;
	    default:
		return_error(imemory, e_rangecheck);
	}
	if (code < 0)
	    return code;
    } else
	predictor = 1;
    if (predictor == 1)
	return filter_read(i_ctx_p, npop, template, st, 0);
    {
	/* We need to cascade filters. */
	ref rsource, rdict;
	int code;

	/* Save the operands, just in case. */
	ref_assign(&rsource, op - 1);
	ref_assign(&rdict, op);
	code = filter_read(i_ctx_p, 1, template, st, 0);
	if (code < 0)
	    return code;
	/* filter_read changed osp.... */
	op = osp;
	code =
	    (predictor == 2 ?
	 filter_read(i_ctx_p, 0, &s_PDiffD_template, (stream_state *) & pds, 0) :
	  filter_read(i_ctx_p, 0, &s_PNGPD_template, (stream_state *) & pps, 0));
	if (code < 0) {
	    /* Restore the operands.  Don't bother trying to clean up */
	    /* the first stream. */
	    osp = ++op;
	    ref_assign(op - 1, &rsource);
	    ref_assign(op, &rdict);
	    return code;
	}
	/*
	 * Mark the compression stream as temporary, and propagate
	 * CloseSource from it to the predictor stream.
	 */
	filter_mark_strm_temp(op, 2);
	return code;
    }
}

/* ------ Generalized LZW/GIF decoding filter ------ */

/* Common setup for encoding and decoding filters. */
int
zlz_setup(os_ptr op, stream_LZW_state * plzs)
{
    int code;
    const ref *dop;

    if (r_has_type(op, t_dictionary)) {
	check_dict_read(plzs->memory, *op);
	dop = op;
    } else
	dop = 0;
    if (   (code = dict_int_param(plzs->memory, dop, "EarlyChange", 0, 1, 1,
				  &plzs->EarlyChange)) < 0 ||
	   /*
	    * The following are not PostScript standard, although
	    * LanguageLevel 3 provides the first two under different
	    * names.
	    */
	   (code = dict_int_param(plzs->memory, dop, "InitialCodeLength", 2, 11, 8,
				  &plzs->InitialCodeLength)) < 0 ||
	   (code = dict_bool_param(plzs->memory, dop, "FirstBitLowOrder", false,
				   &plzs->FirstBitLowOrder)) < 0 ||
	   (code = dict_bool_param(plzs->memory, dop, "BlockData", false,
				   &plzs->BlockData)) < 0
	)
	return code;
    return 0;
}

/* <source> LZWDecode/filter <file> */
/* <source> <dict> LZWDecode/filter <file> */
private int
zLZWD(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    stream_LZW_state lzs;
    int code = zlz_setup(op, &lzs);

    if (code < 0)
	return code;
    if (LL3_ENABLED && r_has_type(op, t_dictionary)) {
	int unit_size;

	if ((code = dict_bool_param(imemory, op, "LowBitFirst", lzs.FirstBitLowOrder,
				    &lzs.FirstBitLowOrder)) < 0 ||
	    (code = dict_int_param(imemory, op, "UnitSize", 3, 8, 8,
				   &unit_size)) < 0
	    )
	    return code;
	if (code == 0 /* UnitSize specified */ )
	    lzs.InitialCodeLength = unit_size + 1;
    }
    return filter_read_predictor(i_ctx_p, 0, &s_LZWD_template,
				 (stream_state *) & lzs);
}

/* ------ Color differencing filters ------ */

/* We include both encoding and decoding filters here, */
/* because it would be a nuisance to separate them. */

/* Common setup for encoding and decoding filters. */
int
zpd_setup(os_ptr op, stream_PDiff_state * ppds)
{
    int code, bpc;

    check_type(ppds->memory, *op, t_dictionary);
    check_dict_read(ppds->memory, *op);
    if ((code = dict_int_param(ppds->memory, op, "Colors", 1, s_PDiff_max_Colors, 1,
			       &ppds->Colors)) < 0 ||
	(code = dict_int_param(ppds->memory, op, "BitsPerComponent", 1, 8, 8,
			       &bpc)) < 0 ||
	(bpc & (bpc - 1)) != 0 ||
	(code = dict_int_param(ppds->memory, op, "Columns", 1, max_int, 1,
			       &ppds->Columns)) < 0
	)
	return (code < 0 ? code : gs_note_error(ppds->memory, e_rangecheck));
    ppds->BitsPerComponent = bpc;
    return 0;
}

/* <target> <dict> PixelDifferenceEncode/filter <file> */
private int
zPDiffE(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    stream_PDiff_state pds;
    int code = zpd_setup(op, &pds);

    if (code < 0)
	return code;
    return filter_write(i_ctx_p, 0, &s_PDiffE_template, (stream_state *) & pds, 0);
}

/* <source> <dict> PixelDifferenceDecode/filter <file> */
private int
zPDiffD(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    stream_PDiff_state pds;
    int code = zpd_setup(op, &pds);

    if (code < 0)
	return code;
    return filter_read(i_ctx_p, 0, &s_PDiffD_template, (stream_state *) & pds, 0);
}

/* ------ PNG pixel predictor filters ------ */

/* Common setup for encoding and decoding filters. */
int
zpp_setup(os_ptr op, stream_PNGP_state * ppps)
{
    int code, bpc;

    check_type(ppps->memory, *op, t_dictionary);
    check_dict_read(ppps->memory, *op);
    if ((code = dict_int_param(ppps->memory, op, "Colors", 1, 16, 1,
			       &ppps->Colors)) < 0 ||
	(code = dict_int_param(ppps->memory, op, "BitsPerComponent", 1, 16, 8,
			       &bpc)) < 0 ||
	(bpc & (bpc - 1)) != 0 ||
	(code = dict_uint_param(ppps->memory, op, "Columns", 1, max_uint, 1,
				&ppps->Columns)) < 0 ||
	(code = dict_int_param(ppps->memory, op, "Predictor", 10, 15, 15,
			       &ppps->Predictor)) < 0
	)
	return (code < 0 ? code : gs_note_error(ppps->memory, e_rangecheck));
    ppps->BitsPerComponent = bpc;
    return 0;
}

/* <target> <dict> PNGPredictorEncode/filter <file> */
private int
zPNGPE(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    stream_PNGP_state pps;
    int code = zpp_setup(op, &pps);

    if (code < 0)
	return code;
    return filter_write(i_ctx_p, 0, &s_PNGPE_template, (stream_state *) & pps, 0);
}

/* <source> <dict> PNGPredictorDecode/filter <file> */
private int
zPNGPD(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    stream_PNGP_state pps;
    int code = zpp_setup(op, &pps);

    if (code < 0)
	return code;
    return filter_read(i_ctx_p, 0, &s_PNGPD_template, (stream_state *) & pps, 0);
}

/* ---------------- Initialization procedure ---------------- */

const op_def zfdecode_op_defs[] = {
    op_def_begin_filter(),
    {"1ASCII85Encode", zA85E},
    {"1ASCII85Decode", zA85D},
    {"2CCITTFaxDecode", zCFD},
    {"1LZWDecode", zLZWD},
    {"2PixelDifferenceDecode", zPDiffD},
    {"2PixelDifferenceEncode", zPDiffE},
    {"2PNGPredictorDecode", zPNGPD},
    {"2PNGPredictorEncode", zPNGPE},
    op_def_end(0)
};
