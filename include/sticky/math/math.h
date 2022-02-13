/*
 * math.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/02/2022
 */

#ifndef FR_RAYMENT_STICKY_MATH_H
#define FR_RAYMENT_STICKY_MATH_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @file math.h
 */

/**
 * @addtogroup mat3
 * @{
 */

/**
 * @brief 3x3 single-precision floating-point matrix.
 *
 * Matrices may be directly cast into {@link Sfloat Sfloat *} and accessed in
 * column-major order. That is, <c>m[0] ... m[2]</c> refer to the first
 * <b>column</b> of the matrix.
 *
 * A 3x3 matrix @f$M@f$ is defined as follows:
 *
 * @f[
 *     M=\left[{\begin{array}{ccc}
 *         a & d & g \\
 *         b & e & h \\
 *         c & f & i
 *     \end{array}}\right]
 * @f]
 *
 * @since 1.0.0
 */
typedef struct
Smat3_s
{
	Sfloat m00, m10, m20,
	       m01, m11, m21,
	       m02, m12, m22;
} Smat3;

/**
 * @}
 */

/**
 * @addtogroup mat4
 * @{
 */

/**
 * @brief 4x4 single-precision floating-point matrix.
 *
 * Matrices may be directly cast into {@link Sfloat Sfloat *} and accessed in
 * column-major order. That is, <c>m[0] ... m[3]</c> refer to the first
 * <b>column</b> of the matrix.
 *
 * A 4x4 matrix @f$M@f$ is defined as follows:
 *
 * @f[
 *     M=\left[{\begin{array}{cccc}
 *         a & e & i & m \\
 *         b & f & j & n \\
 *         c & g & k & o \\
 *         d & h & l & p
 *     \end{array}}\right]
 * @f]
 *
 * @since 1.0.0
 */
typedef struct
Smat4_s
{
	Sfloat m00, m10, m20, m30,
	       m01, m11, m21, m31,
	       m02, m12, m22, m32,
	       m03, m13, m23, m33;
} Smat4;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MATH_H */

