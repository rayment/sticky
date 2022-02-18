/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * vec4.c
 * 4D vector source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/09/2021
 */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/math.h"
#include "sticky/math/vec4.h"

void
S_vec4_zero(Svec4 *vec)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec4_zero");
		return;
	}
	vec->x = 0.0f;
	vec->y = 0.0f;
	vec->z = 0.0f;
	vec->w = 0.0f;
}

void
S_vec4_fill(Svec4 *vec,
            Sfloat scalar)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec4_fill");
		return;
	}
	vec->x = scalar;
	vec->y = scalar;
	vec->z = scalar;
	vec->w = scalar;
}

void
S_vec4_set(Svec4 *dest,
           Sfloat x,
           Sfloat y,
           Sfloat z,
           Sfloat w)
{
	dest->x = x;
	dest->y = y;
	dest->z = z;
	dest->w = w;
}

void
S_vec4_add(Svec4 *dest,
           const Svec4 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec4_add");
		return;
	}
	dest->x += src->x;
	dest->y += src->y;
	dest->z += src->z;
	dest->w += src->w;
}

void
S_vec4_multiply(Svec4 *dest,
                const Svec4 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec4_multiply");
		return;
	}
	dest->x *= src->x;
	dest->y *= src->y;
	dest->z *= src->z;
	dest->w *= src->w;
}

void
S_vec4_scale(Svec4 *vec,
             Sfloat scalar)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec4_scale");
		return;
	}
	vec->x *= scalar;
	vec->y *= scalar;
	vec->z *= scalar;
	vec->w *= scalar;
}

Sfloat
S_vec4_dot(const Svec4 *a,
           const Svec4 *b)
{
	if (!a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec4_dot");
		return S_INFINITY;
	}
	return a->x*b->x + a->y*b->y + a->z*b->z + a->w*b->w;
}

void
S_vec4_normalize(Svec4 *vec)
{
	Sfloat sqrtdot;
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec4_normalize");
		return;
	}
	_S_CALL("S_vec4_dot", sqrtdot = S_vec4_dot(vec, vec));
	sqrtdot = S_sqrt(sqrtdot);
	vec->x /= sqrtdot;
	vec->y /= sqrtdot;
	vec->z /= sqrtdot;
	vec->w /= sqrtdot;
}

void
S_vec4_inverse(Svec4 *vec)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec4_inverse");
		return;
	}
	vec->x = 1.0f / vec->x;
	vec->y = 1.0f / vec->y;
	vec->z = 1.0f / vec->z;
	vec->w = 1.0f / vec->w;
}

void
S_vec4_lerp(Svec4 *dest,
            const Svec4 *src,
            Sfloat time)
{
	Sfloat tdiff;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec4_lerp");
		return;
	}
	time = S_clamp(time, 0.0f, 1.0f);
	tdiff = (1.0f - time);
	dest->x = dest->x*time + src->x*tdiff;
	dest->y = dest->y*time + src->y*tdiff;
	dest->z = dest->z*time + src->z*tdiff;
	dest->w = dest->w*time + src->w*tdiff;
}

void
S_vec4_copy(Svec4 *dest,
            const Svec4 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec4_copy");
		return;
	}
	dest->x = src->x;
	dest->y = src->y;
	dest->z = src->z;
	dest->w = src->w;
}

Sbool
S_vec4_equals(Sfloat epsilon,
              const Svec4 *a,
              const Svec4 *b)
{
	Sint8 i;
	if (epsilon < 0 || !a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec4_equals");
		return S_FALSE;
	}
	i = 0;
	i += S_abs(a->x - b->x) <= epsilon;
	i += S_abs(a->y - b->y) <= epsilon;
	i += S_abs(a->z - b->z) <= epsilon;
	i += S_abs(a->w - b->w) <= epsilon;
	return i == 4;
}

