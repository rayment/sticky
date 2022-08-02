/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * qsort.c
 * Quicksort algorithm source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 02/08/2022
 */

#include "sticky/algorithm/qsort.h"

void
S_qsort(void *arr,
        Ssize_t elems,
        Ssize_t size,
        Scomparator_func cmp)
{
	if (arr == NULL || size <= 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_qsort");
		return;
	}
	else if (elems <= 0)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_qsort");
		return;
	}
	_S_qsort_inline_body(arr, elems, size, cmp(a, b));
}

