/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * quat.c
 * Quaternion source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 09/09/2021
 */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/math.h"
#include "sticky/math/quat.h"
#include "sticky/math/vec3.h"
#include "sticky/memory/memtrace.h"

void
S_quat_identity(Squat *quat)
{
	if (!quat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_identity");
		return;
	}
	quat->r = 1.0f;
	quat->i = 0.0f;
	quat->j = 0.0f;
	quat->k = 0.0f;
}

void
S_quat_multiply(Squat *dest,
                const Squat *src)
{
	Squat tmp;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_multiply");
		return;
	}
	tmp.r = dest->r*src->r - dest->i*src->i - dest->j*src->j - dest->k*src->k;
	tmp.i = dest->r*src->i + dest->i*src->r + dest->j*src->k - dest->k*src->j;
	tmp.j = dest->r*src->j - dest->i*src->k + dest->j*src->r + dest->k*src->i;
	tmp.k = dest->r*src->k + dest->i*src->j - dest->j*src->i + dest->k*src->r;
	_S_CALL("S_quat_copy", S_quat_copy(dest, &tmp));
}

void
S_quat_conjugate(Squat *quat)
{
	if (!quat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_conjugate");
		return;
	}
	quat->i = -quat->i;
	quat->j = -quat->j;
	quat->k = -quat->k;
}

Sfloat
S_quat_dot(const Squat *a,
           const Squat *b)
{
	if (!a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_dot");
		return 0.0f;
	}
	return a->r*b->r + a->i*b->i + a->j*b->j + a->k*b->k;
}

void
S_quat_normalize(Squat *quat)
{
	Sfloat norm;
	if (!quat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_normalize");
		return;
	}
	_S_CALL("S_quat_dot", norm = S_quat_dot(quat, quat));
	norm = S_sqrt(norm);
	quat->r /= norm;
	quat->i /= norm;
	quat->j /= norm;
	quat->k /= norm;
}

void
S_quat_inverse(Squat *quat)
{
	Sfloat dot;
	Squat conjugate;
	if (!quat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_inverse");
		return;
	}
	_S_CALL("S_quat_dot", dot = S_quat_dot(quat, quat));
	_S_CALL("S_quat_copy", S_quat_copy(&conjugate, quat));
	_S_CALL("S_quat_conjugate", S_quat_conjugate(&conjugate));
	quat->r = conjugate.r / dot;
	quat->i = conjugate.i / dot;
	quat->j = conjugate.j / dot;
	quat->k = conjugate.k / dot;
}

void
S_quat_lerp(Squat *dest,
            const Squat *src,
            Sfloat time)
{
	Sfloat tdiff;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_lerp");
		return;
	}
	time = S_clamp(time, 0.0f, 1.0f);
	tdiff = (1.0f - time);
	dest->r = dest->r*time + src->r*tdiff;
	dest->i = dest->i*time + src->i*tdiff;
	dest->j = dest->j*time + src->j*tdiff;
	dest->k = dest->k*time + src->k*tdiff;
	_S_CALL("S_quat_normalize", S_quat_normalize(dest));
}

void
S_quat_slerp(Squat *dest,
             const Squat *src,
             Sfloat time)
{
	Sfloat theta, stheta, a, b;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_slerp");
		return;
	}
	time = S_clamp(time, 0.0f, 1.0f);
	_S_CALL("S_quat_dot", theta = S_quat_dot(src, dest));
	theta = S_arccos(theta);
	if (theta < 0.0)
		theta = -theta;
	stheta = S_sin(theta);
	a = S_sin((1.0f-time)*theta) / stheta;
	b = S_sin(time*theta) / stheta;
	dest->r = dest->r*b + src->r*a;
	dest->i = dest->i*b + src->i*a;
	dest->j = dest->j*b + src->j*a;
	dest->k = dest->k*b + src->k*a;
	_S_CALL("S_quat_normalize", S_quat_normalize(dest));
}

Sfloat
S_quat_angle(Squat *dest,
             const Squat *src)
{
	Squat inverse;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_angle");
		return S_INFINITY;
	}
	_S_CALL("S_quat_copy", S_quat_copy(&inverse, src));
	_S_CALL("S_quat_inverse", S_quat_inverse(&inverse));
	_S_CALL("S_quat_multiply", S_quat_multiply(dest, &inverse));
	return S_degrees(2.0f * S_arccos(dest->r));
}

void
S_quat_angleaxis(Squat *dest,
                 const Svec3 *axis,
                 Sfloat angle)
{
	Sfloat s, c;
	if (!dest || !axis)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_angleaxis");
		return;
	}
	s = S_sin(S_radians(angle / 2.0f));
	c = S_cos(S_radians(angle / 2.0f));
	dest->i = axis->x * s;
	dest->j = axis->y * s;
	dest->k = axis->z * s;
	dest->r = c;
}

void
S_quat_lookpoint(Squat *dest,
                 const Svec3 *from,
                 const Svec3 *to)
{
	Svec3 forward, world_forward, world_up;
	Sfloat dot, rotangle;
	if (!dest || !from || !to)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_lookat");
		return;
	}
	_S_CALL("S_vec3_set", S_vec3_set(&world_forward, 0.0f, 0.0f, -1.0f));
	/* calculate forward vector */
	_S_CALL("S_vec3_copy", S_vec3_copy(&forward, to));
	_S_CALL("S_vec3_subtract", S_vec3_subtract(&forward, from));
	_S_CALL("S_vec3_normalize", S_vec3_normalize(&forward));

	_S_CALL("S_vec3_dot", dot = S_vec3_dot(&world_forward, &forward));
	if (S_epsilon(S_EPSILON, dot, -1.0f))
	{
		/* directly up */
		_S_CALL("S_vec3_set", S_vec3_set(&world_up, 0.0f, 1.0f, 0.0f));
		_S_CALL("S_quat_angleaxis", S_quat_angleaxis(dest, &world_up, 180.0f));
		return;
	}
	if (S_epsilon(S_EPSILON, dot, 1.0f))
	{
		/* directly down */
		_S_CALL("S_quat_identity", S_quat_identity(dest));
		return;
	}
	/* build rotation */
	rotangle = S_degrees(S_arccos(dot));
	_S_CALL("S_vec3_cross", S_vec3_cross(&forward, &world_forward));
	_S_CALL("S_vec3_normalize", S_vec3_normalize(&forward));
	_S_CALL("S_quat_angleaxis", S_quat_angleaxis(dest, &forward, rotangle));
}


void
S_quat_copy(Squat *dest,
            const Squat *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_copy");
		return;
	}
	dest->r = src->r;
	dest->i = src->i;
	dest->j = src->j;
	dest->k = src->k;
}

Sbool
S_quat_equals(Sfloat epsilon,
              const Squat *a,
              const Squat *b)
{
	Sfloat dot;
	if (epsilon < 0 || !a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_equals");
		return S_FALSE;
	}
	_S_CALL("S_quat_dot", dot = S_quat_dot(a, b));
	return S_abs(dot) > (1.0f - epsilon);
}

/*
 * See:
 * https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
 *
 * ZXY euler system.
 */
void
S_quat_to_vec3(Svec3 *dest,
               const Squat *src)
{
	Sfloat lock;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_quat_to_vec3");
		return;
	}
	lock = src->r*src->k + src->i*src->j;
	if (S_epsilon(S_EPSILON, lock, 0.5f))
	{
		/* north-pole gimbal lock */
		dest->x = 0.0f;
		dest->y = S_degrees(2.0f * S_arctan2(src->i, src->r));
		dest->z = 90.0f;
	}
	else if (S_epsilon(S_EPSILON, lock, -0.5f))
	{
		/* south-pole gimbal lock */
		dest->x = 0.0f;
		dest->y = S_degrees(-2.0f * S_arctan2(src->i, src->r));
		dest->z = -90.0f;
	}
	else
	{
		/* general case */
		dest->x = S_degrees(S_arcsin(
			2.0f * (src->r*src->i - src->j*src->k)
		));
		dest->y = S_degrees(S_arctan2(
			(2.0f * (src->r*src->j + src->k*src->i))
				,
			(1.0f - (2.0f * (src->i*src->i + src->j*src->j)))
		));
		dest->z = S_degrees(S_arctan2(
			(2.0f * (src->r*src->k + src->i*src->j))
				,
			(1.0f - (2.0f * (src->k*src->k + src->i*src->i)))
		));
	}
}

