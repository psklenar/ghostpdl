/*
 * Copyright (C) 1998 Aladdin Enterprises.
 * All rights reserved.
 *
 * This file is part of Aladdin Ghostscript.
 *
 * Aladdin Ghostscript is distributed with NO WARRANTY OF ANY KIND.  No author
 * or distributor accepts any responsibility for the consequences of using it,
 * or for whether it serves any particular purpose or works at all, unless he
 * or she says so in writing.  Refer to the Aladdin Ghostscript Free Public
 * License (the "License") for full details.
 *
 * Every copy of Aladdin Ghostscript must include a copy of the License,
 * normally in a plain ASCII text file named PUBLIC.  The License grants you
 * the right to copy, modify and redistribute Aladdin Ghostscript, but only
 * under certain conditions described in the License.  Among other things, the
 * License requires that the copyright notice and this notice be preserved on
 * all copies.
 */

/* pcmtx3.h - 3 x 3 matrix utilities for PCL device independent color spaces */

#ifndef pcmtx3_INCLUDED
#define pcmtx3_INCLUDED

#include "math.h"
#include "gx.h"
#include "gsmatrix.h"
#include "gscspace.h"
#include "gscolor2.h"
#include "gscie.h"


/*
 * To support device independent color spaces, PCL performs a number of
 * calculations with 3 x 3 matrices. The CIE color space code in the graphic
 * library provides a format for 3 x 3 matrices, but the format is a bit
 * unusual and no corresponding utilities are provided. Hence, a separate
 * (though logically identical) format is provided here.
 *
 * Essentially all of the literature on color spaces deals with column 
 * vectors, EXCEPT that specific to PostScript. The latter deals with row 
 * vectors to remain consistent with the use of matrices for geometric 
 * transformations. The graphics library code, being based on PostScript,
 * uses the latter arrangement of components. The CIE code in the library is
 * documented for column vectors, but specifies matrices in the less usual
 * column-first order. The current code takes the logically equivalent and 
 * no less intuitive approach of using row vectors, and specifying 
 * matrices in the more normal row-first order.
 *
 * There is actually some reason for these alternate approaches. The CIE code
 * is primarily concerned with transformation of color vectors, and performs
 * relatively few other matrix operations. The current code infrequently 
 * transforms color vectors, but does perform other matrix operations. 
 * Each code module uses the format that is more intuitive for its more 
 * common operation.
 */
typedef union {
    struct {
        floatp    v1, v2, v3;
    }       vc;
    floatp  va[3];
} pcl_vec3_t;

typedef union {
    struct {
        floatp    a11, a12, a13,
                  a21, a22, a23,
                  a31, a32, a33;
    }       c;
    floatp  a[9];
} pcl_mtx3_t;

/*
 * Vectors in PCL and the graphic library are physically the same structure,
 * but the former are thought of as row vectors and the latter as column
 * vectors, so two different C-language structures are used. The following
 * macros are used for conversion.
 */
#define pcl_vec3_to_gs_vector3(gsv3, pcv3)                                    \
    ((gsv3).u = (pcv3).vc.v1, (gsv3).v = (pcv3).vc.v2, (gsv3).w = (pcv3).vc.v3)

#define pcl_vec3_from_gs_vector3(pcv3, gsv3)                                  \
    ((pcv3).vc.v1 = (gsv3).u, (pcv3).vc.v2 = (gsv3).v, (pcv3).vc.v3 = (gsv3).w)

/*
 * Add and subtract 3 dimensional vectors. These are not currently needed,
 * but are included for completeness.
 *
 * Any of the three pointer operands may be the same.
 */
extern  void    pcl_vec3_add( 
    const pcl_vec3_t *  pivec1,
    const pcl_vec3_t *  pivec2,
    pcl_vec3_t *        poutvec
);

extern  void    pcl_vec3_sub(
    const pcl_vec3_t *  pivec1,
    const pcl_vec3_t *  pivec2,
    pcl_vec3_t *        poutvec
);

/*
 * Apply a matrix to a 3-dimensional row vector.
 *
 * The code will properly handle the case pinvec == poutvec.
 */
extern  void    pcl_vec3_xform(
    const pcl_vec3_t *  pinvec,
    pcl_vec3_t *        poutvec,
    const pcl_mtx3_t *  pmtx
);

/*
 * Invert a 3 x 3 matrix.
 *
 * This will properly handle the case of pinmtx == poutmtx.
 *
 * Returns 0 on success, e_Range if the matrix provided is singular.
 */
extern  int     pcl_mtx3_invert(
    const pcl_mtx3_t *  pinmtx,
    pcl_mtx3_t *        poutmtx
);

/*
 * Add, subtract, and multiply two 3 x 3 matrices. These are not currently
 * needed, but are included for completenese.
 *
 * In all cases, any of the three pointers provided may be identical.
 */
extern  void    pcl_mtx3_add(
    const pcl_mtx3_t *  pinmtx1,
    const pcl_mtx3_t *  pinmtx2,
    pcl_mtx3_t *        poutmtx
);

extern  void    pcl_mtx3_sub(
    const pcl_mtx3_t *  pinmtx1,
    const pcl_mtx3_t *  pinmtx2,
    pcl_mtx3_t *        poutmtx
);

extern  void    pcl_mtx3_mul(
    const pcl_mtx3_t *  pinmtx1,
    const pcl_mtx3_t *  pinmtx2,
    pcl_mtx3_t *        poutmtx
);

/*
 * Convert a pcl_mtx3_t structure to and from a gs_matrix3 struct. Identity
 * transformations are rare in PCL, so no attempt is made to identify them.
 */
extern  void    pcl_mtx3_convert_to_gs(
    const pcl_mtx3_t *  pinmtx,
    gs_matrix3 *        pgsmtx
);

extern  void   pcl_mtx3_convert_from_gs(
    pcl_mtx3_t *        poutmtx,
    const gs_matrix3 *  pgsmtx
);

#endif  	/* pcmtx3_INCLUDED */
