/*
 * mat3.c
 * 3x3 matrix test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/02/2022
 */

#include "test_common.h"

#define EPSILON S_EPSILON

void
print_2_matrices(Smat3 *a,
                 Smat3 *b)
{
	ATOMIC_PRINT("\n%5.2f %5.2f %5.2f    "
	             "%5.2f %5.2f %5.2f\n",
	             a->m00, a->m01, a->m02,
	             b->m00, b->m01, b->m02);
	ATOMIC_PRINT("%5.2f %5.2f %5.2f -> "
	             "%5.2f %5.2f %5.2f\n",
	             a->m10, a->m11, a->m12,
	             b->m10, b->m11, b->m12);
	ATOMIC_PRINT("%5.2f %5.2f %5.2f    "
	             "%5.2f %5.2f %5.2f\n",
	             a->m20, a->m21, a->m22,
	             b->m20, b->m21, b->m22);
}

void
print_3_matrices(Smat3 *a,
                 Smat3 *b,
                 Smat3 *c)
{
	ATOMIC_PRINT("\n%5.2f %5.2f %5.2f   "
	             "%5.2f %5.2f %5.2f   "
	             "%6.2f %6.2f %6.2f\n",
	             a->m00, a->m01, a->m02,
	             b->m00, b->m01, b->m02,
	             c->m00, c->m01, c->m02);
	ATOMIC_PRINT("%5.2f %5.2f %5.2f . "
	             "%5.2f %5.2f %5.2f = "
	             "%6.2f %6.2f %6.2f\n",
	             a->m10, a->m11, a->m12,
	             b->m10, b->m11, b->m12,
	             c->m10, c->m11, c->m12);
	ATOMIC_PRINT("%5.2f %5.2f %5.2f   "
	             "%5.2f %5.2f %5.2f   "
	             "%6.2f %6.2f %6.2f\n",
	             a->m20, a->m21, a->m22,
	             b->m20, b->m21, b->m22,
	             c->m20, c->m21, c->m22);
}

int
main(void)
{
	Smat3 a, b, c, tmp;
	Smat4 d, e;
	Sbool bb;

	INIT();

	TEST(
		S_mat3_identity(&a);
	, a.m00 == 1.0f && a.m10 == 0.0f && a.m20 == 0.0f &&
	  a.m01 == 0.0f && a.m11 == 1.0f && a.m21 == 0.0f &&
	  a.m02 == 0.0f && a.m12 == 0.0f && a.m22 == 1.0f
	, "S_mat3_identity");

	TEST(
		S_mat3_identity(&a);
	, S_mat3_equals(EPSILON, &a, &a)
	, "S_mat3_equals (I = I)");

	TEST(
		S_mat3_identity(&b);
	, S_mat3_equals(EPSILON, &a, &b) && S_mat3_equals(EPSILON, &b, &a)
	, "S_mat3_equals (I = I, different var)");

	a.m00 = 0.0f;   a.m10 = 1.0f;   a.m20 = 2.0f;
	a.m01 = 3.0f;   a.m11 = 4.0f;   a.m21 = 5.0f;
	a.m02 = 6.0f;   a.m12 = 7.0f;   a.m22 = 8.0f;

	b.m00 = 0.0f;   b.m10 = 3.0f;   b.m20 = 6.0f;
	b.m01 = 1.0f;   b.m11 = 4.0f;   b.m21 = 7.0f;
	b.m02 = 2.0f;   b.m12 = 5.0f;   b.m22 = 8.0f;

	TEST(
		S_mat3_identity(&tmp);
		S_mat3_copy(&tmp, &a);
	, S_mat3_equals(EPSILON, &a, &tmp)
	, "S_mat3_copy (M1 = M2)");

	TEST(
		S_mat3_copy(&tmp, &a);
		S_mat3_transpose(&a);
		print_2_matrices(&tmp, &a);
	, S_mat3_equals(EPSILON, &a, &b)
	, "S_mat3_transpose (M1^T = M2)");

	TEST(
		S_mat3_identity(&a);
		S_mat3_identity(&b);
		S_mat3_transpose(&a);
		print_2_matrices(&a, &b);
	, S_mat3_equals(EPSILON, &a, &b)
	, "S_mat3_transpose (I^T = I)");

	a.m00 = 0.0f;   a.m10 = 1.0f;   a.m20 = 2.0f;
	a.m01 = 3.0f;   a.m11 = 4.0f;   a.m21 = 5.0f;
	a.m02 = 6.0f;   a.m12 = 7.0f;   a.m22 = 8.0f;

	TEST(
		S_mat3_copy(&tmp, &a);
	, S_mat3_inverse(&tmp) == S_FALSE
	, "S_mat3_inverse (degenerate M1^-1)");

	a.m00 = 16.0f;  a.m10 = 44.0f;  a.m20 = 4.0f;
	a.m01 = 8.0f;   a.m11 = 3.0f;   a.m21 = 3.0f;
	a.m02 = 4.0f;   a.m12 = 2.0f;   a.m22 = 2.0f;

	b.m00 = 0.0f;
	b.m01 = -80.0f/-160.0f;
	b.m02 = 120.0f/-160.0f;
	b.m10 = -4.0f/-160.0f;
	b.m11 = 16.0f/-160.0f;
	b.m12 = -16.0f/-160.0f;
	b.m20 = 4.0f/-160.0f;
	b.m21 = 144.0f/-160.0f;
	b.m22 = -304.0f/-160.0f;

	TEST(
		S_mat3_copy(&tmp, &a);
		bb = S_mat3_inverse(&tmp);
		print_2_matrices(&a, &tmp);
	, bb == S_TRUE && S_mat3_equals(EPSILON, &tmp, &b)
	, "S_mat3_inverse (M1^-1 = M2)");

	a.m00 = 10.0f;  a.m10 = 4.0f;   a.m20 = 2.0f;
	a.m01 = 20.0f;  a.m11 = 5.0f;   a.m21 = 3.0f;
	a.m02 = 10.0f;  a.m12 = 6.0f;   a.m22 = 5.0f;

	b.m00 = 3.0f;   b.m10 = 3.0f;   b.m20 = 4.0f;
	b.m01 = 2.0f;   b.m11 = 3.0f;   b.m21 = 4.0f;
	b.m02 = 4.0f;   b.m12 = 9.0f;   b.m22 = 2.0f;

	c.m00 = 130.0f; c.m10 = 51.0f;  c.m20 = 35.0f;
	c.m01 = 120.0f; c.m11 = 47.0f;  c.m21 = 33.0f;
	c.m02 = 240.0f; c.m12 = 73.0f;  c.m22 = 45.0f;

	TEST(
		S_mat3_copy(&tmp, &a);
		S_mat3_multiply(&tmp, &b);
		print_3_matrices(&a, &b, &tmp);
	, S_mat3_equals(EPSILON, &tmp, &c) && S_mat3_equals(EPSILON, &c, &tmp)
	, "S_mat3_multiply (M1 x M2 = M3)");

	e.m00 = 130.0f; e.m10 = 51.0f;  e.m20 = 35.0f;  e.m30 = 0.0f;
	e.m01 = 120.0f; e.m11 = 47.0f;  e.m21 = 33.0f;  e.m31 = 0.0f;
	e.m02 = 240.0f; e.m12 = 73.0f;  e.m22 = 45.0f;  e.m32 = 0.0f;
	e.m03 = 0.0f;   e.m13 = 0.0f;   e.m23 = 0.0f;   e.m33 = 1.0f;

	TEST(
		S_mat3_to_mat4(&d, &c);
	, S_mat4_equals(EPSILON, &d, &e) && S_mat4_equals(EPSILON, &e, &d)
	, "S_mat3_to_mat4");

	FREE();

	return EXIT_SUCCESS;
}

