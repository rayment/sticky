/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * quat.h
 * Quaternion header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_QUAT_H
#define FR_RAYMENT_STICKY_QUAT_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/types.h"
#include "sticky/math/math.h"

/**
 * @file quat.h
 * @addtogroup quat
 * @{
 */

void   S_quat_identity(Squat *);
void   S_quat_multiply(Squat *, const Squat *);
void   S_quat_conjugate(Squat *);
Sfloat S_quat_dot(const Squat *, const Squat *);
void   S_quat_normalize(Squat *);
void   S_quat_inverse(Squat *);
void   S_quat_lerp(Squat *, const Squat *, Sfloat);
Sfloat S_quat_angle(Squat *, const Squat *);
void   S_quat_copy(Squat *, const Squat *);
Sbool  S_quat_equals(Sfloat, const Squat *, const Squat *);
void   S_quat_to_vec3(Svec3 *, const Squat *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_QUAT_H */

