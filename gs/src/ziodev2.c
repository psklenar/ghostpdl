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
/* (Level 2) IODevice operators */
#include "string_.h"
#include "ghost.h"
#include "gp.h"
#include "oper.h"
#include "stream.h"
#include "gxiodev.h"
#include "dstack.h"		/* for systemdict */
#include "files.h"		/* for file_open_stream */
#include "iparam.h"
#include "iutil2.h"
#include "store.h"

/* ------ %null% ------ */

/* This represents the null output file. */
private iodev_proc_open_device(null_open);
const gx_io_device gs_iodev_null = {
    "%null%", "Special",
    {
	iodev_no_init, null_open, iodev_no_open_file,
	iodev_os_fopen, iodev_os_fclose,
	iodev_no_delete_file, iodev_no_rename_file, iodev_no_file_status,
	iodev_no_enumerate_files, NULL, NULL,
	iodev_no_get_params, iodev_no_put_params
    }
};

private int
null_open(gx_io_device * iodev, const char *access, stream ** ps,
	  gs_memory_t * mem)
{
    if (!streq1(access, 'w'))
	return_error(mem, e_invalidfileaccess);
    return file_open_stream(gp_null_file_name,
			    strlen(gp_null_file_name),
			    access, 256 /* arbitrary */ , ps,
			    iodev, iodev->procs.fopen, mem);
}

/* ------ Operators ------ */

/* <iodevice> .getdevparams <mark> <name> <value> ... */
private int
zgetdevparams(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gx_io_device *iodev;
    stack_param_list list;
    gs_param_list *const plist = (gs_param_list *) & list;
    int code;
    ref *pmark;

    check_read_type(imemory, *op, t_string);
    iodev = gs_findiodevice(op->value.bytes, r_size(op));
    if (iodev == 0)
	return_error(imemory, e_undefinedfilename);
    stack_param_list_write(&list, &o_stack, NULL, iimemory);
    if ((code = gs_getdevparams(iodev, plist)) < 0) {
	ref_stack_pop(&o_stack, list.count * 2);
	return code;
    }
    pmark = ref_stack_index(&o_stack, list.count * 2);
    make_mark(pmark);
    return 0;
}

/* <mark> <name> <value> ... <iodevice> .putdevparams */
private int
zputdevparams(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gx_io_device *iodev;
    stack_param_list list;
    gs_param_list *const plist = (gs_param_list *) & list;
    int code;
    password system_params_password;

    check_read_type(imemory, *op, t_string);
    iodev = gs_findiodevice(op->value.bytes, r_size(op));
    if (iodev == 0)
	return_error(imemory, e_undefinedfilename);
    code = stack_param_list_read(&list, &o_stack, 1, NULL, false, iimemory);
    if (code < 0)
	return code;
    code = dict_read_password(imemory, &system_params_password, systemdict,
			      "SystemParamsPassword");
    if (code < 0)
	return code;
    code = param_check_password(imemory, plist, &system_params_password);
    if (code != 0) {
	iparam_list_release(&list);
	return_error(imemory, code < 0 ? code : e_invalidaccess);
    }
    code = gs_putdevparams(iodev, plist);
    iparam_list_release(&list);
    if (code < 0)
	return code;
    ref_stack_pop(&o_stack, list.count * 2 + 2);
    return 0;
}

/* ------ Initialization procedure ------ */

const op_def ziodev2_l2_op_defs[] =
{
    op_def_begin_level2(),
    {"1.getdevparams", zgetdevparams},
    {"2.putdevparams", zputdevparams},
    op_def_end(0)
};
