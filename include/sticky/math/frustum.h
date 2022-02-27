/*
 * frustum.h
 * Culling frustum math header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 26/02/2022
 */

#ifndef FR_RAYMENT_STICKY_FRUSTUM_H
#define FR_RAYMENT_STICKY_FRUSTUM_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/types.h"
#include "sticky/math/vec3.h"

typedef struct
Sfrustum_s
{
	Svec3 fnear[3], ffar[3], center;
	struct
	Splane_s
	{
		Svec3 vec;
		Sfloat d;
	} planes[6];
} Sfrustum;

Sfrustum *S_frustum_new(void);
void      S_frustum_delete(Sfrustum *);

Sbool     S_frustum_intersects_point(const Sfrustum *, const Svec3 *);
Sbool     S_frustum_intersects_bounds(const Sfrustum *,
                                      const Svec3 *, const Svec3 *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_FRUSTUM_H */

