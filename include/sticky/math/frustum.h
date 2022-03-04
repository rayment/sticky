/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

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

/**
 * @addtogroup frustum
 * @{
 */

#define S_FRUSTUM_RIGHT  0
#define S_FRUSTUM_LEFT   1
#define S_FRUSTUM_BOTTOM 2
#define S_FRUSTUM_TOP    3
#define S_FRUSTUM_FAR    4
#define S_FRUSTUM_NEAR   5

typedef Svec4 Sfrustum[6];

/**
 * @brief Load a frustum for a given camera.
 *
 * Stores a frustum for a given camera that is calculated from the cameras
 * perspective and view matrices.
 *
 * @param[out] frustum The frustum to create for a given camera.
 * @param[in] camera The camera from which to create the frustum.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid frustum or camera is
 * provided to the function.
 * @since 1.0.0
 */
void  S_frustum_load(Sfrustum *, const Scamera *);

/**
 * @brief Check if a point lies within a frustum.
 *
 * Note that the point @p point is calculated from world space, and not any
 * location space of, say, a transform.
 *
 * @param[in] frustum The frustum.
 * @param[in] point The point that should lie within the frustum.
 * @return {@link S_TRUE} If a given point lies within a frustum, otherwise
 * {@link S_FALSE}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid frustum or 3D point
 * is provided to the function.
 * @since 1.0.0
 */
Sbool S_frustum_intersects_point(const Sfrustum *, const Svec3 *);

/**
 * @brief Check if a sphere lies within a frustum.
 *
 * Note that the point @p point is calculated from world space, and not any
 * location space of, say, a transform.
 *
 * @param[in] frustum The frustum.
 * @param[in] point The center point of the sphere.
 * @param[in] radius The radius of the sphere.
 * @return {@link S_TRUE} If a given sphere lies within a frustum, otherwise
 * {@link S_FALSE}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid frustum or 3D point
 * is provided to the function.
 * @since 1.0.0
 */
Sbool S_frustum_intersects_sphere(const Sfrustum *, const Svec3 *, Sfloat);

Sbool S_frustum_intersects_bounds(const Sfrustum *,
                                  const Svec3 *, const Svec3 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_FRUSTUM_H */

