/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * thread.c
 * Thread test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/02/2022
 */

#include "test_common.h"

#define NUM_THREADS 10

void *
func_adder(void *data)
{
	Sint8 *num;
	num = (Sint8 *) data;
	(*num)++;
	return data;
}

int
main(void)
{
	Sthread threads[NUM_THREADS];
	Sint8 num, *ptr, err, i;

	INIT();

	num = 0;
	err = 0;
	TEST(
		for (i = 0; i < NUM_THREADS; ++i)
		{
			*(threads+i) = S_thread_new(&func_adder, &num);
			if (!*(threads+i))
				++err;
		}
	, err == 0
	, "S_thread_new");

	TEST(
		for (i = 0; i < NUM_THREADS; ++i)
			ptr = (Sint8 *) S_thread_join(*(threads+i));
	, num == NUM_THREADS && ptr == &num
	, "S_thread_join");

	FREE();

	return EXIT_SUCCESS;
}

