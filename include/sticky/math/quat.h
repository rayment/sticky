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

/**
 * @brief Set a quaternion to the identity quaternion.
 *
 * The identity quaternion @f$I@f$ is defined as:
 *
 * @f[
 *     \begin{align}
 *         I&=1+0\mathbf{i}+0\mathbf{j}+0\mathbf{k} \\
 *          &=1
 *     \end{align}
 * @f]
 *
 * @param[out] quat The quaternion to set to the identity quaternion.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion is provided
 * to the function.
 * @since 1.0.0
 */
void   S_quat_identity(Squat *);

/**
 * @brief Multiply two quaternions.
 *
 * Takes two quaternions, multiplies them, and replaces the second quaternion
 * with the result.
 *
 * The multiplication of two quaternions @f$q_1@f$ and @f$q_2@f$ is defined as
 * @f$q_2 q_1@f$ due to non-commutativity of the multiplication operation. As
 * such, the second quaternion must be multiplied first.
 *
 * @param[in,out] dest The @f$q_2@f$ quaternion to multiply, and result
 * destination.
 * @param[in] src The @f$q_1@f$ quaternion to multiply.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion is provided
 * to the function.
 * @since 1.0.0
 */
void   S_quat_multiply(Squat *, const Squat *);

/**
 * @brief Get the conjugate value of a quaternion.
 *
 * Takes a quaternion and replaces it with its conjugate value. The conjugate
 * @f$q^\ast@f$ of a quaternion is defined as:
 *
 * @f[
 *     q^\ast=a-b\mathbf{i}-c\mathbf{j}-d\mathbf{k}
 * @f]
 *
 * @param[in,out] quat The quaternion to replace with its conjugate.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion is provided
 * to the function.
 * @since 1.0.0
 */
void   S_quat_conjugate(Squat *);

/**
 * @brief Calculate the dot product of two quaternions.
 *
 * The dot product of two quaternions @f$q_1@f$ and @f$q_2@f$ is defined as:
 *
 * @f[
 *     q_1\cdot q_2=q_1^0 q_2^0+q_1^1 q_2^1+q_1^2 q_2^2+q_1^3 q_2^3
 * @f]
 *
 * @param[in] a The first quaternion.
 * @param[in] b The second quaternion.
 * @return The dot product @f$q_1\cdot q_2@f$ of two quaternions.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion is provided
 * to the function.
 * @since 1.0.0
 */
Sfloat S_quat_dot(const Squat *, const Squat *);

/**
 * @brief Normalise a quaternion.
 *
 * Takes a quaternion and overwrites it with its normalised equivalent. A
 * normalised quaternion is a unit quaternion because it has a distance of
 * @f$1@f$ from the origin.
 *
 * A unit quaternion @f$U_q@f$ is defined as:
 *
 * @f[
 *     \begin{align}
 *     U_q&=\frac{q}{\Vert q\Vert} \\
 *         &=\frac{q^0}{\Vert q\Vert}+\frac{q^1}{\Vert q\Vert}\mathbf{i}+
 *	       \frac{q^2}{\Vert q\Vert}\mathbf{j}+\frac{q^3}{\Vert q\Vert}\mathbf{k}
 *	   \end{align}
 * @f]
 *
 * @param[in,out] quat The quaternion to be normalised.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion is provided
 * to the function.
 * @since 1.0.0
 */
void   S_quat_normalize(Squat *);

/**
 * @brief Calculate the inverse of a quaternion.
 *
 * Takes a quaternion and replaces it with its inverse. The inverse @f$q^{-1}@f$
 * of a quaternion @f$q@f$ is defined as:
 *
 * @f[
 *     q^{-1}=\frac{q^\ast}{\Vert q\Vert^2}
 * @f]
 *
 * That is, the inverse of a quaternion is equal to its conjugate divided by its
 * norm (which is equal to the square of the dot product). In the case of a unit
 * quaternion, this operation will be equivalent the conjugate.
 *
 * Given the inverse of a unit quaternion is the same thing as taking its
 * conjugate, it is much faster to call {@link S_quat_conjugate} than calling
 * this function if only unit quaternions are being used.
 *
 * @param[in,out] quat The quaternion to replace with its inverse.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion is provided
 * to the function.
 * @since 1.0.0
 */
void   S_quat_inverse(Squat *);

/**
 * @brief Perform a linear interpolation on two quaternions.
 *
 * Linearly interpolates from a quaternion @f$q_1@f$ to another @f$q_3@f$ for a
 * given time offset, and stores the result. The result will be normalised.
 *
 * @warning This function expects both quaternions to be normalised. The output
 * of any other quaternions cannot be guaranteed to be correct.
 * @param[in,out] dest The @f$q_2@f$ quaternion to which the interpolation is
 * applied, and the result destination.
 * @param[in] src The @f$q_2@f$ quaternion from which the interpolation is
 * applied.
 * @param[in] time The time offset to apply to the interpolation. This value
 * will be clamped to the range @f$[0,1]@f$.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion is provided
 * to the function.
 * @since 1.0.0
 */
void   S_quat_lerp(Squat *, const Squat *, Sfloat);

/**
 * @brief Perform a spherical linear interpolation on two quaternions.
 *
 * Spherically interpolates from one quaternion to another linearly for a given
 * time offset, and stores the result. The result will be normalised.
 *
 * @warning This function expects both quaternions to be normalised. The output
 * of any other quaternions cannot be guaranteed to be correct.
 * @param[in,out] dest The @f$q_2@f$ quaternion to which the interpolation is
 * applied, and the result destination.
 * @param[in] src The @f$q_1@f$ quaternion from which the interpolation is
 * applied.
 * @param[in] time The time offset to apply to the interpolation. This value
 * will be clamped to the range @f$[0,1]@f$.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion is provided
 * to the function.
 * @since 1.0.0
 */
void   S_quat_slerp(Squat *, const Squat *, Sfloat);

/**
 * @brief Calculate the quaternion angle and Euler angle between two
 * quaternions.
 *
 * Takes two quaternions @f$q_1@f$ and @f$q_2@f$ and calculates the transition
 * quaternion @f$q_t@f$ such that the following shall hold true:
 *
 * @f[
 *     q_tq_1=q_2
 * @f]
 *
 * That is, the multiplication of the transition quaternion with the source
 * quaternion @f$q_1@f$ shall result in the target quaternion @f$q_2@f$.
 *
 * Additionally, the function shall return the angle in degrees between the two
 * given quaternions.
 *
 * @warning This function expects both quaternions to be normalised. The output
 * of any other quaternions cannot be guaranteed to be correct.
 * @param[in,out] dest The @f$q_2@f$ quaternion to which the angle is measured,
 * and the result destination.
 * @param[in] src The @f$q_1@f$ quaternion from which the angle is measured.
 * @return The angle between two quaternions in degrees.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion is provided
 * to the function.
 * @since 1.0.0
 */
Sfloat S_quat_angle(Squat *, const Squat *);

/**
 * @brief Create a copy of a quaternion.
 *
 * Creates a duplicate copy of a given quaternion to a given destination
 * pointer.
 *
 * @param[out] dest The destination quaternion to copy the quaternion to.
 * @param[in] src The source quaternion to copy.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion is provided
 * to the function.
 * @since 1.0.0
 */
void   S_quat_copy(Squat *, const Squat *);

/**
 * @brief Check if two quaternions are equivalent.
 *
 * Two quaternions are equivalent if they have the same orientation. The
 * rule is given by the formula:
 *
 * @f[
 *     \vert q_1\cdot q_2\vert\gt 1-\epsilon
 * @f]
 *
 * That is, if the absolute value of the dot product of the two quaternions is
 * equivalent to @f$1@f$, then they are of the same orientation. @f$\epsilon@f$
 * refers to the @p epsilon parameter.
 *
 * @warning This function expects both quaternions to be normalised. The output
 * of any other quaternions cannot be guaranteed to be correct.
 * @param[in] epsilon Maximum error between each float comparison for two floats
 * to be considered equivalent.
 * @param[in] a The first quaternion to compare.
 * @param[in] b The second quaternion to compare.
 * @return {@link S_TRUE} only if two quaternions have the same orientation.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion is provided
 * to the function.
 * @since 1.0.0
 */
Sbool  S_quat_equals(Sfloat, const Squat *, const Squat *);

/**
 * @brief Convert a quaternion to Euler angles.
 *
 * Turns a quaternion into a 3D vector which contains the @f$XYZ@f$ Euler angles
 * equivalent to a given quaternion. The rotation is achieved by first
 * performing a rotation across the @f$Z@f$ axis of @f$z@f$ degrees, then across
 * the @f$X@f$ axis of @f$x@f$ degrees, and finally across the @f$Y@f$ axis of
 * @f$y@f$ degrees, where @f$x@f$, @f$y@f$ and @f$z@f$ are the values contained
 * in the resulting vector.
 *
 * This function additionally takes into account gimbal-lock at polar-north and
 * polar-south and will attempt to correct the resulting angles accordingly.
 *
 * @warning This function expects the quaternion to be normalised. The output of
 * any other quaternion cannot be guaranteed to be correct.
 * @param[out] dest The destination vector to output the Euler angles.
 * @param[in] src The source quaternion from which the angles shall be
 * extracted.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid quaternion or 3D
 * vector pointer is provided to the function.
 * @see {@link S_vec3_to_quat} To turn Euler angles back into a quaternion.
 * @since 1.0.0
 */
void   S_quat_to_vec3(Svec3 *, const Squat *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_QUAT_H */

