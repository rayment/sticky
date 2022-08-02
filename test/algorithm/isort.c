/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * isort.c
 * Insertion sort algorithm test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 02/08/2022
 */

#include "test_common.h"

#define NUM_INTS 4096

void
num_gen(Sint32 *arr)
{
	Sint32 i;
	for (i = 0; i < NUM_INTS; ++i)
		*(arr+i) = S_random_next_int32();
}

Sbool
in_order(Sint32 *arr)
{
	Sint32 i;
	for (i = 1; i < NUM_INTS; ++i)
	{
		if (*(arr+i) < *(arr+i-1))
			return S_FALSE;
	}
	return S_TRUE;
}

Scomparator
comparator(const void *a,
           const void *b)
{
	Sint32 x, y;
	x = *((Sint32 *) a);
	y = *((Sint32 *) b);
	if (x < y)
		return -1;
	else if (x == y)
		return 0;
	else
		return 1;
}

int
main(void)
{
	Sint32 numbers[NUM_INTS];

	INIT();

	num_gen(numbers);
	TEST(
		S_isort(numbers, NUM_INTS, sizeof(Sint32), comparator);
	, in_order(numbers)
	, "S_isort (random)");

	TEST(
		S_isort(numbers, NUM_INTS, sizeof(Sint32), comparator);
	, in_order(numbers)
	, "S_isort (already sorted)");

	num_gen(numbers);
	TEST(
		S_isort_inline(numbers, NUM_INTS, sizeof(Sint32), comparator(a, b));
	, in_order(numbers)
	, "S_isort_inline (random)");

	TEST(
		S_isort_inline(numbers, NUM_INTS, sizeof(Sint32), comparator(a, b));
	, in_order(numbers)
	, "S_isort_inline (already sorted)");

	FREE();

	return EXIT_SUCCESS;
}

