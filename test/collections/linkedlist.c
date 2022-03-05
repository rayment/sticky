/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * linkedlist.c
 * Linked list test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 18/11/2021
 */

#include "test_common.h"

#define NUM_INTS 1024

int
main(void)
{
	Slinkedlist *list;
	Slinkedlist_iter *iter;
	Ssize_t idx;
	Sbool b;
	Sint32 numbers[NUM_INTS], i, j, addon;
	void *ptr;

	INIT();

	addon = 0xCAFEBABE;

	for (i = 0; i < NUM_INTS; ++i)
		*(numbers+i) = i;

	TEST(
		list = S_linkedlist_new();
	, list,
	"S_linkedlist_new");

	TEST(
		i = S_linkedlist_size(list);
	, i == 0,
	"S_linkedlist_size (empty)");

	TEST(
		for (i = 0; i < NUM_INTS; ++i)
		{
			if (!(ptr = S_linkedlist_add_head(list, numbers+i)))
				break;
		}
	, ptr
	, "S_linkedlist_add_head");

	TEST(
		ptr = S_linkedlist_add(list, NULL, NUM_INTS/2);
	, !ptr
	, "S_linkedlist_add");

	TEST(
		b = S_linkedlist_remove_ptr(list, NULL);
	, b
	, "S_linkedlist_remove_ptr");

	TEST(
	, S_linkedlist_size(list) == NUM_INTS
	, "S_linkedlist_size (full)");

	TEST(
		iter = S_linkedlist_iter_begin(list);
	, iter
	, "S_linkedlist_iter_begin");

	TEST(
	, S_linkedlist_iter_hasnext(iter)
	, "S_linkedlist_iter_hasnext");

	TEST(
		j = NUM_INTS - 1;
		while (S_linkedlist_iter_hasnext(iter))
		{
			i = *((Sint32 *) S_linkedlist_iter_next(&iter));
			if (j != i)
				j = -2;
			else
				--j;
		}
	, j == -1
	, "S_linkedlist_iter_next");

	TEST(
	, S_linkedlist_get(list, 2) == numbers+NUM_INTS-3
	, "S_linkedlist_get");

	TEST(
		S_linkedlist_clear(list);
	, 1
	, "S_linkedlist_clear");

	TEST(
	, S_linkedlist_size(list) == 0
	, "S_linkedlist_size (empty)");

	TEST(
		b = S_linkedlist_get(list, 2) == NULL;
		b = b && SERRNO != S_NO_ERROR;
		SERRNO = S_NO_ERROR; /* reset error trip */
	, b
	, "S_linkedlist_get (out of range)");

	TEST(
		for (i = 0; i < NUM_INTS; ++i)
		{
			if (!(ptr = S_linkedlist_add_tail(list, numbers+i)))
				break;
		}
	, ptr
	, "S_linkedlist_add_tail");

	TEST(
	, S_linkedlist_size(list) == NUM_INTS
	, "S_linkedlist_size (full)");

	TEST(
	, S_linkedlist_get(list, 2) == numbers+2
	, "S_linkedlist_get");

	TEST(
	, S_linkedlist_add(list, &addon, 5) != NULL
	, "S_linkedlist_add");

	TEST(
		b = S_linkedlist_search(list, &addon, &idx);
	, b && idx == 5
	, "S_linkedlist_search");

	TEST(
	, S_linkedlist_remove(list, 5) != NULL
	, "S_linkedlist_remove");

	TEST(
		iter = S_linkedlist_iter_end(list);
	, iter
	, "S_linkedlist_iter_end");

	TEST(
	, S_linkedlist_iter_hasprev(iter)
	, "S_linkedlist_iter_hasprev");

	TEST(
		j = NUM_INTS - 1;
		while (S_linkedlist_iter_hasprev(iter))
		{
			i = *((Sint32 *) S_linkedlist_iter_prev(&iter));
			if (j != i)
				j = -2;
			else
				--j;
		}
	, j == -1
	, "S_linkedlist_iter_prev");

	TEST(
	, *((Sint32 *) S_linkedlist_remove_head(list)) == 0
	, "S_linkedlist_remove_head");

	TEST(
	, *((Sint32 *) S_linkedlist_get_head(list)) == 1
	, "S_linkedlist_get_head")

	TEST(
	, *((Sint32 *) S_linkedlist_remove_tail(list)) == 1023
	, "S_linkedlist_remove_tail");

	TEST(
	, *((Sint32 *) S_linkedlist_get_tail(list)) == 1022
	, "S_linkedlist_get_tail")

	TEST(
	, *((Sint32 *) S_linkedlist_remove_head(list)) == 1
	, "S_linkedlist_remove_head");

	TEST(
		S_linkedlist_delete(list);
	, 1
	, "S_linkedlist_delete");

	FREE();

	return EXIT_SUCCESS;
}

