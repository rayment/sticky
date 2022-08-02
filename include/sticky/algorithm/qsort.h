/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * qsort.h
 * Quicksort algorithm header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 02/08/2022
 */

#ifndef FR_RAYMENT_STICKY_QSORT_H
#define FR_RAYMENT_STICKY_QSORT_H 1

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
 * @addtogroup qsort
 * @{
 */

struct
_S_qsort_stack_s
{
	void *lo, *hi;
};

#define _S_QSORT_SWAP(x,y,size)  \
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

#define _S_QSORT_PUSH(low, high) \
	top->lo = (low);             \
	top->hi = (high);            \
	++top

#define _S_QSORT_POP(low, high)  \
	--top;                       \
	low = top->lo;               \
	high = top->hi

#define _S_QSORT_PART_SIZE(low, high, size) (((high)-(low)+(size))/(size))

/**
 * @brief Sort an array in sequential order using the Quicksort algorithm.
 *
 * The contents of the given array are sorted in ascending order according to
 * the output of a cmparison function <c>cmp</c> which takes in arguments two
 * <c>void *</c>, referring to two of any element in the array. The cmparison
 * function must return <c>-1</c> if the first of two elements belongs before
 * the second element in the sequence, <c>1</c> if the second of two elements
 * belongs after the first element in the sequence, or <c>0</c> if the two
 * elements are equivalent, in which case the order of the two elements is
 * undefined in relation to one another because they may or may not be sorted.
 *
 * If an inline comparator is to be used, the function {@link S_qsort_inline}
 * may be used.
 *
 * @param[in,out] arr The array to sort.
 * @param[in] elems The number of elements in the array.
 * @param[in] size The size in bytes of each element in the array.
 * @param[in] cmp The cmparison function to use for ordering the elements.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid array or comparator,
 * or an element size of <c>0</c> is provided to the function.
 * @exception S_INVALID_OPERATION If @p elems is equal to <c>0</c>.
 * @since 1.0.0
 */
STICKY_API void S_qsort(void *, Ssize_t, Ssize_t, Scomparator_func);

/**
 * @brief Sort an array in sequential order using the Quicksort algorithm with
 * an inline comparator.
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
 * ordered sequence.
 *
 * See {@link S_qsort(void *, Ssize_t, Ssize_t, Scomparator_func)} for more
 * information.
 *
 * @param[in,out] arr The array to sort.
 * @param[in] elems The number of elements in the array.
 * @param[in] size The size in bytes of each element in the array.
 * @param[in] cmp The inline cmparison code to be inserted into the sorting
 * algorithm for ordering the elements.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid array or comparator,
 * or an element size of <c>0</c> is provided to the function.
 * @exception S_INVALID_OPERATION If @p elems is equal to <c>0</c>.
 * @since 1.0.0
 */
#define S_qsort_inline(arr, elems, size, cmp)                \
do                                                           \
{                                                            \
	if (arr == NULL || size <= 0)                            \
	{                                                        \
		_S_SET_ERROR(S_INVALID_VALUE, "S_qsort_inline");     \
		break;                                               \
	}                                                        \
	else if (elems <= 0)                                     \
	{                                                        \
		_S_SET_ERROR(S_INVALID_OPERATION, "S_qsort_inline"); \
		break;                                               \
	}                                                        \
	_S_qsort_inline_body(arr, elems, size, cmp);             \
} while (0)

/*
 * TODO: Use insertion sort when PART_SIZE <= 4.
 */
#define _S_qsort_inline_body(arr, elems, size, cmp)                            \
do                                                                             \
{                                                                              \
	struct _S_qsort_stack_s stack[CHAR_BIT * sizeof(Ssize_t)];                 \
	struct _S_qsort_stack_s *top;                                              \
	Schar *carr, *lo, *hi, *a, *b;                                             \
                                                                               \
	if (elems <= 0)                                                            \
		break;                                                                 \
                                                                               \
	carr = (Schar *) arr;                                                      \
	top = stack;                                                               \
	lo = carr;                                                                 \
	hi = carr + (elems-1)*size;                                                \
	_S_QSORT_PUSH(lo, hi); /* OPTIMISE */                                      \
	do                                                                         \
	{                                                                          \
		_S_QSORT_POP(lo, hi); /* OPTIMISE */                                   \
		/* median code */                                                      \
		Schar *mid = lo + size*((hi-lo)/size >> 1); /* prevent int overflow */ \
		a = mid;                                                               \
		b = lo;                                                                \
		if (cmp < 0)                                                           \
			_S_QSORT_SWAP(lo, mid, size);                                      \
		a = hi;                                                                \
		b = lo;                                                                \
		if (cmp < 0)                                                           \
			_S_QSORT_SWAP(lo, hi, size);                                       \
		a = mid;                                                               \
		b = hi;                                                                \
		if (cmp < 0)                                                           \
			_S_QSORT_SWAP(mid, hi, size);                                      \
		Schar *pivot = hi;                                                     \
		/* partition code */                                                   \
		Schar *i = lo-size;                                                    \
		for (Schar *j = lo; j <= hi-size; j += size)                           \
		{                                                                      \
			a = j;                                                             \
			b = pivot;                                                         \
			if (cmp <= 0)                                                      \
			{                                                                  \
				i += size;                                                     \
				_S_QSORT_SWAP(i, j, size);                                     \
			}                                                                  \
		}                                                                      \
		i += size;                                                             \
		/* swap code */                                                        \
		_S_QSORT_SWAP(i, hi, size);                                            \
		Ssize_t ls, rs;                                                        \
		ls = _S_QSORT_PART_SIZE(lo, i-size, size);                             \
		rs = _S_QSORT_PART_SIZE(i+size, hi, size);                             \
		if (ls > 0 && rs > 0)                                                  \
		{                                                                      \
			/* store largest partitions first to avoid overflowing the stack */\
			if (ls > rs)                                                       \
			{                                                                  \
				_S_QSORT_PUSH(lo, i-size);                                     \
				_S_QSORT_PUSH(i+size, hi);                                     \
			}                                                                  \
			else                                                               \
			{                                                                  \
				_S_QSORT_PUSH(i+size, hi);                                     \
				_S_QSORT_PUSH(lo, i-size);                                     \
			}                                                                  \
		}                                                                      \
		else if (ls > 0)                                                       \
		{                                                                      \
			_S_QSORT_PUSH(lo, i-size);                                         \
		}                                                                      \
		else if (rs > 0)                                                       \
		{                                                                      \
			_S_QSORT_PUSH(i+size, hi);                                         \
		}                                                                      \
	} while (top > stack);                                                     \
} while (0)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_QSORT_H */

