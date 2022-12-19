/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * tree.c
 * Self-balancing binary tree test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 16/04/2022
 */

#include "test_common.h"

#define NUM_INTS 50

Scomparator
comparator(const void *a,
           const void *b)
{
	Sint32 *ia, *ib;
	ia = (Sint32 *) a;
	ib = (Sint32 *) b;
	if (*ia < *ib)
		return -1;
	else if (*ia > *ib)
		return 1;
	else
		return 0;
}

void
print_tree(_Stree_node *node,
           int i)
{
	int j;
	if (!node)
		return;
	for (j = i; j > 0; --j)
	{
		if (j == 1)
			printf("|---");
		else
			printf("    ");
	}
	printf("%d(%ld)\n", *((int *) node->ptr), node->bal);
	print_tree(node->left, i+1);
	print_tree(node->right, i+1);
}

int
main(void)
{
	Stree *tree;
	Stree_iter *iter;
	Sint32 numbers[NUM_INTS], i, j;
	Sbool b;
	void *ptr, *ptr2;

	INIT();

	for (i = 0; i < NUM_INTS; ++i)
		*(numbers+i) = i;
	S_random_shuffle_array(numbers, NUM_INTS, sizeof(Sint32));

	TEST(
		tree = S_tree_new(comparator);
	, tree,
	"S_tree_new");

	TEST(
		i = S_tree_size(tree);
	, i == 0,
	"S_tree_size (empty)");

	TEST(
		ptr = S_tree_get_min(tree);
	, !ptr
	, "S_tree_get_min (empty)");

	TEST(
		ptr = S_tree_get_max(tree);
	, !ptr
	, "S_tree_get_max (empty)");

	TEST(
		i = S_tree_size(tree);
	, i == 0
	, "S_tree_size (empty)");

	TEST(
		b = S_tree_search(tree, numbers);
	, !b
	, "S_tree_search (empty)");

	TEST(
		for (i = 0; i < NUM_INTS; ++i)
		{
			if (!(ptr = S_tree_insert(tree, numbers+i)))
				break;
		}
		printf("\n");
		print_tree(tree->root, 0);
	, ptr
	, "S_tree_insert (ordered)");

	TEST(
		b = S_tree_search(tree, numbers);
	, b
	, "S_tree_search");

	TEST(
		i = S_tree_size(tree);
	, i == NUM_INTS
	, "S_tree_size");

	TEST(
		ptr = S_tree_get_min(tree);
	, *((Sint32 *) ptr) == 0
	, "S_tree_get_min");

	TEST(
		ptr = S_tree_get_max(tree);
	, *((Sint32 *) ptr) == NUM_INTS - 1
	, "S_tree_get_max");

	TEST(
		iter = S_tree_iter_begin(tree);
	, iter
	, "S_tree_iter_begin");

	TEST(
	, S_tree_iter_hasnext(iter)
	, "S_tree_iter_hasnext");

	TEST(
		j = 0;
		while (S_tree_iter_hasnext(iter))
		{
			i = *((Sint32 *) S_tree_iter_next(&iter));
			if (j != i)
				j = -1;
			else
				++j;
		}
	, j == NUM_INTS
	, "S_tree_iter_next");

	TEST(
		iter = S_tree_iter_end(tree);
	, iter
	, "S_tree_iter_end");

	TEST(
	, S_tree_iter_hasprev(iter)
	, "S_tree_iter_hasprev");

	TEST(
		j = NUM_INTS - 1;
		while (S_tree_iter_hasprev(iter))
		{
			i = *((Sint32 *) S_tree_iter_prev(&iter));
			if (j != i)
				j = -2;
			else
				--j;
		}
	, j == -1
	, "S_tree_iter_prev");

	TEST(
		for (i = 0; i < NUM_INTS/2; ++i)
		{
			if (!(ptr = S_tree_remove(tree, numbers+i)))
				break;
		}
		printf("\n");
		print_tree(tree->root, 0);
	, ptr
	, "S_tree_remove (ordered)");

	TEST(
		ptr = S_tree_get_max(tree);
		ptr2 = S_tree_get(tree, ptr);
	, ptr == ptr2
	, "S_tree_get"
	);

	TEST(
		ptr = S_tree_get_min(tree);
		b = S_tree_search(tree, ptr);
	, b
	, "S_tree_search");

	TEST(
		i = S_tree_size(tree);
	, i == NUM_INTS / 2
	, "S_tree_size");

	TEST(
		S_tree_clear(tree);
	, S_tree_size(tree) == 0
	, "S_tree_clear");

	TEST(
		ptr = S_tree_get(tree, numbers);
	, !ptr
	, "S_tree_get (empty)");

	TEST(
		ptr = S_tree_get_min(tree);
	, !ptr
	, "S_tree_get_min (empty)");

	TEST(
		ptr = S_tree_get_max(tree);
	, !ptr
	, "S_tree_get_max (empty)");

	TEST(
		i = S_tree_size(tree);
	, i == 0
	, "S_tree_size (empty)");

	TEST(
		b = S_tree_search(tree, numbers);
	, !b
	, "S_tree_search (empty)");

	TEST(
		S_tree_delete(tree);
	, 1
	, "S_tree_delete");

	FREE();

	return EXIT_SUCCESS;
}

