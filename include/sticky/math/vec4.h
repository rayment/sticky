/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * vec4.h
 * 4D vector header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_VEC4_H
#define FR_RAYMENT_STICKY_VEC4_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/types.h"
#include "sticky/math/math.h"

/**
 * @file vec4.h
 * @addtogroup vec4
 * @{
 */

/**
 * @brief Set a vector to the zero vector.
 *
 * The zero vector @f$\vec{V}@f$ is defined as:
 *
 * @f[
 *     \vec{V}=\left[\begin{array}{c}0 \\ 0 \\ 0 \\ 0\end{array}\right]
 * @f]
 *
 * @param[out] vec The vector to set to zero.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec4_zero(Svec4 *);

/**
 * @brief Set all four vector components to a given value.
 *
 * Takes a vector @f$V@f$ and a scalar @f$s@f$ and sets the vector to:
 *
 * @f[
 *     \vec{V}=\left[\begin{array}{c}s \\ s \\ s \\ s\end{array}\right]
 * @f]
 *
 * @param[out] vec The vector to fill with a given value.
 * @param[in] scalar The scalar value with which to fill the vector.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec4_fill(Svec4 *, Sfloat);

/**
 * @brief Add two vectors together.
 *
 * Takes two vectors @f$\vec{V_1}@f$ and @f$\vec{V_2}@f$ and adds together each
 * @f$x@f$, @f$y@f$, @f$z@f$ and @f$w@f$ component as follows:
 *
 * @f[
 *     \vec{V_1}+\vec{V_2}=\left[\begin{array}{c}
 *         x_1+x_2 \\ y_1+y_2 \\ z_1+z_2 \\ w_1+w_2
 *     \end{array}\right]
 * @f]
 *
 * @param[out] dest The first vector to add, and the result destination.
 * @param[in] src The second vector to add.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec4_add(Svec4 *, const Svec4 *);

/**
 * @brief Multiply two vectors together.
 *
 * Takes two vectors @f$\vec{V_1}@f$ and @f$\vec{V_2}@f$ and multiplies together
 * each @f$x@f$, @f$y@f$, @f$z@f$ and @f$w@f$ component as follows:
 *
 * @f[
 *     \vec{V_1}\vec{V_2}=\left[\begin{array}{c}
 *         x_1.x_2 \\ y_1.y_2 \\ z_1.z_2 \\ w_1.w_2
 *     \end{array}\right]
 * @f]
 *
 * @param[out] dest The first vector to multiply, and the result destination.
 * @param[in] src The second vector to multiply.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec4_multiply(Svec4 *, const Svec4 *);

/**
 * @brief Scale all four vector components by a given value.
 *
 * Takes a vector @f$V@f$ and a scalar @f$s@f$ and scales the vector by the
 * following equation:
 *
 * @f[
 *     \vec{V}=\left[\begin{array}{c}sx \\ sy \\ sz \\ sw\end{array}\right]
 * @f]
 *
 * Where @f$x@f$, @f$y@f$, @f$z@f$ and @f$w@f$ are the four components currently
 * stored in the vector.
 *
 * @param[out] vec The vector to scale by a given value.
 * @param[in] scalar The scalar value with which to scale the vector.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec4_scale(Svec4 *, Sfloat);

/**
 * @brief Calculate the dot product of two vectors.
 *
 * The dot product of two 4D vectors @f$V_1@f$ and @f$V_1@f$ is defined as:
 *
 * @f[
 *     \vec{V_1}\cdot \vec{V_2}=x_1 x_2+y_1 y_2+z_1 z_2+w_1 w_2
 * @f]
 *
 * @param[in,out] a The first vector from which to extract the dot product.
 * @param[in] b The second vector from which to extract the dot product.
 * @return The dot product @f$V_1\cdot V_2@f$ of two vectors.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4D vector is provided
 * to the function.
 * @since 1.0.0
 */
Sfloat S_vec4_dot(const Svec4 *, const Svec4 *);

/**
 * @brief Normalise a vector.
 *
 * Takes a vector and overwrites it with its normalised equivalent. A normalised
 * vector is a unit vector because it has a distance of @f$1@f$ from the origin.
 *
 * A unit vector @f$\hat{u}@f$ of a vector @f$u@f$ is defined as:
 *
 * @f[
 *    \hat{u}=\frac{u}{\vert u\vert}
 * @f]
 *
 * That is, the unit vector @f$\hat{u}@f$ is equal to the vector @f$u@f$ divided
 * by its norm @f$\vert u\vert@f$, or, the Euclidian distance from the vector to
 * the origin.
 *
 * @param[in,out] vec The vector to normalise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec4_normalize(Svec4 *);

/**
 * @brief Invert a vector.
 *
 * The inverse @f$V^{-1}@f$ of a 4D vector @f$V@f$ is defined as:
 *
 * @f[
 *     V^{-1}=\left[\begin{array}{c}
 *         \frac{1}{x} \\ \frac{1}{y} \\ \frac{1}{z} \\ \frac{1}{w}
 *     \end{array}\right]
 * @f]
 *
 * @param[in,out] vec The vector to invert.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec4_inverse(Svec4 *);

/**
 * @brief Perform a linear interpolation on two vectors.
 *
 * Linearly interpolates from a vector @f$V_1@f$ to another @f$V_2@f$ for a
 * given time offset, and stores the result.
 *
 * @param[in,out] dest The @f$V_2@f$ vector to which the interpolation is
 * applied, and the result destination.
 * @param[in] src The @f$V_1@f$ vector from which the interpolation is applied.
 * @param[in] time The time offset to apply to the interpolation. This value
 * will be clamped to the range @f$[0,1]@f$.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec4_lerp(Svec4 *, const Svec4 *, Sfloat);

/**
 * @brief Create a copy of a vector.
 *
 * Creates a duplicate copy of a given 4D vector to a given destination pointer.
 *
 * @param[out] dest The destination vector to copy the vector to.
 * @param[in] src The source vector to copy.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec4_copy(Svec4 *, const Svec4 *);

/**
 * @brief Check if two vectors are equivalent.
 *
 * Two 4D vectors are equivalent if the corresponding @f$x@f$, @f$y@f$, @f$z@f$
 * and @f$w@f$ components are equal to one another, taking into account a small
 * floating-point error @f$\epsilon@f$.
 *
 * @param[in] epsilon Maximum error between each float comparison for two floats
 * to be considered equivalent.
 * @param[in] a The first vector to compare.
 * @param[in] b The second vector to compare.
 * @return {@link S_TRUE} only if two vectors are equivalent.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4D vector is provided
 * to the function.
 * @since 1.0.0
 */
Sbool  S_vec4_equals(Sfloat, const Svec4 *, const Svec4 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_VEC4_H */

