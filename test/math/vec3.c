/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * vec3.c
 * 3D vector test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/02/2022
 */

#include "test_common.h"

#define EPSILON S_EPSILON

void
print_vec_quat(Svec3 *a,
               Squat *b)
{
	ATOMIC_PRINT("\n%6.2f %6.2f %6.2f -> "
	             "%6.2f %6.2f %6.2f %6.2f\n",
	             a->x, a->y, a->z,
	             b->r, b->i, b->j, b->k);
}

int
main(void)
{
	Svec3 a, b, c, tmp;
	Squat quata, quatb;
	Sfloat sqrt2on2, f;

	INIT();

	sqrt2on2 = S_sqrt(2.0f) / 2.0f;

	TEST(
		S_vec3_zero(&a);
	, a.x == 0.0f && a.y == 0.0f && a.z == 0.0f
	, "S_vec3_zero");

	TEST(
		S_vec3_zero(&a);
	, S_vec3_equals(EPSILON, &a, &a)
	, "S_vec3_equals (0 = 0)");

	TEST(
		S_vec3_zero(&b);
	, S_vec3_equals(EPSILON, &a, &b) && S_vec3_equals(EPSILON, &b, &a)
	, "S_vec3_equals (0 = 0, different var)");

	a.x = 0.0f; a.y = 1.0f; a.z = 2.0f;

	TEST(
		S_vec3_zero(&tmp);
		S_vec3_copy(&tmp, &a);
	, S_vec3_equals(EPSILON, &a, &tmp)
	, "S_vec3_copy (V1 = V2)");

	TEST(
		S_vec3_fill(&a, S_PI);
	, a.x == S_PI && a.y == S_PI && a.z == S_PI
	, "S_vec3_fill");

	TEST(
		S_vec3_set(&a, S_PI, S_HALFPI, 0.5f);
	, a.x == S_PI && a.y == S_HALFPI && a.z == 0.5f
	, "S_vec3_set");

	TEST(
		a.x = 0.0f; a.y = S_HALFPI; a.z = S_PI;
		b.x = S_PI; b.y = S_HALFPI; b.z = 0.0f;
		c.x = S_PI; c.y = S_PI;     c.z = S_PI;
		S_vec3_add(&a, &b);
	, S_vec3_equals(EPSILON, &a, &c)
	, "S_vec3_add");

	TEST(
		a.x = 2.0f; a.y = -1.5f;  a.z = -10.0f;
		b.x = 4.0f; b.y = -4.5f;  b.z = 100.0f;
		c.x = 8.0f; c.y = 6.75f;  c.z = -1000.0f;
		S_vec3_multiply(&a, &b);
	, S_vec3_equals(EPSILON, &a, &c)
	, "S_vec3_multiply");

	TEST(
		a.x = 2.0f; a.y = -1.5f;  a.z = -10.0f;
		b.x = 5.0f; b.y = -3.75f; b.z = -25.0f;
		S_vec3_scale(&a, 2.5f);
	, S_vec3_equals(EPSILON, &a, &b)
	, "S_vec3_scale");

	TEST(
		a.x = 3.0f; a.y = 2.0f;  a.z = -1.0f;
		b.x = 3.0f/S_sqrt(14.0f);
		b.y = S_sqrt(2.0f/7.0f);
		b.z = -1.0f/S_sqrt(14.0f);
		S_vec3_normalize(&a);
	, S_vec3_equals(EPSILON, &a, &b)
	, "S_vec3_normalize");

	TEST(
		a.x = 1.0f; a.y = 2.0f; a.z = 3.0f;
		S_vec3_normalize(&a);
		f = S_vec3_dot(&a, &a);
	, S_epsilon(EPSILON, f, 1.0f)
	, "S_vec3_dot (unit)");

	TEST(
		a.x = 1.0f; a.y = 2.0f; a.z = 3.0f;
		f = S_vec3_dot(&a, &a);
	, S_epsilon(EPSILON, f, 14.0f)
	, "S_vec3_dot (arbitrary)");

	TEST(
		S_vec3_zero(&a);
		f = S_vec3_dot(&a, &a);
	, S_epsilon(EPSILON, f, 0.0f)
	, "S_vec3_dot (0)");

	TEST(
		a.x = 1.0f; a.y = 1.0f; a.z = 14.0f;
		b.x = 4.0f; b.y = 5.0f; b.z = 7.0f;
		f = S_vec3_distance(&a, &b);
	, S_epsilon(EPSILON, f, 8.602326f)
	, "S_vec3_distance");

	TEST(
		a.x = 3.0f;   a.y = -3.0f; a.z = 1.0f;
		b.x = 4.0f;   b.y = 9.0f;  b.z = 2.0f;
		c.x = -15.0f; c.y = -2.0f; c.z = 39.0f;
		S_vec3_cross(&b, &a);
	, S_vec3_equals(EPSILON, &b, &c)
	, "S_vec3_cross");

	TEST(
		a.x = 4.0f;  a.y = 1.0f; a.z = -2.0f;
		b.x = 0.25f; b.y = 1.0f; b.z = -0.5f;
		S_vec3_inverse(&a);
	, S_vec3_equals(EPSILON, &a, &b)
	, "S_vec3_inverse");

	TEST(
		S_vec3_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f; b.z = -0.536f;
		S_vec3_lerp(&b, &a, 0.0f);
	, S_vec3_equals(EPSILON, &a, &b)
	, "S_vec3_lerp (0.0)");

	TEST(
		S_vec3_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f; b.z = -0.536f;
		c.x = sqrt2on2/4.0f; c.y = 0.0575f; c.z = -0.134f;
		S_vec3_copy(&tmp, &b);
		S_vec3_lerp(&b, &a, 0.25f);
	, S_vec3_equals(EPSILON, &b, &c)
	, "S_vec3_lerp (0.25)");

	TEST(
		S_vec3_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f; b.z = -0.536f;
		c.x = sqrt2on2/2.0f; c.y = 0.115f; c.z = -0.268f;
		S_vec3_copy(&tmp, &b);
		S_vec3_lerp(&b, &a, 0.5f);
	, S_vec3_equals(EPSILON, &b, &c)
	, "S_vec3_lerp (0.5)");

	TEST(
		S_vec3_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f; b.z = -0.536f;
		S_vec3_copy(&tmp, &b);
		S_vec3_lerp(&b, &a, 1.0f);
	, S_vec3_equals(EPSILON, &b, &tmp)
	, "S_vec3_lerp (1.0)");

	TEST(
		a.x = 0.0f; a.y = 0.0f; a.z = 90.0f;
		quata.r = sqrt2on2; quata.i = 0.0f; quata.j = 0.0f; quata.k = sqrt2on2;
		S_vec3_to_quat(&quatb, &a);
		print_vec_quat(&a, &quatb);
	, S_quat_equals(EPSILON, &quata, &quatb)
	, "S_vec3_to_quat (gimbal-lock)");

	TEST(
		a.x = 90.0f; a.y = 90.0f; a.z = 90.0f;
		quata.r = sqrt2on2; quata.i = sqrt2on2; quata.j = 0.0f; quata.k = 0.0f;
		S_vec3_to_quat(&quatb, &a);
		print_vec_quat(&a, &quatb);
	, S_quat_equals(EPSILON, &quata, &quatb)
	, "S_vec3_to_quat (gimbal-lock)");

	TEST(
		a.x = 48.93121f; a.y = -60.87643f; a.z = 29.05586f;
		quata.r = sqrt2on2; quata.i = 0.23f; quata.j = -0.536f; quata.k = 0.4f;
		S_vec3_to_quat(&quatb, &a);
		print_vec_quat(&a, &quatb);
	, S_quat_equals(EPSILON, &quata, &quatb)
	, "S_vec3_to_quat (arbitrary)");

	FREE();

	return EXIT_SUCCESS;
}

