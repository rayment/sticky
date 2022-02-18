/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mat4.c
 * 4x4 matrix test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/02/2022
 */

#include "test_common.h"

#define EPSILON S_EPSILON

void
print_2_matrices(Smat4 *a,
                 Smat4 *b)
{
	ATOMIC_PRINT("\n%5.2f %5.2f %5.2f %5.2f    "
	             "%5.2f %5.2f %5.2f %5.2f\n",
	             a->m00, a->m01, a->m02, a->m03,
	             b->m00, b->m01, b->m02, b->m03);
	ATOMIC_PRINT("%5.2f %5.2f %5.2f %5.2f    "
	             "%5.2f %5.2f %5.2f %5.2f\n",
	             a->m10, a->m11, a->m12, a->m13,
	             b->m10, b->m11, b->m12, b->m13);
	ATOMIC_PRINT("%5.2f %5.2f %5.2f %5.2f -> "
	             "%5.2f %5.2f %5.2f %5.2f\n",
	             a->m20, a->m21, a->m22, a->m23,
	             b->m20, b->m21, b->m22, b->m23);
	ATOMIC_PRINT("%5.2f %5.2f %5.2f %5.2f    "
	             "%5.2f %5.2f %5.2f %5.2f\n",
	             a->m30, a->m31, a->m32, a->m33,
	             b->m30, b->m31, b->m32, b->m33);
}

void
print_3_matrices(Smat4 *a,
                 Smat4 *b,
                 Smat4 *c)
{
	ATOMIC_PRINT("\n%5.2f %5.2f %5.2f %5.2f   "
	             "%5.2f %5.2f %5.2f %5.2f   "
	             "%6.2f %6.2f %6.2f %6.2f\n",
	             a->m00, a->m01, a->m02, a->m03,
	             b->m00, b->m01, b->m02, b->m03,
	             c->m00, c->m01, c->m02, c->m03);
	ATOMIC_PRINT("%5.2f %5.2f %5.2f %5.2f   "
	             "%5.2f %5.2f %5.2f %5.2f   "
	             "%6.2f %6.2f %6.2f %6.2f\n",
	             a->m10, a->m11, a->m12, a->m13,
	             b->m10, b->m11, b->m12, b->m13,
	             c->m10, c->m11, c->m12, c->m13);
	ATOMIC_PRINT("%5.2f %5.2f %5.2f %5.2f . "
	             "%5.2f %5.2f %5.2f %5.2f = "
	             "%6.2f %6.2f %6.2f %6.2f\n",
	             a->m20, a->m21, a->m22, a->m23,
	             b->m20, b->m21, b->m22, b->m23,
	             c->m20, c->m21, c->m22, c->m23);
	ATOMIC_PRINT("%5.2f %5.2f %5.2f %5.2f   "
	             "%5.2f %5.2f %5.2f %5.2f   "
	             "%6.2f %6.2f %6.2f %6.2f\n",
	             a->m30, a->m31, a->m32, a->m33,
	             b->m30, b->m31, b->m32, b->m33,
	             c->m30, c->m31, c->m32, c->m33);
}

int
main(void)
{
	Smat4 a, b, c, tmp;
	Smat3 d, e;
	Svec3 vec;
	Squat quat;
	Sbool bb;

	INIT();

	TEST(
		S_mat4_identity(&a);
	, a.m00 == 1.0f && a.m10 == 0.0f && a.m20 == 0.0f && a.m30 == 0.0f &&
	  a.m01 == 0.0f && a.m11 == 1.0f && a.m21 == 0.0f && a.m31 == 0.0f &&
	  a.m02 == 0.0f && a.m12 == 0.0f && a.m22 == 1.0f && a.m32 == 0.0f &&
	  a.m03 == 0.0f && a.m13 == 0.0f && a.m23 == 0.0f && a.m33 == 1.0f
	, "S_mat4_identity");

	TEST(
		S_mat4_identity(&a);
	, S_mat4_equals(EPSILON, &a, &a)
	, "S_mat4_equals (I = I)");

	TEST(
		S_mat4_identity(&b);
	, S_mat4_equals(EPSILON, &a, &b) && S_mat4_equals(EPSILON, &b, &a)
	, "S_mat4_equals (I = I, different var)");

	a.m00 = 0.0f;   a.m10 = 1.0f;   a.m20 = 2.0f;   a.m30 = 3.0f;
	a.m01 = 4.0f;   a.m11 = 5.0f;   a.m21 = 6.0f;   a.m31 = 7.0f;
	a.m02 = 8.0f;   a.m12 = 9.0f;   a.m22 = 10.0f;  a.m32 = 11.0f;
	a.m03 = 12.0f;  a.m13 = 13.0f;  a.m23 = 14.0f;  a.m33 = 15.0f;

	b.m00 = 0.0f;   b.m10 = 4.0f;   b.m20 = 8.0f;   b.m30 = 12.0f;
	b.m01 = 1.0f;   b.m11 = 5.0f;   b.m21 = 9.0f;   b.m31 = 13.0f;
	b.m02 = 2.0f;   b.m12 = 6.0f;   b.m22 = 10.0f;  b.m32 = 14.0f;
	b.m03 = 3.0f;   b.m13 = 7.0f;   b.m23 = 11.0f;  b.m33 = 15.0f;

	TEST(
		S_mat4_identity(&tmp);
		S_mat4_copy(&tmp, &a);
	, S_mat4_equals(EPSILON, &a, &tmp)
	, "S_mat4_copy (M1 = M2)");

	TEST(
		S_mat4_copy(&tmp, &a);
		S_mat4_transpose(&a);
		print_2_matrices(&tmp, &a);
	, S_mat4_equals(EPSILON, &a, &b)
	, "S_mat4_transpose (M1^T = M2)");

	TEST(
		S_mat4_identity(&a);
		S_mat4_identity(&b);
		S_mat4_transpose(&a);
		print_2_matrices(&a, &b);
	, S_mat4_equals(EPSILON, &a, &b)
	, "S_mat4_transpose (I^T = I)");

	a.m00 = 0.0f;   a.m10 = 1.0f;   a.m20 = 2.0f;   a.m30 = 3.0f;
	a.m01 = 4.0f;   a.m11 = 5.0f;   a.m21 = 6.0f;   a.m31 = 7.0f;
	a.m02 = 8.0f;   a.m12 = 9.0f;   a.m22 = 10.0f;  a.m32 = 11.0f;
	a.m03 = 12.0f;  a.m13 = 13.0f;  a.m23 = 14.0f;  a.m33 = 15.0f;

	TEST(
		S_mat4_copy(&tmp, &a);
	, S_mat4_inverse(&tmp) == S_FALSE
	, "S_mat4_inverse (degenerate M1^-1)");

	a.m00 = 16.0f;  a.m10 = 44.0f;  a.m20 = 4.0f;   a.m30 = 8.0f;
	a.m01 = 8.0f;   a.m11 = 3.0f;   a.m21 = 3.0f;   a.m31 = 4.0f;
	a.m02 = 4.0f;   a.m12 = 2.0f;   a.m22 = 2.0f;   a.m32 = 2.0f;
	a.m03 = 1.0f;   a.m13 = 1.0f;   a.m23 = 1.0f;   a.m33 = 1.0f;

	b.m00 = 0.0f;
	b.m01 = 2.0f/80.0f;
	b.m02 = -2.0f/80.0f;
	b.m03 = 0.0f;
	b.m10 = 0.0f;
	b.m11 = -8.0f/80.0f;
	b.m12 = -72.0f/80.0f;
	b.m13 = 1.0f;
	b.m20 = 40.0f/80.0f;
	b.m21 = 8.0f/80.0f;
	b.m22 = 152.0f/80.0f;
	b.m23 = -200.0f/80.0f;
	b.m30 = -1.0f;
	b.m31 = 0.0f;
	b.m32 = 0.0f;
	b.m33 = 2.0f;

	TEST(
		S_mat4_copy(&tmp, &a);
		bb = S_mat4_inverse(&tmp);
		print_2_matrices(&a, &tmp);
	, bb == S_TRUE && S_mat4_equals(EPSILON, &tmp, &b)
	, "S_mat4_inverse (M1^-1 = M2)");

	TEST(
		a.m00 = 1.0f; a.m10 = 0.0f; a.m20 = 0.0f; a.m30 = 0.0f;
		a.m01 = 0.0f; a.m11 = 1.0f; a.m21 = 0.0f; a.m31 = 0.0f;
		a.m02 = 0.0f; a.m12 = 0.0f; a.m22 = 1.0f; a.m32 = 0.0f;
		a.m03 = 5.0f; a.m13 = 3.0f; a.m23 = 2.0f; a.m33 = 1.0f;
		vec.x = 5.0f; vec.y = 3.0f; vec.z = 2.0f;
		S_mat4_translate(&b, &vec);
	, S_mat4_equals(EPSILON, &a, &b)
	, "S_mat4_translate");

	TEST(
		a.m00 =  0.75847f; a.m10 = 0.23395f; a.m20 =  0.60827f; a.m30 = 0.0f;
		a.m01 = -0.03270f; a.m11 = 0.94583f; a.m21 = -0.32300f; a.m31 = 0.0f;
		a.m02 = -0.65089f; a.m12 = 0.22510f; a.m22 =  0.72504f; a.m32 = 0.0f;
		a.m03 =  0.0f;     a.m13 = 0.0f;     a.m23 = 0.0f;      a.m33 = 1.0f;
		quat.r = -0.926f; quat.i = 0.148f; quat.j = 0.340f; quat.k = -0.072f;
		S_quat_normalize(&quat);
		S_mat4_rotate(&b, &quat);
	, S_mat4_equals(EPSILON, &a, &b)
	, "S_mat4_rotate");

	TEST(
		a.m00 = 5.0f; a.m10 = 0.0f; a.m20 = 0.0f; a.m30 = 0.0f;
		a.m01 = 0.0f; a.m11 = 3.0f; a.m21 = 0.0f; a.m31 = 0.0f;
		a.m02 = 0.0f; a.m12 = 0.0f; a.m22 = 2.0f; a.m32 = 0.0f;
		a.m03 = 0.0f; a.m13 = 0.0f; a.m23 = 0.0f; a.m33 = 1.0f;
		vec.x = 5.0f; vec.y = 3.0f; vec.z = 2.0f;
		S_mat4_scale(&b, &vec);
	, S_mat4_equals(EPSILON, &a, &b)
	, "S_mat4_scale");

	a.m00 = 5.0f;   a.m10 = 2.0f;   a.m20 = 8.0f;   a.m30 = 3.0f;
	a.m01 = 7.0f;   a.m11 = 3.0f;   a.m21 = 10.0f;  a.m31 = 3.0f;
	a.m02 = 9.0f;   a.m12 = 3.0f;   a.m22 = 2.0f;   a.m32 = 4.0f;
	a.m03 = 10.0f;  a.m13 = 8.0f;   a.m23 = 3.0f;   a.m33 = 8.0f;

	b.m00 = 3.0f;   b.m10 = 12.0f;  b.m20 = 9.0f;   b.m30 = 3.0f;
	b.m01 = 10.0f;  b.m11 = 1.0f;   b.m21 = 10.0f;  b.m31 = 12.0f;
	b.m02 = 12.0f;  b.m12 = 4.0f;   b.m22 = 12.0f;  b.m32 = 4.0f;
	b.m03 = 18.0f;  b.m13 = 9.0f;   b.m23 = 2.0f;   b.m33 = 10.0f;

	c.m00 = 210.0f; c.m10 = 93.0f;  c.m20 = 171.0f; c.m30 = 105.0f;
	c.m01 = 267.0f; c.m11 = 149.0f; c.m21 = 146.0f; c.m31 = 169.0f;
	c.m02 = 236.0f; c.m12 = 104.0f; c.m22 = 172.0f; c.m32 = 128.0f;
	c.m03 = 271.0f; c.m13 = 149.0f; c.m23 = 268.0f; c.m33 = 169.0f;

	TEST(
		S_mat4_copy(&tmp, &a);
		S_mat4_multiply(&tmp, &b);
		print_3_matrices(&a, &b, &tmp);
	, S_mat4_equals(EPSILON, &tmp, &c) && S_mat4_equals(EPSILON, &c, &tmp)
	, "S_mat4_multiply (M1 x M2 = M3)");

	e.m00 = 210.0f; e.m10 = 93.0f;  e.m20 = 171.0f;
	e.m01 = 267.0f; e.m11 = 149.0f; e.m21 = 146.0f;
	e.m02 = 236.0f; e.m12 = 104.0f; e.m22 = 172.0f;

	TEST(
		S_mat4_to_mat3(&d, &c);
	, S_mat3_equals(EPSILON, &d, &e) && S_mat3_equals(EPSILON, &e, &d)
	, "S_mat4_to_mat3");

	FREE();

	return EXIT_SUCCESS;
}

