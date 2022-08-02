/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * isort.c
 * Insertion sort algorithm source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 02/08/2022
 */

#include "sticky/algorithm/isort.h"
#include "sticky/common/defines.h"
#include "sticky/common/error.h"
#include "sticky/common/types.h"

void
S_isort(void *arr,
        Ssize_t elems,
		Ssize_t size,
		Scomparator_func cmp)
{
	if (arr == NULL || size <= 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_isort");
		return;
	}
	else if (elems <= 0)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_isort");
		return;
	}
	_S_isort_inline_body(arr, elems, size, cmp(a, b));
}

