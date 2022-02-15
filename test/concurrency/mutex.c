/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mutex.c
 * Mutex test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/02/2022
 */

#include "test_common.h"

/*
 * Note: The `test_common.h' header makes use of mutexes for atomic printing,
 * which for obvious reasons cannot be used in this test suite, so raw stdio
 * calls shall be used instead.
 */

int
main(void)
{
	Smutex mutex;
	Sbool b;

	S_sticky_init();

	TEST_RAW(
		mutex = S_mutex_new();
	, mutex
	, "S_mutex_new");

	TEST_RAW(
		S_mutex_lock(mutex);
	, 1
	, "S_mutex_lock (unlocked)");

	TEST_RAW(
		b = S_mutex_trylock(mutex);
	, b == S_FALSE
	, "S_mutex_trylock (locked)");

	TEST_RAW(
		S_mutex_unlock(mutex);
	, 1
	, "S_mutex_unlock (locked)");

	TEST_RAW(
		b = S_mutex_trylock(mutex);
	, b == S_TRUE
	, "S_mutex_trylock (unlocked)");

	TEST_RAW(
		S_mutex_unlock(mutex);
	, 1
	, "S_mutex_unlock (locked)");

	TEST_RAW(
		S_mutex_delete(mutex);
	, 1
	, "S_mutex_delete");

	S_sticky_free();

	return EXIT_SUCCESS;
}

