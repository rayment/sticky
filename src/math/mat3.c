/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mat3.c
 * 3x3 matrix source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 09/09/2021
 */

#include <math.h>
#include <string.h>

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/mat3.h"
#include "sticky/memory/memtrace.h"

void
S_mat3_identity(Smat3 *mat)
{
	if (!mat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat3_identity");
		return;
	}
	mat->m00 = 1.0f; mat->m01 = 0.0f; mat->m02 = 0.0f;
	mat->m10 = 0.0f; mat->m11 = 1.0f; mat->m12 = 0.0f;
	mat->m20 = 0.0f; mat->m21 = 0.0f; mat->m22 = 1.0f;
}

void
S_mat3_multiply(Smat3 *dest,
                const Smat3 *src)
{
	Smat3 mat;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat3_multiply");
		return;
	}
	mat.m00 = dest->m00*src->m00 + dest->m01*src->m10 + dest->m02*src->m20;
	mat.m01 = dest->m00*src->m01 + dest->m01*src->m11 + dest->m02*src->m21;
	mat.m02 = dest->m00*src->m02 + dest->m01*src->m12 + dest->m02*src->m22;
	mat.m10 = dest->m10*src->m00 + dest->m11*src->m10 + dest->m12*src->m20;
	mat.m11 = dest->m10*src->m01 + dest->m11*src->m11 + dest->m12*src->m21;
	mat.m12 = dest->m10*src->m02 + dest->m11*src->m12 + dest->m12*src->m22;
	mat.m20 = dest->m20*src->m00 + dest->m21*src->m10 + dest->m22*src->m20;
	mat.m21 = dest->m20*src->m01 + dest->m21*src->m11 + dest->m22*src->m21;
	mat.m22 = dest->m20*src->m02 + dest->m21*src->m12 + dest->m22*src->m22;
	_S_CALL("S_mat3_copy", S_mat3_copy(dest, &mat));
}

void
S_mat3_transpose(Smat3 *mat)
{
	if (!mat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat3_transpose");
		return;
	}
	Smat3 tmp = {
		.m00 = mat->m00, .m01 = mat->m10, .m02 = mat->m20,
		.m10 = mat->m01, .m11 = mat->m11, .m12 = mat->m21,
		.m20 = mat->m02, .m21 = mat->m12, .m22 = mat->m22
	};
	_S_CALL("S_mat3_copy", S_mat3_copy(mat, &tmp));
}

/*
 * See: https://stackoverflow.com/a/44446912
 */
Sbool
S_mat3_inverse(Smat3 *mat)
{
	Smat3 tmp;
	float a1122, a2112, a1022, a1220, a1021, a1120, a0122, a0221, a0112, a0211,
	      a0022, a0220, a0012, a1002, a2011, a0021, a2001, a0011, a1001, det;

	if (!mat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat3_inverse");
		return S_FALSE;
	}
	a1122 = mat->m11 * mat->m22;
	a2112 = mat->m21 * mat->m12;
	a1022 = mat->m10 * mat->m22;
	a1220 = mat->m12 * mat->m20;
	a1021 = mat->m10 * mat->m21;
	a1120 = mat->m11 * mat->m20;
	a0122 = mat->m01 * mat->m22;
	a0221 = mat->m02 * mat->m21;
	a0112 = mat->m01 * mat->m12;
	a0211 = mat->m02 * mat->m11;
	a0022 = mat->m00 * mat->m22;
	a0220 = mat->m02 * mat->m20;
	a0012 = mat->m00 * mat->m12;
	a1002 = mat->m10 * mat->m02;
	a2011 = mat->m20 * mat->m11;
	a0021 = mat->m00 * mat->m21;
	a2001 = mat->m20 * mat->m01;
	a0011 = mat->m00 * mat->m11;
	a1001 = mat->m10 * mat->m01;

	det = mat->m00 * (a1122 - a2112)
	    - mat->m01 * (a1022 - a1220)
	    + mat->m02 * (a1021 - a1120);
	if (det == 0.0f)
		return S_FALSE; /* degenerate matrix */
	det = 1.0f / det;

	tmp.m00 = det *  (a1122 - a2112);
	tmp.m01 = det * -(a1022 - a1220);
	tmp.m02 = det *  (a1021 - a2011);
	tmp.m10 = det * -(a0122 - a0221);
	tmp.m11 = det *  (a0022 - a0220);
	tmp.m12 = det * -(a0021 - a2001);
	tmp.m20 = det *  (a0112 - a0211);
	tmp.m21 = det * -(a0012 - a1002);
	tmp.m22 = det *  (a0011 - a1001);

	_S_CALL("S_mat3_copy", S_mat3_copy(mat, &tmp));
	return S_TRUE;
}

void
S_mat3_copy(Smat3 *dest,
            const Smat3 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat3_copy");
		return;
	}
	memcpy(dest, src, sizeof(Smat3));
}

Sbool
S_mat3_equals(Sfloat epsilon,
              const Smat3 *a,
              const Smat3 *b)
{
	Sint8 i;
	if (epsilon < 0 || !a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat3_equals");
		return S_FALSE;
	}
	i = 0;
	i += fabs(a->m00 - b->m00) <= epsilon;
	i += fabs(a->m01 - b->m01) <= epsilon;
	i += fabs(a->m02 - b->m02) <= epsilon;
	i += fabs(a->m10 - b->m10) <= epsilon;
	i += fabs(a->m11 - b->m11) <= epsilon;
	i += fabs(a->m12 - b->m12) <= epsilon;
	i += fabs(a->m20 - b->m20) <= epsilon;
	i += fabs(a->m21 - b->m21) <= epsilon;
	i += fabs(a->m22 - b->m22) <= epsilon;
	return i == 9;
}

void
S_mat3_to_mat4(Smat4 *dest,
               const Smat3 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat3_to_mat4");
		return;
	}
	dest->m00 = src->m00;
	dest->m01 = src->m01;
	dest->m02 = src->m02;
	dest->m03 = 0.0f;
	dest->m10 = src->m10;
	dest->m11 = src->m11;
	dest->m12 = src->m12;
	dest->m03 = 0.0f;
	dest->m20 = src->m20;
	dest->m21 = src->m21;
	dest->m22 = src->m22;
	dest->m03 = 0.0f;
	dest->m30 = 0.0f;
	dest->m31 = 0.0f;
	dest->m32 = 0.0f;
	dest->m33 = 1.0f;
}

