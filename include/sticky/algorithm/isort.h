/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * isort.h
 * Insertion sort algorithm header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 02/08/2022
 */

#ifndef FR_RAYMENT_STICKY_ISORT_H
#define FR_RAYMENT_STICKY_ISORT_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <limits.h>
#include <stdlib.h>

#include "sticky/common/defines.h"
#include "sticky/common/error.h"
#include "sticky/common/types.h"

/**
 * @addtogroup sort
 * @{
 */

#define _S_ISORT_SWAP(x,y,size)  \
do                               \
{                                \
	Ssize_t rem = size;          \
	Schar *tmpx = x;             \
	Schar *tmpy = y;             \
	Schar tmpc;                  \
	do                           \
	{                            \
		tmpc = *tmpx;            \
		*tmpx++ = *tmpy;         \
		*tmpy++ = tmpc;          \
	} while (--rem > 0);         \
} while (0)

/**
 * @brief Sort an array in sequential order using the insertion sort algorithm.
 *
 * The contents of the given array are sorted in ascending order according to
 * the output of a comparison function <c>cmp</c> which takes in arguments two
 * <c>void *</c>, referring to two of any element in the array. The comparison
 * function must return <c>-1</c> if the first of two elements belongs before
 * the second element in the sequence, <c>1</c> if the second of two elements
 * belongs after the first element in the sequence, or <c>0</c> if the two
 * elements are equivalent, in which case the order of the two elements is
 * undefined in relation to one another because they may or may not be sorted.
 *
 * If an inline comparator is to be used, the function {@link S_isort_inline}
 * may be used.
 *
 * @param[in,out] arr The array to sort.
 * @param[in] elems The number of elements in the array.
 * @param[in] size The size in bytes of each element in the array.
 * @param[in] cmp The comparison function to use for ordering the elements.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid array or comparator,
 * or an element size of <c>0</c> is provided to the function.
 * @exception S_INVALID_OPERATION If @p elems is equal to <c>0</c>.
 * @since 1.0.0
 */
STICKY_API void S_isort(void *, Ssize_t, Ssize_t, Scomparator_func);

/**
 * @brief Sort an array in sequential order using the insertion sort algorithm
 * with an inline comparator.
 * @hideinitializer
 *
 * This macro will sort an array sequentially using a comparator that is
 * provided in place of the last argument @p cmp. Note that the entire sorting
 * function will be rewritten in place of this macro, so it is advisable to not
 * call the function multiple times in a row outside of a loop, or else risk
 * very large object files.
 *
 * The inline comparator must be an expression that results in either <c>-1</c>,
 * <c>0</c> or <c>1</c> by making use of the two <c>void *</c> arguments
 * <c>a</c> and <c>b</c> where <c>a</c> must come before <c>b</c> in the
 * ordered sequence. It is undefined behaviour if either of the two pointers or
 * their contents are modified.
 *
 * See {@link S_isort(void *, Ssize_t, Ssize_t, Scomparator_func)} for more
 * information.
 *
 * @param[in,out] arr The array to sort.
 * @param[in] elems The number of elements in the array.
 * @param[in] size The size in bytes of each element in the array.
 * @param[in] cmp The inline comparison code to be inserted into the sorting
 * algorithm for ordering the elements.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid array or comparator,
 * or an element size of <c>0</c> is provided to the function.
 * @exception S_INVALID_OPERATION If @p elems is equal to <c>0</c>.
 * @since 1.0.0
 */
#define S_isort_inline(arr, elems, size, cmp)                \
do                                                           \
{                                                            \
	if (arr == NULL || size <= 0)                            \
	{                                                        \
		_S_SET_ERROR(S_INVALID_VALUE, "S_isort_inline");     \
		break;                                               \
	}                                                        \
	else if (elems <= 0)                                     \
	{                                                        \
		_S_SET_ERROR(S_INVALID_OPERATION, "S_isort_inline"); \
		break;                                               \
	}                                                        \
	_S_isort_inline_body(arr, elems, size, cmp);             \
} while (0)

#define _S_isort_inline_body(arr, elems, size, cmp)                            \
do                                                                             \
{                                                                              \
	Ssize_t i, j; \
	Schar *a, *b, *carr;  \
	i = 1; \
	carr = (Schar *) arr; \
	while (i < elems) \
	{ \
		j = i; \
		a = carr + (j-1)*size; \
		b = carr + j*size; \
		while (j > 0 && cmp > 0) \
		{ \
			_S_ISORT_SWAP(a, b, size); \
			--j; \
			a = carr + (j-1)*size; \
			b = carr + j*size; \
		} \
		++i; \
	} \
} while (0)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_ISORT_H */

