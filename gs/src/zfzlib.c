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
/* zlib and Flate filter creation */
#include "ghost.h"
#include "oper.h"
#include "idict.h"
#include "strimpl.h"
#include "spdiffx.h"
#include "spngpx.h"
#include "szlibx.h"
#include "idparam.h"
#include "ifilter.h"
#include "ifrpred.h"
#include "ifwpred.h"
#include "ialloc.h"

/* Common setup for zlib (Flate) filter */
private int
filter_zlib(i_ctx_t *i_ctx_p, stream_zlib_state *pzls)
{
    os_ptr op = osp;
    int code = 0;

    (*s_zlibE_template.set_defaults)((stream_state *)pzls);
    if (r_has_type(op, t_dictionary))
	code = dict_int_param(imemory, op, "Effort", -1, 9, -1, &pzls->level);
    return code;
}

/* <source> zlibEncode/filter <file> */
/* <source> <dict> zlibEncode/filter <file> */
private int
zzlibE(i_ctx_t *i_ctx_p)
{
    stream_zlib_state zls;
    int code = filter_zlib(i_ctx_p, &zls);

    if (code < 0)
	return code;
    return filter_write(i_ctx_p, 0, &s_zlibE_template, (stream_state *)&zls, 0);
}

/* <target> zlibDecode/filter <file> */
/* <target> <dict> zlibDecode/filter <file> */
private int
zzlibD(i_ctx_t *i_ctx_p)
{
    stream_zlib_state zls;

    (*s_zlibD_template.set_defaults)((stream_state *)&zls);
    return filter_read(i_ctx_p, 0, &s_zlibD_template, (stream_state *)&zls, 0);
}

/* <source> FlateEncode/filter <file> */
/* <source> <dict> FlateEncode/filter <file> */
private int
zFlateE(i_ctx_t *i_ctx_p)
{
    stream_zlib_state zls;
    int code = filter_zlib(i_ctx_p, &zls);

    if (code < 0)
	return code;
    return filter_write_predictor(i_ctx_p, 0, &s_zlibE_template,
				  (stream_state *)&zls);
}

/* <target> FlateDecode/filter <file> */
/* <target> <dict> FlateDecode/filter <file> */
private int
zFlateD(i_ctx_t *i_ctx_p)
{
    stream_zlib_state zls;

    (*s_zlibD_template.set_defaults)((stream_state *)&zls);
    return filter_read_predictor(i_ctx_p, 0, &s_zlibD_template,
				 (stream_state *)&zls);
}

/* ------ Initialization procedure ------ */

const op_def zfzlib_op_defs[] =
{
    op_def_begin_filter(),
    {"1zlibEncode", zzlibE},
    {"1zlibDecode", zzlibD},
    {"1FlateEncode", zFlateE},
    {"1FlateDecode", zFlateD},
    op_def_end(0)
};
