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
#include "sticky/math/vec4.h"
#include "sticky/video/camera.h"

#define S_FRUSTUM_RIGHT  0
#define S_FRUSTUM_LEFT   1
#define S_FRUSTUM_BOTTOM 2
#define S_FRUSTUM_TOP    3
#define S_FRUSTUM_FAR    4
#define S_FRUSTUM_NEAR   5

typedef Svec4 Sfrustum[6];

void  S_frustum_load(Sfrustum *, const Scamera *);

Sbool S_frustum_intersects_point(const Sfrustum *, const Svec3 *);
Sbool S_frustum_intersects_bounds(const Sfrustum *,
                                  const Svec3 *, const Svec3 *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_FRUSTUM_H */

