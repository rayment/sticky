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
 * @addtogroup mat3
 * @{
 */

/**
 * @brief Set a 3x3 matrix to the identity matrix.
 *
 * The identity matrix @f$I@f$ is defined as:
 *
 * @f[
 *     I=\left[{\begin{array}{ccc}
 *         1 & 0 & 0 \\
 *         0 & 1 & 0 \\
 *         0 & 0 & 1
 *     \end{array}}\right]
 * @f]
 *
 * @param[out] mat The matrix to set to the identity matrix.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3x3 matrix is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void  S_mat3_identity(Smat3 *);

/**
 * @brief Multiply two 3x3 matrices.
 *
 * Takes two 3x3 matrices @f$A@f$ and @f$B@f$ and performs the matrix
 * multiplication @f$A\cdot B@f$, before putting the result back into the
 * @f$A@f$ matrix pointer.
 *
 * @param[in,out] dest The @f$A@f$ matrix to multiply, and also the pointer
 * where the multiplication result shall be stored.
 * @param[in] src The @f$B@f$ matrix to multiply.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3x3 matrix is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void  S_mat3_multiply(Smat3 *, const Smat3 *);

/**
 * @brief Transpose a 3x3 matrix.
 *
 * Transposes a 3x3 matrix and stores the result in the same given pointer. The
 * transpose operation @f$T@f$ on a given matrix is defined as follows:
 *
 * @f[
 *     \left[{\begin{array}{ccc}
 *         a & d & g \\
 *         b & e & h \\
 *         c & f & i
 *     \end{array}}\right]^T
 *     \rightarrow
 *     \left[{\begin{array}{ccc}
 *         a & b & c \\
 *         d & e & f \\
 *         g & h & i
 *     \end{array}}\right]
 * @f]
 *
 * @param[in,out] mat The matrix to transpose.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3x3 matrix is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void  S_mat3_transpose(Smat3 *);

/**
 * @brief Inverse a 3x3 matrix.
 *
 * Inverses a 3x3 matrix and stores the result in the same given pointer if and
 * only if the following equation holds true:
 *
 * @f[
 *     MM^{-1} = M^{-1}M = I
 * @f]
 *
 * That is, for a given matrix @f$M@f$, there exists a matrix @f$M^{-1}@f$ that
 * when multiplied with @f$M@f$, gives the identity matrix @f$I@f$.
 *
 * In the case of a degenerate matrix, that is, one whose determinant
 * @f$\vert M\vert=0@f$, then {@link S_FALSE} is returned.
 *
 * @param[in,out] mat The matrix to inverse.
 * @return {@link S_TRUE} if the inverse succeeds, or else {@link S_FALSE} in
 * the case of a degenerate matrix.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3x3 matrix is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API Sbool S_mat3_inverse(Smat3 *);

/**
 * @brief Create a copy of a 3x3 matrix.
 *
 * Creates a duplicate copy of a given matrix to a given destination pointer.
 *
 * @param[out] dest The destination matrix to copy the matrix to.
 * @param[in] src The source matrix to copy.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3x3 matrix is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void  S_mat3_copy(Smat3 *, const Smat3 *);

/**
 * @brief Check if two 3x3 matrices are equivalent.
 *
 * Each element @f$(i,j)@f$ and @f$(i^\prime,j^\prime)@f$ of two 3x3 matrices
 * are compared to one another, and if any such pair of values are not equal,
 * then {@link S_FALSE} is returned. If all such pairs of values are equal, then
 * {@link S_TRUE} shall be returned.
 *
 * An epsilon value @f$\epsilon@f$ is required for this function due to
 * floating-point imprecision. The exact comparison for each @f$(i,j)@f$ and
 * @f$(i^\prime,j^\prime)@f$ pair is:
 *
 * @f[
 *     \vert(i,j)-(i^\prime,j^\prime)\vert\leq\epsilon,\ \epsilon\geq0
 * @f]
 *
 * @param[in] epsilon Maximum error between each float comparison for two floats
 * to be considered equivalent.
 * @param[in] a The first matrix to compare.
 * @param[in] b The second matrix to compare.
 * @return {@link S_TRUE} only if all respective matrix column and row values
 * are equivalent.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3x3 matrix is provided
 * to the function, or if the given epsilon value is less than 0.
 * @since 1.0.0
 */
STICKY_API Sbool S_mat3_equals(Sfloat, const Smat3 *, const Smat3 *);

/**
 * @brief Convert a 3x3 matrix to a 4x4 matrix.
 *
 * Converts a 3x3 matrix to a 4x4 matrix by adding the fourth row and column as
 * 0 except for the diagonal which is set to 1, and finally stores it in a given
 * destination pointer. The operation is defined as follows:
 *
 * @f[
 *     \left[{\begin{array}{ccc}
 *         a & d & g \\
 *         b & e & h \\
 *         c & f & i
 *     \end{array}}\right]
 *     \rightarrow
 *     \left[{\begin{array}{cccc}
 *         a & d & g & 0 \\
 *         b & e & h & 0 \\
 *         c & f & i & 0 \\
 *         0 & 0 & 0 & 1
 *     \end{array}}\right]
 * @f]
 *
 * @param[out] dest The destination to output the new matrix.
 * @param[in] src The source matrix to truncate.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 3x3 matrix or 4x4
 * pointer is provided to the function.
 * @since 1.0.0
 */
STICKY_API void  S_mat3_to_mat4(Smat4 *, const Smat3 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MAT3_H */

