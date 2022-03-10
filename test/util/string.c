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

#define TEST_STRING       "Hello, world!"
#define TEST_STRING_LOWER "hello, world!"
#define TEST_STRING_UPPER "HELLO, WORLD!"

int
main(void)
{
	Sstring *a, *b, *c;
	Schar *ptr;
	Sbool bool1, bool2, bool3;
	Ssize_t idx1, idx2, idx3;

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

	TEST(
		S_string_set(b, TEST_STRING_UPPER, strlen(TEST_STRING_UPPER));
		S_string_upper(c);
	, S_string_equals(c, b)
	, "S_string_upper");

	TEST(
		S_string_set(b, TEST_STRING_LOWER, strlen(TEST_STRING_LOWER));
		S_string_lower(c);
	, S_string_equals(c, b)
	, "S_string_lower");

	TEST(
	, S_string_startswith(c, b)
	, "S_string_startswith (same)");

	TEST(
	, S_string_endswith(c, b)
	, "S_string_endswith (same)");

	TEST(
		S_string_set(b, TEST_STRING, 5);
		S_string_set(c, TEST_STRING, strlen(TEST_STRING));
	, S_string_startswith(c, b)
	, "S_string_startswith");

	TEST(
		ptr = TEST_STRING;
		S_string_set(b, ptr+strlen(TEST_STRING)-5, 5);
		S_string_set(c, TEST_STRING, strlen(TEST_STRING));
	, S_string_endswith(c, b)
	, "S_string_endswith");

	TEST(
		bool1 = S_string_charat(c, strlen(TEST_STRING)-1) == '!';
		bool2 = S_string_charat(c, 0) == 'H';
		bool3 = S_string_charat(c, 1) == 'e';
	, bool1 && bool2 && bool3
	, "S_string_charat");

	TEST(
		bool1 = S_string_indexof(c, 'H', &idx1);
		bool2 = S_string_indexof(c, 'o', &idx2);
		bool3 = S_string_indexof(c, '@', &idx3);
	, bool1 && bool2 && !bool3 && idx1 == 0 && idx2 == 4
	, "S_string_indexof");

	S_string_delete(a);
	S_string_delete(b);
	S_string_delete(c);

	FREE();

	return EXIT_SUCCESS;
}

