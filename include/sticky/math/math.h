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

#include <math.h>

/**
 * @file math.h
 */

/**
 * @addtogroup math_constants
 * @{
 */

/**
 * @brief Maximum error between two single-precision floating-point numbers for
 * them to be considered equivalent.
 *
 * @since 1.0.0
 */
#define S_EPSILON  0.0001f
/**
 * @brief The value of @f$\pi@f$.
 *
 * @since 1.0.0
 */
#define S_PI       3.1415926536f
/**
 * @brief The value of @f$\frac{\pi}{2}@f$.
 *
 * @since 1.0.0
 */
#define S_HALFPI   1.5707963268f
/**
 * @brief The infinity value for single-precision floating-point numbers.
 *
 * @since 1.0.0
 */
#define S_INFINITY INFINITY

/**
 * @}
 */

/**
 * @addtogroup vec3
 * @{
 */

/**
 * @brief Single-precision floating-point 3D vector.
 *
 * 3D vectors may used for positions, rotations, velocities and other properties
 * pertaining to 3D space. Each vector @f$\vec{V}@f$has three axis components,
 * namely @f$(x,y,z)@f$.
 *
 * @since 1.0.0
 */
typedef struct
Svec3_s
{
	Sfloat x, y, z;
} Svec3;

/**
 * @}
 */

/**
 * @addtogroup quat
 * @{
 */

/**
 * @brief Rotational quaternion.
 *
 * Quaternions allow for 3D rotations without suffering from gimbal lock and use
 * complex numbers to facilitate this.
 *
 * For the purpose of this documentation, quaternions will be shown in the form:
 *
 * @f[
 *     q=a+b\mathbf{i}+c\mathbf{j}+d\mathbf{k}
 * @f]
 *
 * where @f$a@f$, @f$b@f$, @f$c@f$ and @f$d@f$ are real numbers.
 *
 * Quaternion components may be referenced in superscript, such as @f$q^0@f$ and
 * @f$q^3@f$ which refer to the real and @f$k@f$ components respectively.
 *
 * @since 1.0.0
 */
typedef struct
Squat_s
{
	Sfloat r, i, j, k;
} Squat;

/**
 * @}
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

/**
 * @addtogroup scalar
 * @{
 */

#define S_abs(x)          fabsf(x)
#define S_sqrt(x)         sqrtf(x)
#define S_radians(x)      ((x) * 0.01745329252f)
#define S_degrees(x)      ((x) * 57.2957795131f)
#define S_sin(x)          sinf(x)
#define S_cos(x)          cosf(x)
#define S_tan(x)          tanf(x)
#define S_arcsin(x)       asinf(x)
#define S_arccos(x)       acosf(x)
#define S_arctan(x)       atanf(x)
#define S_arctan2(x,y)    atan2f(x,y)
#define S_epsilon(x,y,z) ((y) > ((z)-(x)) && (y) < ((z)+(x)))
#define S_clamp(x,y,z)   ((x) < (y) ? y : ((x) > (z) ? z : (x)))

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MATH_H */

