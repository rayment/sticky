/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mat3.h
 * 3x3 matrix header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_MAT3_H
#define FR_RAYMENT_STICKY_MAT3_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/types.h"
#include "sticky/math/math.h"

/**
 * @file mat3.h
 * @addtogroup mat3
 * @{
 */

void  S_mat3_identity(Smat3 *);
void  S_mat3_multiply(Smat3 *, const Smat3 *);
void  S_mat3_transpose(Smat3 *);
Sbool S_mat3_inverse(Smat3 *);
void  S_mat3_copy(Smat3 *, const Smat3 *);
Sbool S_mat3_equals(Sfloat, const Smat3 *, const Smat3 *);
void  S_mat3_to_mat4(Smat4 *, const Smat3 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MAT3_H */

