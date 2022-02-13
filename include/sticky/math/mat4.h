/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mat4.h
 * 4x4 matrix header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_MAT4_H
#define FR_RAYMENT_STICKY_MAT4_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/types.h"
#include "sticky/math/mat3.h"

/* extern */
typedef struct Smat3_s Smat3;

/**
 * @file mat4.h
 * @todo Provide graphical examples of matrix functions.
 * @addtogroup mat4
 * @{
 */

/**
 * @brief 4x4 matrix.
 *
 * Defines a 4x4 single-precision floating-point matrix. Matrices can be
 * directly cast into {@link Sfloat Sfloat *} and accessed in column-major
 * order. That is, <c>m[0] ... m[3]</c> refer to the first <b>column</b> of the
 * matrix.
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
 * @brief Set a 4x4 matrix to the identity matrix.
 *
 * The identity matrix @f$I@f$ is defined as:
 *
 * @f[
 *     I=\left[{\begin{array}{cccc}
 *         1 & 0 & 0 & 0 \\
 *         0 & 1 & 0 & 0 \\
 *         0 & 0 & 1 & 0 \\
 *         0 & 0 & 0 & 1
 *     \end{array}}\right]
 * @f]
 *
 * @param[out] mat The matrix to set to the identity matrix.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4x4 matrix is provided
 * to the function.
 * @since 1.0.0
 */
void  S_mat4_identity(Smat4 *);

/**
 * @brief Multiply two 4x4 matrices.
 *
 * Takes two 4x4 matrices @f$A@f$ and @f$B@f$ and performs the matrix
 * multiplication @f$A\cdot B@f$, before putting the result back into the
 * @f$A@f$ matrix pointer.
 *
 * @param[in,out] dest The @f$A@f$ matrix to multiply, and also the pointer
 * where the multiplication result shall be stored.
 * @param[in] src The @f$B@f$ matrix to multiply.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4x4 matrix is provided
 * to the function.
 * @since 1.0.0
 */
void  S_mat4_multiply(Smat4 *, const Smat4 *);

/**
 * @brief Transpose a 4x4 matrix.
 *
 * Transposes a 4x4 matrix and stores the result in the same given pointer. The
 * transpose operation @f$T@f$ on a given matrix is defined as follows:
 *
 * @f[
 *     \left[{\begin{array}{cccc}
 *         a & e & i & m \\
 *         b & f & j & n \\
 *         c & g & k & o \\
 *         d & h & l & p
 *     \end{array}}\right]^T
 *     \rightarrow
 *     \left[{\begin{array}{cccc}
 *         a & b & c & d \\
 *         e & f & g & h \\
 *         i & j & k & l \\
 *         m & n & o & p
 *     \end{array}}\right]
 * @f]
 *
 * @param[in,out] mat The matrix to transpose.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4x4 matrix is provided
 * to the function.
 * @since 1.0.0
 */
void  S_mat4_transpose(Smat4 *);

/**
 * @brief Inverse a 4x4 matrix.
 *
 * Inverses a 4x4 matrix and stores the result in the same given pointer if and
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
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4x4 matrix is provided
 * to the function.
 * @since 1.0.0
 */
Sbool S_mat4_inverse(Smat4 *);

/**
 * @brief Create a copy of a 4x4 matrix.
 *
 * Creates a duplicate copy of a given matrix to a given destination pointer.
 *
 * @param[out] dest The destination matrix to copy the matrix to.
 * @param[in] src The source matrix to copy.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4x4 matrix is provided
 * to the function.
 * @since 1.0.0
 */
void  S_mat4_copy(Smat4 *, const Smat4 *);

/**
 * @brief Check if two 4x4 matrices are equivalent.
 *
 * Each element @f$(i,j)@f$ and @f$(i^\prime,j^\prime)@f$ of two 4x4 matrices
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
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4x4 matrix is provided
 * to the function, or if the given epsilon value is less than 0.
 * @since 1.0.0
 */
Sbool  S_mat4_equals(Sfloat, const Smat4 *, const Smat4 *);

/**
 * @brief Convert a 4x4 matrix to a 3x3 matrix.
 *
 * Converts a 4x4 matrix to a 3x3 matrix by removing the fourth row and column
 * only and stores it in a given destination pointer. The operation is defined
 * as follows:
 *
 * @f[
 *     \left[{\begin{array}{cccc}
 *         a & e & i & m \\
 *         b & f & j & n \\
 *         c & g & k & o \\
 *         d & h & l & p
 *     \end{array}}\right]
 *     \rightarrow
 *     \left[{\begin{array}{ccc}
 *         a & e & i \\
 *         b & f & j \\
 *         c & g & k
 *     \end{array}}\right]
 * @f]
 *
 * @param[out] dest The destination to output the new matrix.
 * @param[in] src The source matrix to truncate.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid 4x4 matrix or 3x3
 * pointer is provided to the function.
 * @since 1.0.0
 */
void  S_mat4_to_mat3(Smat3 *, const Smat4 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MAT4_H */

