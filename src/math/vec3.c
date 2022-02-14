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

