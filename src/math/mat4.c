/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mat4.c
 * 4x4 matrix source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#include <string.h>

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/math.h"
#include "sticky/math/mat4.h"
#include "sticky/memory/memtrace.h"

void
S_mat4_identity(Smat4 *mat)
{
	if (!mat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat4_identity");
		return;
	}
	mat->m00 = 1.0f; mat->m01 = 0.0f; mat->m02 = 0.0f; mat->m03 = 0.0f;
	mat->m10 = 0.0f; mat->m11 = 1.0f; mat->m12 = 0.0f; mat->m13 = 0.0f;
	mat->m20 = 0.0f; mat->m21 = 0.0f; mat->m22 = 1.0f; mat->m23 = 0.0f;
	mat->m30 = 0.0f; mat->m31 = 0.0f; mat->m32 = 0.0f; mat->m33 = 1.0f;
}

void
S_mat4_multiply(Smat4 *dest,
                const Smat4 *src)
{
	Smat4 mat;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat4_multiply");
		return;
	}
	mat.m00 = dest->m00*src->m00 + dest->m01*src->m10 +
	          dest->m02*src->m20 + dest->m03*src->m30;
	mat.m01 = dest->m00*src->m01 + dest->m01*src->m11 +
	          dest->m02*src->m21 + dest->m03*src->m31;
	mat.m02 = dest->m00*src->m02 + dest->m01*src->m12 +
	          dest->m02*src->m22 + dest->m03*src->m32;
	mat.m03 = dest->m00*src->m03 + dest->m01*src->m13 +
	          dest->m02*src->m23 + dest->m03*src->m33;
	mat.m10 = dest->m10*src->m00 + dest->m11*src->m10 +
	          dest->m12*src->m20 + dest->m13*src->m30;
	mat.m11 = dest->m10*src->m01 + dest->m11*src->m11 +
	          dest->m12*src->m21 + dest->m13*src->m31;
	mat.m12 = dest->m10*src->m02 + dest->m11*src->m12 +
	          dest->m12*src->m22 + dest->m13*src->m32;
	mat.m13 = dest->m10*src->m03 + dest->m11*src->m13 +
	          dest->m12*src->m23 + dest->m13*src->m33;
	mat.m20 = dest->m20*src->m00 + dest->m21*src->m10 +
	          dest->m22*src->m20 + dest->m23*src->m30;
	mat.m21 = dest->m20*src->m01 + dest->m21*src->m11 +
	          dest->m22*src->m21 + dest->m23*src->m31;
	mat.m22 = dest->m20*src->m02 + dest->m21*src->m12 +
	          dest->m22*src->m22 + dest->m23*src->m32;
	mat.m23 = dest->m20*src->m03 + dest->m21*src->m13 +
	          dest->m22*src->m23 + dest->m23*src->m33;
	mat.m30 = dest->m30*src->m00 + dest->m31*src->m10 +
	          dest->m32*src->m20 + dest->m33*src->m30;
	mat.m31 = dest->m30*src->m01 + dest->m31*src->m11 +
	          dest->m32*src->m21 + dest->m33*src->m31;
	mat.m32 = dest->m30*src->m02 + dest->m31*src->m12 +
	          dest->m32*src->m22 + dest->m33*src->m32;
	mat.m33 = dest->m30*src->m03 + dest->m31*src->m13 +
	          dest->m32*src->m23 + dest->m33*src->m33;
	_S_CALL("S_mat4_copy", S_mat4_copy(dest, &mat));
}

void
S_mat4_transpose(Smat4 *mat)
{
	if (!mat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat4_transpose");
		return;
	}
	Smat4 tmp = {
		.m00 = mat->m00, .m01 = mat->m10, .m02 = mat->m20, .m03 = mat->m30,
		.m10 = mat->m01, .m11 = mat->m11, .m12 = mat->m21, .m13 = mat->m31,
		.m20 = mat->m02, .m21 = mat->m12, .m22 = mat->m22, .m23 = mat->m32,
		.m30 = mat->m03, .m31 = mat->m13, .m32 = mat->m23, .m33 = mat->m33,
	};
	_S_CALL("S_mat4_copy", S_mat4_copy(mat, &tmp));
}

/*
 * See: https://stackoverflow.com/a/44446912
 */
Sbool
S_mat4_inverse(Smat4 *mat)
{
	Smat4 tmp;
	float a2323, a1323, a1223, a0323, a0223, a0123, a2313, a1313, a1213,
	      a2312, a1312, a1212, a0313, a0213, a0312, a0212, a0113, a0112,
	      det;

	if (!mat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat4_inverse");
		return S_FALSE;
	}
	a2323 = mat->m22*mat->m33 - mat->m23*mat->m32;
	a1323 = mat->m21*mat->m33 - mat->m23*mat->m31;
	a1223 = mat->m21*mat->m32 - mat->m22*mat->m31;
	a0323 = mat->m20*mat->m33 - mat->m23*mat->m30;
	a0223 = mat->m20*mat->m32 - mat->m22*mat->m30;
	a0123 = mat->m20*mat->m31 - mat->m21*mat->m30;
	a2313 = mat->m12*mat->m33 - mat->m13*mat->m32;
	a1313 = mat->m11*mat->m33 - mat->m13*mat->m31;
	a1213 = mat->m11*mat->m32 - mat->m12*mat->m31;
	a2312 = mat->m12*mat->m23 - mat->m13*mat->m22;
	a1312 = mat->m11*mat->m23 - mat->m13*mat->m21;
	a1212 = mat->m11*mat->m22 - mat->m12*mat->m21;
	a0313 = mat->m10*mat->m33 - mat->m13*mat->m30;
	a0213 = mat->m10*mat->m32 - mat->m12*mat->m30;
	a0312 = mat->m10*mat->m23 - mat->m13*mat->m20;
	a0212 = mat->m10*mat->m22 - mat->m12*mat->m20;
	a0113 = mat->m10*mat->m31 - mat->m11*mat->m30;
	a0112 = mat->m10*mat->m21 - mat->m11*mat->m20;

	det = mat->m00 * (mat->m11*a2323 - mat->m12*a1323 + mat->m13*a1223)
	    - mat->m01 * (mat->m10*a2323 - mat->m12*a0323 + mat->m13*a0223)
	    + mat->m02 * (mat->m10*a1323 - mat->m11*a0323 + mat->m13*a0123)
	    - mat->m03 * (mat->m10*a1223 - mat->m11*a0223 + mat->m12*a0123);
	if (det == 0.0f)
		return S_FALSE; /* degenerate matrix */
	det = 1.0f / det;

	tmp.m00 = det *   (mat->m11*a2323 - mat->m12*a1323 + mat->m13*a1223);
	tmp.m01 = det * - (mat->m01*a2323 - mat->m02*a1323 + mat->m03*a1223);
	tmp.m02 = det *   (mat->m01*a2313 - mat->m02*a1313 + mat->m03*a1213);
	tmp.m03 = det * - (mat->m01*a2312 - mat->m02*a1312 + mat->m03*a1212);
	tmp.m10 = det * - (mat->m10*a2323 - mat->m12*a0323 + mat->m13*a0223);
	tmp.m11 = det *   (mat->m00*a2323 - mat->m02*a0323 + mat->m03*a0223);
	tmp.m12 = det * - (mat->m00*a2313 - mat->m02*a0313 + mat->m03*a0213);
	tmp.m13 = det *   (mat->m00*a2312 - mat->m02*a0312 + mat->m03*a0212);
	tmp.m20 = det *   (mat->m10*a1323 - mat->m11*a0323 + mat->m13*a0123);
	tmp.m21 = det * - (mat->m00*a1323 - mat->m01*a0323 + mat->m03*a0123);
	tmp.m22 = det *   (mat->m00*a1313 - mat->m01*a0313 + mat->m03*a0113);
	tmp.m23 = det * - (mat->m00*a1312 - mat->m01*a0312 + mat->m03*a0112);
	tmp.m30 = det * - (mat->m10*a1223 - mat->m11*a0223 + mat->m12*a0123);
	tmp.m31 = det *   (mat->m00*a1223 - mat->m01*a0223 + mat->m02*a0123);
	tmp.m32 = det * - (mat->m00*a1213 - mat->m01*a0213 + mat->m02*a0113);
	tmp.m33 = det *   (mat->m00*a1212 - mat->m01*a0212 + mat->m02*a0112);

	_S_CALL("S_mat4_copy", S_mat4_copy(mat, &tmp));
	return S_TRUE;
}

void
S_mat4_translate(Smat4 *dest,
                 const Svec3 *vec)
{
	if (!dest || !vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat4_translate");
		return;
	}
	_S_CALL("S_mat4_identity", S_mat4_identity(dest));
	dest->m03 = vec->x;
	dest->m13 = vec->y;
	dest->m23 = vec->z;
}

void
S_mat4_scale(Smat4 *dest,
             const Svec3 *vec)
{
	if (!dest || !vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat4_scale");
		return;
	}
	_S_CALL("S_mat4_identity", S_mat4_identity(dest));
	dest->m00 = vec->x;
	dest->m11 = vec->y;
	dest->m22 = vec->z;
}

void
S_mat4_copy(Smat4 *dest,
            const Smat4 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat4_copy");
		return;
	}
	memcpy(dest, src, sizeof(Smat4));
}

Sbool
S_mat4_equals(Sfloat epsilon,
              const Smat4 *a,
              const Smat4 *b)
{
	Sint8 i;
	if (epsilon < 0 || !a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat4_equals");
		return S_FALSE;
	}
	i = 0;
	i += S_abs(a->m00 - b->m00) <= epsilon;
	i += S_abs(a->m01 - b->m01) <= epsilon;
	i += S_abs(a->m02 - b->m02) <= epsilon;
	i += S_abs(a->m03 - b->m03) <= epsilon;
	i += S_abs(a->m10 - b->m10) <= epsilon;
	i += S_abs(a->m11 - b->m11) <= epsilon;
	i += S_abs(a->m12 - b->m12) <= epsilon;
	i += S_abs(a->m13 - b->m13) <= epsilon;
	i += S_abs(a->m20 - b->m20) <= epsilon;
	i += S_abs(a->m21 - b->m21) <= epsilon;
	i += S_abs(a->m22 - b->m22) <= epsilon;
	i += S_abs(a->m23 - b->m23) <= epsilon;
	i += S_abs(a->m30 - b->m30) <= epsilon;
	i += S_abs(a->m31 - b->m31) <= epsilon;
	i += S_abs(a->m32 - b->m32) <= epsilon;
	i += S_abs(a->m33 - b->m33) <= epsilon;
	return i == 16;
}

void
S_mat4_to_mat3(Smat3 *dest,
               const Smat4 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mat4_to_mat3");
		return;
	}
	dest->m00 = src->m00;
	dest->m01 = src->m01;
	dest->m02 = src->m02;
	dest->m10 = src->m10;
	dest->m11 = src->m11;
	dest->m12 = src->m12;
	dest->m20 = src->m20;
	dest->m21 = src->m21;
	dest->m22 = src->m22;
}

