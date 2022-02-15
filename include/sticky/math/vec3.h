/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * vec3.h
 * 3D vector header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_VEC3_H
#define FR_RAYMENT_STICKY_VEC3_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/types.h"
#include "sticky/math/math.h"

/**
 * @file vec3.h
 * @addtogroup vec3
 * @{
 */

void   S_vec3_zero(Svec3 *);
void   S_vec3_fill(Svec3 *, Sfloat);
void   S_vec3_add(Svec3 *, const Svec3 *);
void   S_vec3_multiply(Svec3 *, const Svec3 *);
void   S_vec3_scale(Svec3 *, Sfloat);
Sfloat S_vec3_dot(const Svec3 *, const Svec3 *);
void   S_vec3_cross(Svec3 *, const Svec3 *);
void   S_vec3_normalize(Svec3 *);
void   S_vec3_inverse(Svec3 *);
void   S_vec3_lerp(Svec3 *, const Svec3 *, Sfloat);
void   S_vec3_copy(Svec3 *, const Svec3 *);
Sbool  S_vec3_equals(Sfloat, const Svec3 *, const Svec3 *);
/**
 * @brief temp
 */
void   S_vec3_to_quat(Squat *, const Svec3 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_VEC3_H */

