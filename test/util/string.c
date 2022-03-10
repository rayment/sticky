/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * string.c
 * Character string test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 10/03/2022
 */

#include "test_common.h"

#define TEST_STRING "Hello, world!"

int
main(void)
{
	Sstring *a, *b, *c;

	INIT();

	TEST(
		a = S_string_new();
	, a
	, "S_string_new");

	TEST(
		b = S_string_load(TEST_STRING, strlen(TEST_STRING));
	, b
	, "S_string_load");

	TEST(
		c = S_string_load(TEST_STRING, 5);
	, c
	, "S_string_load");

	TEST(
	, S_string_length(a) == 0 && S_string_length(b) == strlen(TEST_STRING)
	, "S_string_length");

	TEST(
	, S_string_equals(NULL, NULL)
	, "S_string_equals (null)");

	TEST(
	, !S_string_equals(NULL, b)
	, "S_string_equals (left null)");

	TEST(
	, !S_string_equals(a, NULL)
	, "S_string_equals (right null)");

	TEST(
	, !S_string_equals(a, b)
	, "S_string_equals");

	TEST(
	, S_string_compare(a, b) == -1
	, "S_string_compare");

	TEST(
		S_string_concat(a, b);
	, S_string_equals(a, b) && S_string_length(a) == S_string_length(b)
	, "S_string_concat");

	TEST(
	, S_string_compare(a, b) == 0
	, "S_string_compare");

	TEST(
		S_string_concat(a, b);
	, !S_string_equals(a, b) && S_string_length(a) == 2 * S_string_length(b)
	, "S_string_concat");

	TEST(
	, S_string_compare(a, b) == 1
	, "S_string_compare");

	TEST(
		S_string_substring(a, a, 0, 5);
	, S_string_equals(a, c)
	, "S_string_substring");

	TEST(
		S_string_set(a, "    ", 4);
		S_string_copy(c, a);
		S_string_concat(c, b);
		S_string_concat(c, a);
		S_string_trim(c);
	, S_string_equals(c, b)
	, "S_string_trim");

	TEST(
		S_string_trim(c);
	, S_string_equals(c, b)
	, "S_string_trim");

	S_string_delete(a);
	S_string_delete(b);
	S_string_delete(c);

	FREE();

	return EXIT_SUCCESS;
}

