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

#include "sticky/algorithm/isort.h"
#include "sticky/common/defines.h"
#include "sticky/common/error.h"
#include "sticky/common/types.h"

/**
 * @addtogroup sort
 * @{
 */

struct
_S_qsort_stack_s
{
	void *lo, *hi;
};

#define _S_QSORT_THRESH 4

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
	_q_top->lo = (low);             \
	_q_top->hi = (high);            \
	++_q_top

#define _S_QSORT_POP(low, high)  \
	--_q_top;                       \
	low = _q_top->lo;               \
	high = _q_top->hi

#define _S_QSORT_PART_SIZE(low, high, size) (((high)-(low)+(size))/(size))

/**
 * @brief Sort an array in sequential order using the Quicksort algorithm.
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
 * If an inline comparator is to be used, the function {@link S_qsort_inline}
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
 * ordered sequence. It is undefined behaviour if either of the two pointers or
 * their contents are modified.
 *
 * See {@link S_qsort(void *, Ssize_t, Ssize_t, Scomparator_func)} for more
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
#define S_qsort_inline(arr, elems, size, cmp)                  \
do                                                             \
{                                                              \
	Schar *_q_sarr;                                            \
	Ssize_t _q_ssize, _q_selems;                               \
	/* prevent macro expansion more than once */               \
	_q_sarr = (Schar *) arr;                                   \
	_q_ssize = (size);                                         \
	_q_selems = (elems);                                       \
	if (_q_sarr == NULL || _q_ssize <= 0)                      \
	{                                                          \
		_S_SET_ERROR(S_INVALID_VALUE, "S_qsort_inline");       \
		break;                                                 \
	}                                                          \
	else if (_q_selems <= 0)                                   \
	{                                                          \
		_S_SET_ERROR(S_INVALID_OPERATION, "S_qsort_inline");   \
		break;                                                 \
	}                                                          \
	_S_qsort_inline_body(_q_sarr, _q_selems, _q_ssize, (cmp)); \
} while (0)

#define _S_qsort_inline_body(arr, elems, size, cmp)                            \
do                                                                             \
{                                                                              \
	struct _S_qsort_stack_s _q_stack[CHAR_BIT * sizeof(Ssize_t)];              \
	struct _S_qsort_stack_s *_q_top;                                           \
	Schar *_q_carr, *_q_lo, *_q_hi, *a, *b;                                    \
                                                                               \
	_q_carr = (Schar *) arr;                                                   \
                                                                               \
	if (elems <= 1)                                                            \
		break;                                                                 \
                                                                               \
	_q_top = _q_stack;                                                         \
	_q_lo = _q_carr;                                                           \
	_q_hi = _q_carr + (elems-1)*size;                                          \
	_S_QSORT_PUSH(_q_lo, _q_hi);                                               \
	while (_q_top > _q_stack)                                                  \
	{                                                                          \
		_S_QSORT_POP(_q_lo, _q_hi);                                            \
		/* median code */                                                      \
		Schar *mid = _q_lo + size*((_q_hi-_q_lo)/size>> 1);                    \
		a = mid;                                                               \
		b = _q_lo;                                                             \
		if ((cmp) < 0)                                                         \
			_S_QSORT_SWAP(_q_lo, mid, size);                                   \
		a = _q_hi;                                                             \
		if ((cmp) < 0)                                                         \
			_S_QSORT_SWAP(_q_lo, _q_hi, size);                                 \
		b = mid;                                                               \
		if ((cmp) >= 0) /* flipped from < to optimise out an assignment */     \
			_S_QSORT_SWAP(mid, _q_hi, size);                                   \
		Schar *_q_pivot = _q_hi;                                               \
		/* partition code */                                                   \
		Schar *_q_i = _q_lo-size;                                              \
		for (Schar *_q_j = _q_lo; _q_j <= _q_hi-size; _q_j += size)            \
		{                                                                      \
			a = _q_j;                                                          \
			b = _q_pivot;                                                      \
			if (cmp <= 0)                                                      \
			{                                                                  \
				_q_i += size;                                                  \
				_S_QSORT_SWAP(_q_i, _q_j, size);                               \
			}                                                                  \
		}                                                                      \
		_q_i += size;                                                          \
		_S_QSORT_SWAP(_q_i, _q_hi, size);                                      \
		/* push or insertion sort code */                                      \
		Ssize_t ls, rs;                                                        \
		ls = _S_QSORT_PART_SIZE(_q_lo, _q_i-size, size);                       \
		rs = _S_QSORT_PART_SIZE(_q_i+size, _q_hi, size);                       \
		if (ls > _S_QSORT_THRESH && rs > _S_QSORT_THRESH)                      \
		{                                                                      \
			/* store largest partitions first to avoid overflowing the stack */\
			if (ls >= rs)                                                      \
			{                                                                  \
				_S_QSORT_PUSH(_q_lo, _q_i-size);                               \
				_S_QSORT_PUSH(_q_i+size, _q_hi);                               \
			}                                                                  \
			else                                                               \
			{                                                                  \
				_S_QSORT_PUSH(_q_i+size, _q_hi);                               \
				_S_QSORT_PUSH(_q_lo, _q_i-size);                               \
			}                                                                  \
		}                                                                      \
		else if (ls > 0 || rs > 0)                                             \
		{                                                                      \
			if (ls > 0 && ls <= _S_QSORT_THRESH)                               \
			{                                                                  \
				_S_isort_inline_body(_q_lo, ls, size, (cmp));                  \
			}                                                                  \
			else if (ls > 0)                                                   \
			{                                                                  \
				_S_QSORT_PUSH(_q_lo, _q_i-size);                               \
			}                                                                  \
			if (rs > 0 && rs <= _S_QSORT_THRESH)                               \
			{                                                                  \
				_S_isort_inline_body(_q_i+size, rs, size, (cmp));              \
			}                                                                  \
			else if (rs > 0)                                                   \
			{                                                                  \
				_S_QSORT_PUSH(_q_i+size, _q_hi);                               \
			}                                                                  \
		}                                                                      \
	}                                                                          \
} while (0)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_QSORT_H */

