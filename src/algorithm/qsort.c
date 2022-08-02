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
	_S_qsort_inline_body(arr, elems, size, cmp(a, b));
}

