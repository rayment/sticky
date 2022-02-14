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

Sbool  S_vec3_equals(Sfloat, const Svec3 *, const Svec3 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_VEC3_H */

