/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * transform.c
 * Object transform test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 16/02/2022
 */

#include "test_common.h"

#define EPSILON S_EPSILON

void
print_tree(Stransform *transform,
           Suint8 level)
{
	Slinkedlist_iter *iter;
	if (level > 0)
	{
		ATOMIC_PRINT("%*c- ", level * 2, ' ');
	}
	ATOMIC_PRINT("%p, parent %p\n",
	             (void *) transform, (void *) transform->parent);
	iter = S_linkedlist_iter_begin(transform->children);
	while (S_linkedlist_iter_hasnext(iter))
		print_tree((Stransform *) S_linkedlist_iter_next(&iter), level + 1);
}

int
main(void)
{
	Stransform *a, *b, *c, *tmp, *tmp2, *tmp3;
	Svec3 vec1, vec2;
	Squat quat1, quat2, quat3;
	Ssize_t i, j, k;

	INIT();

	TEST(
		a = S_transform_new();
		b = S_transform_new();
		c = S_transform_new();
	, a && b && c
	, "S_transform_new");

	TEST(
		tmp = S_transform_get_parent(a);
	, !tmp
	, "S_transform_get_parent");

	TEST(
		i = S_transform_get_number_children(a);
		j = S_transform_get_number_children(a);
	, i == 0 && j == 0
	, "S_transform_get_number_children (none)");

	TEST(
		S_transform_set_parent(a, NULL);
		tmp = S_transform_get_parent(a);
	, !tmp
	, "S_transform_set_parent (null)");

	TEST(
		S_transform_set_parent(a, b);
		tmp = S_transform_get_parent(a);
		tmp2 = S_transform_get_parent(b);
	, tmp == b && tmp2 == NULL
	, "S_transform_set_parent");

	TEST(
		i = S_transform_get_number_children(a);
		j = S_transform_get_number_children(b);
	, i == 0 && j == 1
	, "S_transform_get_number_children (one)");

	TEST(
		S_transform_set_parent(b, a);
		tmp = S_transform_get_parent(a);
		tmp2 = S_transform_get_parent(b);
	, tmp == NULL && tmp2 == a
	, "S_transform_set_parent (swap)");

	TEST(
		i = S_transform_get_number_children(a);
		j = S_transform_get_number_children(b);
	, i == 1 && j == 0
	, "S_transform_get_number_children (swap)");

	TEST(
		S_transform_set_parent(b, NULL);
		tmp = S_transform_get_parent(a);
		tmp2 = S_transform_get_parent(b);
		i = S_transform_get_number_children(a);
		j = S_transform_get_number_children(b);
	, !tmp && !tmp2 && i == 0 && j == 0
	, "S_transform_set_parent (null)");

	TEST(
		S_transform_set_parent(a, b);
		S_transform_set_parent(b, c);
		S_transform_set_parent(c, a);
		tmp = S_transform_get_parent(a);
		tmp2 = S_transform_get_parent(b);
		tmp3 = S_transform_get_parent(c);
		i = S_transform_get_number_children(a);
		j = S_transform_get_number_children(b);
		k = S_transform_get_number_children(c);
	, tmp && !tmp2 && tmp3 && i == 1 && j == 1 && k == 0
	, "S_transform_set_parent (cyclic)");

	TEST(
		ATOMIC_PRINT("\na: ");
		print_tree(a, 0);
		ATOMIC_PRINT("\nb: ");
		print_tree(b, 0);
		ATOMIC_PRINT("\nc: ");
		print_tree(c, 0);
	, S_transform_is_child_in_tree(b, c) &&
	  S_transform_is_child_in_tree(b, a) &&
	  S_transform_is_child_in_tree(a, c) &&
	  !S_transform_is_child_in_tree(c, b) &&
	  !S_transform_is_child_in_tree(c, a) &&
	  !S_transform_is_child_in_tree(a, b)
	, "S_transform_is_child_in_tree");

	TEST(
		S_vec3_zero(&vec1);
		S_transform_get_pos(a, &vec2);
	, S_vec3_equals(EPSILON, &vec1, &vec2)
	, "S_transform_get_pos");

	TEST(
		S_quat_identity(&quat1);
		S_transform_get_rot(a, &quat2);
	, S_quat_equals(EPSILON, &quat1, &quat2)
	, "S_transform_get_rot");

	TEST(
		S_vec3_fill(&vec1, 1.0f);
		S_transform_get_scale(a, &vec2);
	, S_vec3_equals(EPSILON, &vec1, &vec2)
	, "S_transform_get_scale");

	TEST(
		S_vec3_fill(&vec1, S_PI);
		S_transform_set_pos(a, &vec1);
		S_transform_get_pos(a, &vec2);
	, S_vec3_equals(EPSILON, &vec1, &vec2)
	, "S_transform_set_pos");

	TEST(
		S_vec3_fill(&vec1, 90.0f);
		S_vec3_to_quat(&quat1, &vec1);
		S_transform_set_rot(a, &quat1);
		S_transform_get_rot(a, &quat2);
	, S_quat_equals(EPSILON, &quat1, &quat2)
	, "S_transform_set_rot");

	TEST(
		S_vec3_fill(&vec1, S_PI);
		S_transform_set_scale(a, &vec1);
		S_transform_get_scale(a, &vec2);
	, S_vec3_equals(EPSILON, &vec1, &vec2)
	, "S_transform_set_scale");

	TEST(
		S_vec3_fill(&vec1, S_PI);
		S_vec3_fill(&vec2, S_TWOPI);
		S_transform_add_pos(a, &vec1);
		S_transform_get_pos(a, &vec1);
	, S_vec3_equals(EPSILON, &vec1, &vec2)
	, "S_transform_add_pos");

	TEST(
		S_vec3_fill(&vec1, 90.0f);
		S_vec3_to_quat(&quat1, &vec1);
		S_vec3_to_quat(&quat2, &vec1);
		S_quat_multiply(&quat2, &quat2);
		S_transform_get_rot(a, &quat1);
		S_transform_add_rot(a, &quat1);
		S_transform_get_rot(a, &quat1);
		S_quat_to_vec3(&vec1, &quat1);
	, S_quat_equals(EPSILON, &quat1, &quat2)
	, "S_transform_add_rot");

	TEST(
		quat1.r =  0.220066f;
		quat1.i = -0.290087f;
		quat1.j = -0.380114f;
		quat1.k = -0.850255f;
		quat2.r =  0.431513f;
		quat2.i = -0.341196f;
		quat2.j =  0.451583f;
		quat2.k =  0.702463f;
		quat3.r =  0.764910f;
		quat3.i = -0.083317f;
		quat3.j =  0.429233f;
		quat3.k = -0.472999f;
		S_transform_set_rot(a, &quat1);
		S_transform_add_rot(a, &quat2);
		S_transform_get_rot(a, &quat2);
	, S_quat_equals(EPSILON, &quat2, &quat3)
	, "S_transform_add_rot");

	TEST(
		S_vec3_fill(&vec1, S_PI);
		S_vec3_fill(&vec2, S_TWOPI);
		S_transform_add_scale(a, &vec1);
		S_transform_get_scale(a, &vec1);
	, S_vec3_equals(EPSILON, &vec1, &vec2)
	, "S_transform_add_scale");

	TEST(
		S_transform_delete(a);
		S_transform_delete(b);
		S_transform_delete(c);
	, 1
	, "S_transform_delete");

	FREE();

	return EXIT_SUCCESS;
}

