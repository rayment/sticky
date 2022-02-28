/*
 * frustum.c
 * Culling frustum math source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/02/2022
 */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/frustum.h"
#include "sticky/math/vec3.h"
#include "sticky/memory/allocator.h"

Sfrustum *
S_frustum_new(void)
{
	Sfrustum *frustum;
	Suint8 i;
	frustum = S_memory_new(sizeof(Sfrustum));
	for (i = 0; i < 3; ++i)
	{
		_S_CALL("S_vec3_zero", S_vec3_zero(frustum->fnear+i));
		_S_CALL("S_vec3_zero", S_vec3_zero(frustum->ffar+i));
	}
	_S_CALL("S_vec3_zero", S_vec3_zero(&frustum->center));
	memset(frustum->planes, 0, sizeof(frustum->planes));
	return frustum;
}

void
S_frustum_delete(Sfrustum *frustum)
{
	if (!frustum)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_frustum_delete");
		return;
	}
	S_memory_delete(frustum);
}

Sbool     S_frustum_intersects_point(const Sfrustum *, const Svec3 *);
Sbool     S_frustum_intersects_bounds(const Sfrustum *,
                                      const Svec3 *, const Svec3 *);

