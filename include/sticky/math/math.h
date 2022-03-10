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

#include "sticky/common/defines.h"

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
#define S_EPSILON    0.0001f
/**
 * @brief The value of @f$\pi@f$.
 *
 * @since 1.0.0
 */
#define S_PI         3.1415926536f
/**
 * @brief The value of @f$2\pi@f$.
 *
 * @since 1.0.0
 */
#define S_TWOPI      6.2831853072f
/**
 * @brief The value of @f$\frac{\pi}{2}@f$.
 *
 * @since 1.0.0
 */
#define S_HALFPI     1.5707963268f
/**
 * @brief The value of @f$\sqrt{\pi}@f$.
 *
 * @since 1.0.0
 */
#define S_SQRTPI     1.7724538509f
/**
 * @brief The value of @f$\frac{\sqrt{\pi}}{2}@f$.
 *
 * @since 1.0.0
 */
#define S_HALFSQRTPI 0.8862269255f
/**
 * @brief The infinity value for single-precision floating-point numbers.
 *
 * @since 1.0.0
 */
#define S_INFINITY   INFINITY

/**
 * @}
 */

/**
 * @addtogroup vec2
 * @{
 */

/**
 * @brief Single-precision floating-point 2D vector.
 *
 * 2D vectors may used for positions, rotations, velocities and other properties
 * pertaining to 2D space. Each vector @f$\vec{V}@f$has two axis components,
 * namely @f$(x,y)@f$.
 *
 * @since 1.0.0
 */
#ifdef DOXYGEN
typedef struct
Svec2_s
{
	Sfloat x, y;
} Svec2;
#else /* DOXYGEN */
ALIGN(4,
typedef struct
Svec2_s
{
	Sfloat x, y;
}) Svec2;
#endif /* DOXYGEN */

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
#ifdef DOXYGEN
typedef struct
Svec3_s
{
	Sfloat x, y, z;
} Svec3;
#else /* DOXYGEN */
ALIGN(4,
typedef struct
Svec3_s
{
	Sfloat x, y, z;
}) Svec3;
#endif /* DOXYGEN */

/**
 * @}
 */

/**
 * @addtogroup vec4
 * @{
 */

/**
 * @brief Single-precision floating-point 4D vector.
 *
 * Each vector @f$\vec{V}@f$has four axis components, namely @f$(x,y,z,w)@f$.
 *
 * @since 1.0.0
 */
#ifdef DOXYGEN
typedef struct
Svec4_s
{
	Sfloat x, y, z, w;
} Svec4;
#else /* DOXYGEN */
ALIGN(4,
typedef struct
Svec4_s
{
	Sfloat x, y, z, w;
}) Svec4;
#endif /* DOXYGEN */

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
#ifdef DOXYGEN
typedef struct
Squat_s
{
	Sfloat r, i, j, k;
} Squat;
#else /* DOXYGEN */
ALIGN(4,
typedef struct
Squat_s
{
	Sfloat r, i, j, k;
}) Squat;
#endif /* DOXYGEN */

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
#ifdef DOXYGEN
typedef struct
Smat3_s
{
	Sfloat m00, m10, m20,
	       m01, m11, m21,
	       m02, m12, m22;
} Smat3;
#else /* DOXYGEN */
ALIGN(4,
typedef struct
Smat3_s
{
	Sfloat m00, m10, m20,
	       m01, m11, m21,
	       m02, m12, m22;
}) Smat3;
#endif /* DOXYGEN */

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
#ifdef DOXYGEN
typedef struct
Smat4_s
{
	Sfloat m00, m10, m20, m30,
	       m01, m11, m21, m31,
	       m02, m12, m22, m32,
	       m03, m13, m23, m33;
} Smat4;
#else /* DOXYGEN */
ALIGN(4,
typedef struct
Smat4_s
{
	Sfloat m00, m10, m20, m30,
	       m01, m11, m21, m31,
	       m02, m12, m22, m32,
	       m03, m13, m23, m33;
}) Smat4;
#endif /* DOXYGEN */

/**
 * @}
 */

/**
 * @addtogroup scalar
 * @{
 */

/**
 * @brief Get the absolute value of a single-precision floating-point number.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @return @f$\mathrm{abs}(x)@f$
 * @since 1.0.0
 */
#define S_abs(x)          fabsf(x)

/**
 * @brief Return the smallest of two single-precision floating-point numbers.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @param y A single-precision floating-point number.
 * @return @f$\mathrm{min}(x,y)@f$
 * @since 1.0.0
 */
#define S_min(x,y)        ((x)<(y) ? (x) : (y))

/**
 * @brief Return the largest of two single-precision floating-point numbers.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @param y A single-precision floating-point number.
 * @return @f$\mathrm{max}(x,y)@f$
 * @since 1.0.0
 */
#define S_max(x,y)        ((x)>(y) ? (x) : (y))

/**
 * @brief Calculate the square root of a single-precision floating-point number.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @return @f$\sqrt{x}@f$
 * @since 1.0.0
 */
#define S_sqrt(x)         sqrtf(x)

/**
 * @brief Convert a single-precision floating-point number from radians to
 * degrees.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @return @f$x\frac{\pi}{180}@f$
 * @since 1.0.0
 */
#define S_radians(x)      ((x) * 0.01745329252f)

/**
 * @brief Convert a single-precision floating-point number from degrees to
 * radians.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @return @f$x\frac{180}{\pi}@f$
 * @since 1.0.0
 */
#define S_degrees(x)      ((x) * 57.2957795131f)

/**
 * @brief Calculate the sine of a single-precision floating-point number.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @return @f$\sin(x)@f$
 * @since 1.0.0
 */
#define S_sin(x)          sinf(x)

/**
 * @brief Calculate the cosine of a single-precision floating-point number.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @return @f$\cos(x)@f$
 * @since 1.0.0
 */
#define S_cos(x)          cosf(x)

/**
 * @brief Calculate the tangent of a single-precision floating-point number.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @return @f$\tan(x)@f$
 * @since 1.0.0
 */
#define S_tan(x)          tanf(x)

/**
 * @brief Calculate the inverse sine of a single-precision floating-point number.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @return @f$\sin^{-1}(x)@f$
 * @since 1.0.0
 */
#define S_arcsin(x)       asinf(x)

/**
 * @brief Calculate the inverse cosine of a single-precision floating-point
 * number.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @return @f$\cos^{-1}(x)@f$
 * @since 1.0.0
 */
#define S_arccos(x)       acosf(x)

/**
 * @brief Calculate the inverse tangent of a single-precision floating-point
 * number.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @return @f$\tan^{-1}(x)@f$
 * @since 1.0.0
 */
#define S_arctan(x)       atanf(x)

/**
 * @brief Calculate the 2-argument inverse tangent of a single-precision
 * floating-point number.
 * @hideinitializer
 *
 * @param x A single-precision floating-point number.
 * @param y A single-precision floating-point number.
 * @return @f$\mathrm{atan2}(x,y)@f$
 * @since 1.0.0
 */
#define S_arctan2(x,y)    atan2f(x,y)

/**
 * @brief Compare two single-precision floating-point numbers given a maximum
 * acceptable of error.
 * @hideinitializer
 *
 * The equation for two values @f$a@f$ and @f$b@f$ and an acceptable error
 * @f$\epsilon@f$ is defined as:
 *
 * @f[
 *     \vert a-b\vert < \epsilon
 * @f]
 *
 * @param x Maximum error between the two single-precision floating-point
 * numbers for them to be considered equivalent. This is also a single-precision
 * floating-point number.
 * @param y The first single-precision floating-point number to compare.
 * @param z The second single-precision floating-point number to compare.
 * @return {@link S_TRUE} If the two values @p y and @p z are equivalent.
 * @warning This function expects the epsilon parameter @p x to be more than or
 * equal to zero. The return result is undefined for negative numbers.
 * @since 1.0.0
 */
#define S_epsilon(x,y,z) ((y) > ((z)-(x)) && (y) < ((z)+(x)))

/**
 * @brief Clamp a single-precision floating-point number between a maximum and
 * minimum value.
 * @hideinitializer
 *
 * @param x The single-precision floating-point number to clamp.
 * @param y The minimum value of @p x, also a single-precision floating-point
 * number.
 * @param z The maximum value of @p x, also single-precision floating-point
 * number.
 * @return If @p x < @p y, then y. If @p x > @p z, then @p z. Otherwise @p x.
 * @since 1.0.0
 */
#define S_clamp(x,y,z)   ((x) < (y) ? y : ((x) > (z) ? z : (x)))

/**
 * @brief Determine if a floating-point number is negative.
 * @hideinitializer
 *
 * Returns {@link S_TRUE} if the sign of a floating-point number is set to
 * negative, or otherwise {@link S_FALSE}.
 *
 * Note that while @f$-0@f$ and @f$0@f$ are equivalent, they do not have the
 * same sign.
 *
 * @param x The floating-point number.
 * @return If x is negative, then {@link S_TRUE}. Otherwise {@link S_FALSE}.
 * @since 1.0.0
 */
#define S_sign(x) (signbit(x) ? S_TRUE : S_FALSE)

/**
 * @brief Copy the sign of a single-precision floating-point number to another.
 * @hideinitializer
 *
 * Takes the sign from a single-precision floating-point number @p y and applies
 * it to another number @p x which acts as the magnitude.
 *
 * @param x The single-precision floating-point number to act as the magnitude.
 * @param y The single-precision floating-point number from which to extract the
 * sign.
 * @return @p x with the sign of @p y.
 * @since 1.0.0
 */
#define S_copysign(x,y) copysignf(x, y)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MATH_H */

