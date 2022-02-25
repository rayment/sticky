/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * vec2.c
 * 2D vector source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 09/09/2021
 */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/math.h"
#include "sticky/math/vec2.h"

void
S_vec2_zero(Svec2 *vec)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_zero");
		return;
	}
	vec->x = 0.0f;
	vec->y = 0.0f;
}

void
S_vec2_fill(Svec2 *vec,
            Sfloat scalar)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_fill");
		return;
	}
	vec->x = scalar;
	vec->y = scalar;
}

void
S_vec2_set(Svec2 *dest,
           Sfloat x,
           Sfloat y)
{
	dest->x = x;
	dest->y = y;
}

void
S_vec2_add(Svec2 *dest,
           const Svec2 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_add");
		return;
	}
	dest->x += src->x;
	dest->y += src->y;
}

void
S_vec2_multiply(Svec2 *dest,
                const Svec2 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_multiply");
		return;
	}
	dest->x *= src->x;
	dest->y *= src->y;
}

void
S_vec2_scale(Svec2 *vec,
             Sfloat scalar)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_scale");
		return;
	}
	vec->x *= scalar;
	vec->y *= scalar;
}

Sfloat
S_vec2_dot(const Svec2 *a,
           const Svec2 *b)
{
	if (!a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_dot");
		return S_INFINITY;
	}
	return a->x*b->x + a->y*b->y;
}

Sfloat
S_vec2_distance(const Svec2 *a,
                const Svec2 *b)
{
	Sfloat x, y;
	if (!a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_distance");
		return 0.0f;
	}
	x = a->x - b->x;
	y = a->y - b->y;
	return S_sqrt((x*x) + (y*y));
}

void
S_vec2_normalize(Svec2 *vec)
{
	Sfloat sqrtdot;
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_normalize");
		return;
	}
	_S_CALL("S_vec2_dot", sqrtdot = S_vec2_dot(vec, vec));
	sqrtdot = S_sqrt(sqrtdot);
	vec->x /= sqrtdot;
	vec->y /= sqrtdot;
}

void
S_vec2_inverse(Svec2 *vec)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_inverse");
		return;
	}
	vec->x = 1.0f / vec->x;
	vec->y = 1.0f / vec->y;
}

void
S_vec2_lerp(Svec2 *dest,
            const Svec2 *src,
            Sfloat time)
{
	Sfloat tdiff;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_lerp");
		return;
	}
	time = S_clamp(time, 0.0f, 1.0f);
	tdiff = (1.0f - time);
	dest->x = dest->x*time + src->x*tdiff;
	dest->y = dest->y*time + src->y*tdiff;
}

void
S_vec2_copy(Svec2 *dest,
            const Svec2 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_copy");
		return;
	}
	dest->x = src->x;
	dest->y = src->y;
}

Sbool
S_vec2_equals(Sfloat epsilon,
              const Svec2 *a,
              const Svec2 *b)
{
	Sint8 i;
	if (epsilon < 0 || !a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec2_equals");
		return S_FALSE;
	}
	i = 0;
	i += S_abs(a->x - b->x) <= epsilon;
	i += S_abs(a->y - b->y) <= epsilon;
	return i == 2;
}

