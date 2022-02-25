/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * vec3.c
 * 3D vector source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 10/09/2021
 */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/math.h"
#include "sticky/math/vec3.h"

void
S_vec3_zero(Svec3 *vec)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_zero");
		return;
	}
	vec->x = 0.0f;
	vec->y = 0.0f;
	vec->z = 0.0f;
}

void
S_vec3_fill(Svec3 *vec,
            Sfloat scalar)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_fill");
		return;
	}
	vec->x = scalar;
	vec->y = scalar;
	vec->z = scalar;
}

void
S_vec3_set(Svec3 *dest,
           Sfloat x,
           Sfloat y,
           Sfloat z)
{
	dest->x = x;
	dest->y = y;
	dest->z = z;
}

void
S_vec3_add(Svec3 *dest,
           const Svec3 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_add");
		return;
	}
	dest->x += src->x;
	dest->y += src->y;
	dest->z += src->z;
}

void
S_vec3_multiply(Svec3 *dest,
                const Svec3 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_multiply");
		return;
	}
	dest->x *= src->x;
	dest->y *= src->y;
	dest->z *= src->z;
}

void
S_vec3_scale(Svec3 *vec,
             Sfloat scalar)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_scale");
		return;
	}
	vec->x *= scalar;
	vec->y *= scalar;
	vec->z *= scalar;
}

Sfloat
S_vec3_dot(const Svec3 *a,
           const Svec3 *b)
{
	if (!a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_dot");
		return S_INFINITY;
	}
	return a->x*b->x + a->y*b->y + a->z*b->z;
}

Sfloat
S_vec3_distance(const Svec3 *a,
                const Svec3 *b)
{
	Sfloat x, y, z;
	if (!a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_distance");
		return 0.0f;
	}
	x = a->x - b->x;
	y = a->y - b->y;
	z = a->z - b->z;
	return S_sqrt((x*x) + (y*y) + (z*z));
}

void
S_vec3_cross(Svec3 *dest,
             const Svec3 *src)
{
	Svec3 tmp;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_cross");
		return;
	}
	_S_CALL("S_vec3_copy", S_vec3_copy(&tmp, dest));
	dest->x = src->y*tmp.z - src->z*tmp.y;
	dest->y = src->z*tmp.x - src->x*tmp.z;
	dest->z = src->x*tmp.y - src->y*tmp.x;
}

void
S_vec3_normalize(Svec3 *vec)
{
	Sfloat sqrtdot;
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_normalize");
		return;
	}
	_S_CALL("S_vec3_dot", sqrtdot = S_vec3_dot(vec, vec));
	sqrtdot = S_sqrt(sqrtdot);
	vec->x /= sqrtdot;
	vec->y /= sqrtdot;
	vec->z /= sqrtdot;
}

void
S_vec3_inverse(Svec3 *vec)
{
	if (!vec)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_inverse");
		return;
	}
	vec->x = 1.0f / vec->x;
	vec->y = 1.0f / vec->y;
	vec->z = 1.0f / vec->z;
}

void
S_vec3_lerp(Svec3 *dest,
            const Svec3 *src,
            Sfloat time)
{
	Sfloat tdiff;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_lerp");
		return;
	}
	time = S_clamp(time, 0.0f, 1.0f);
	tdiff = (1.0f - time);
	dest->x = dest->x*time + src->x*tdiff;
	dest->y = dest->y*time + src->y*tdiff;
	dest->z = dest->z*time + src->z*tdiff;
}

void
S_vec3_copy(Svec3 *dest,
            const Svec3 *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_copy");
		return;
	}
	dest->x = src->x;
	dest->y = src->y;
	dest->z = src->z;
}

Sbool
S_vec3_equals(Sfloat epsilon,
              const Svec3 *a,
              const Svec3 *b)
{
	Sint8 i;
	if (epsilon < 0 || !a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_equals");
		return S_FALSE;
	}
	i = 0;
	i += S_abs(a->x - b->x) <= epsilon;
	i += S_abs(a->y - b->y) <= epsilon;
	i += S_abs(a->z - b->z) <= epsilon;
	return i == 3;
}

/*
 * See:
 * https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
 */
void
S_vec3_to_quat(Squat *dest,
               const Svec3 *src)
{
	Sfloat cx, sx, cy, sy, cz, sz;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_vec3_to_quat");
		return;
	}
	cx = S_cos(S_radians(src->x) * 0.5f);
	sx = S_sin(S_radians(src->x) * 0.5f);
	cy = S_cos(S_radians(src->y) * 0.5f);
	sy = S_sin(S_radians(src->y) * 0.5f);
	cz = S_cos(S_radians(src->z) * 0.5f);
	sz = S_sin(S_radians(src->z) * 0.5f);
	dest->r = cz*cx*cy + sz*sx*sy;
	dest->i = cz*sx*cy + sz*cx*sy;
	dest->j = cz*cx*sy - sz*sx*cy;
	dest->k = sz*cx*cy - cz*sx*sy;
}

