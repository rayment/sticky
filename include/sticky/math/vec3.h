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
 * @addtogroup vec3
 * @{
 */

/**
 * @brief Set a vector to the zero vector.
 *
 * The zero vector @f$\vec{V}@f$ is defined as:
 *
 * @f[
 *     \vec{V}=\left[\begin{array}{c}0 \\ 0 \\ 0\end{array}\right]
 * @f]
 *
 * @param[out] vec The vector to set to zero.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec3_zero(Svec3 *);

/**
 * @brief Set all three vector components to a given value.
 *
 * Takes a vector @f$V@f$ and a scalar @f$s@f$ and sets the vector to:
 *
 * @f[
 *     \vec{V}=\left[\begin{array}{c}s \\ s \\ s\end{array}\right]
 * @f]
 *
 * @param[out] vec The vector to fill with a given value.
 * @param[in] scalar The scalar value with which to fill the vector.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec3_fill(Svec3 *, Sfloat);

/**
 * @brief Set the three vector components of a vector.
 *
 * Takes a vector @f$V@f$ and three scalars @f$x@f$, @f$y@f$ and @f$z@f$ and
 * sets the vector to:
 *
 * @f[
 *     \vec{V}=\left[\begin{array}{c}x \\ y \\ z\end{array}\right]
 * @f]
 *
 * @param[out] dest The vector to set.
 * @param[in] x The scar value for the @f$x@f$ axis.
 * @param[in] y The scar value for the @f$y@f$ axis.
 * @param[in] z The scar value for the @f$z@f$ axis.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec3_set(Svec3 *, Sfloat, Sfloat, Sfloat);

/**
 * @brief Add two vectors together.
 *
 * Takes two vectors @f$\vec{V_1}@f$ and @f$\vec{V_2}@f$ and adds together each
 * @f$x@f$, @f$y@f$ and @f$z@f$ component as follows:
 *
 * @f[
 *     \vec{V_1}+\vec{V_2}=\left[\begin{array}{c}
 *         x_1+x_2 \\ y_1+y_2 \\ z_1+z_2
 *     \end{array}\right]
 * @f]
 *
 * @param[out] dest The first vector to add, and the result destination.
 * @param[in] src The second vector to add.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec3_add(Svec3 *, const Svec3 *);

/**
 * @brief Multiply two vectors together.
 *
 * Takes two vectors @f$\vec{V_1}@f$ and @f$\vec{V_2}@f$ and multiplies together
 * each @f$x@f$, @f$y@f$ and @f$z@f$ component as follows:
 *
 * @f[
 *     \vec{V_1}\vec{V_2}=\left[\begin{array}{c}
 *         x_1.x_2 \\ y_1.y_2 \\ z_1.z_2
 *     \end{array}\right]
 * @f]
 *
 * @param[out] dest The first vector to multiply, and the result destination.
 * @param[in] src The second vector to multiply.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec3_multiply(Svec3 *, const Svec3 *);

/**
 * @brief Scale all three vector components by a given value.
 *
 * Takes a vector @f$V@f$ and a scalar @f$s@f$ and scales the vector by the
 * following equation:
 *
 * @f[
 *     \vec{V}=\left[\begin{array}{c}sx \\ sy \\ sz\end{array}\right]
 * @f]
 *
 * Where @f$x@f$, @f$y@f$ and @f$z@f$ are the three components currently stored
 * in the vector.
 *
 * @param[out] vec The vector to scale by a given value.
 * @param[in] scalar The scalar value with which to scale the vector.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec3_scale(Svec3 *, Sfloat);

/**
 * @brief Calculate the dot product of two vectors.
 *
 * The dot product of two 3D vectors @f$V_1@f$ and @f$V_1@f$ is defined as:
 *
 * @f[
 *     \vec{V_1}\cdot \vec{V_2}=x_1 x_2+y_1 y_2+z_1 z_2
 * @f]
 *
 * @param[in] a The first vector from which to extract the dot product.
 * @param[in] b The second vector from which to extract the dot product.
 * @return The dot product @f$V_1\cdot V_2@f$ of two vectors.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
Sfloat S_vec3_dot(const Svec3 *, const Svec3 *);

/**
 * @brief Calculate the cross product of two vectors.
 *
 * The cross product of two 3D vectors @f$V_1@f$ and @f$V_1@f$ is defined as:
 *
 * @f[
 *     \vec{V_1}\times \vec{V_2}=\left[\begin{array}{c}
 *         y_1 z_2 - z_1 y_2 \\ z_1 x_2 - x_1 z_2 \\ x_1 y_2 - y_1 x_2
 *     \end{array}\right]
 * @f]
 *
 * @param[in,out] dest The vector @f$V_2@f$ from which to extract the cross
 * product, and the result destination.
 * @param[in] src The vector @f$V_1@f$ from which to extract the cross product.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec3_cross(Svec3 *, const Svec3 *);

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
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec3_normalize(Svec3 *);

/**
 * @brief Invert a vector.
 *
 * The inverse @f$V^{-1}@f$ of a 3D vector @f$V@f$ is defined as:
 *
 * @f[
 *     V^{-1}=\left[\begin{array}{c}
 *         \frac{1}{x} \\ \frac{1}{y} \\ \frac{1}{z}
 *     \end{array}\right]
 * @f]
 *
 * @param[in,out] vec The vector to invert.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec3_inverse(Svec3 *);

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
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec3_lerp(Svec3 *, const Svec3 *, Sfloat);

/**
 * @brief Create a copy of a vector.
 *
 * Creates a duplicate copy of a given 3D vector to a given destination pointer.
 *
 * @param[out] dest The destination vector to copy the vector to.
 * @param[in] src The source vector to copy.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
void   S_vec3_copy(Svec3 *, const Svec3 *);

/**
 * @brief Check if two vectors are equivalent.
 *
 * Two 3D vectors are equivalent if the corresponding @f$x@f$, @f$y@f$ and
 * @f$z@f$ components are equal to one another, taking into account a small
 * floating-point error @f$\epsilon@f$.
 *
 * @param[in] epsilon Maximum error between each float comparison for two floats
 * to be considered equivalent.
 * @param[in] a The first vector to compare.
 * @param[in] b The second vector to compare.
 * @return {@link S_TRUE} only if two vectors are equivalent.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector is provided
 * to the function.
 * @since 1.0.0
 */
Sbool  S_vec3_equals(Sfloat, const Svec3 *, const Svec3 *);

/**
 * @brief Convert Euler angles to a quaternion.
 *
 * Turns a 3D vector that contains Euler angles into the corresponding
 * quaternion representation. Euler angles are measured in degrees and define a
 * given rotation around the @f$XYZ@f$ axis. Quaternions should primarily be
 * used for rotations to avoid gimbal-lock that occurs in degenerate 3D vectors.
 *
 * This function expects the vector to be in degrees. The output of a vector
 * measured in radians will not return the correct resulting quaternion.
 *
 * @param[out] dest The destination quaternion.
 * @param[in] src The source EUler angles to convert to quaternion-form.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3D vector or
 * quaternion pointer is provided
 * to the function.
 * @see {@link S_quat_to_vec3} To turn a quaternion back into Euler angles.
 * @since 1.0.0
 */
void   S_vec3_to_quat(Squat *, const Svec3 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_VEC3_H */

