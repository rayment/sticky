/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * vec2.c
 * 3D vector test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/02/2022
 */

#include "test_common.h"

#define EPSILON S_EPSILON

int
main(void)
{
	Svec2 a, b, c, tmp;
	Sfloat sqrt2on2, f;

	INIT();

	sqrt2on2 = S_sqrt(2.0f) / 2.0f;

	TEST(
		S_vec2_zero(&a);
	, a.x == 0.0f && a.y == 0.0f
	, "S_vec2_zero");

	TEST(
		S_vec2_zero(&a);
	, S_vec2_equals(EPSILON, &a, &a)
	, "S_vec2_equals (0 = 0)");

	TEST(
		S_vec2_zero(&b);
	, S_vec2_equals(EPSILON, &a, &b) && S_vec2_equals(EPSILON, &b, &a)
	, "S_vec2_equals (0 = 0, different var)");

	a.x = 0.0f; a.y = 1.0f;

	TEST(
		S_vec2_zero(&tmp);
		S_vec2_copy(&tmp, &a);
	, S_vec2_equals(EPSILON, &a, &tmp)
	, "S_vec2_copy (V1 = V2)");

	TEST(
		S_vec2_fill(&a, S_PI);
	, a.x == S_PI && a.y == S_PI
	, "S_vec2_fill");

	TEST(
		a.x = 0.0f; a.y = S_HALFPI;
		b.x = S_PI; b.y = S_HALFPI;
		c.x = S_PI; c.y = S_PI;
		S_vec2_add(&a, &b);
	, S_vec2_equals(EPSILON, &a, &c)
	, "S_vec2_add");

	TEST(
		a.x = 2.0f; a.y = -1.5f;
		b.x = 4.0f; b.y = -4.5f;
		c.x = 8.0f; c.y = 6.75f;
		S_vec2_multiply(&a, &b);
	, S_vec2_equals(EPSILON, &a, &c)
	, "S_vec2_multiply");

	TEST(
		a.x = 2.0f; a.y = -1.5f;
		b.x = 5.0f; b.y = -3.75f;
		S_vec2_scale(&a, 2.5f);
	, S_vec2_equals(EPSILON, &a, &b)
	, "S_vec2_scale");

	TEST(
		a.x = 3.0f; a.y = 2.0f;
		b.x = 3.0f/S_sqrt(13.0f);
		b.y = 2.0f/S_sqrt(13.0f);
		S_vec2_normalize(&a);
	, S_vec2_equals(EPSILON, &a, &b)
	, "S_vec2_normalize");

	TEST(
		a.x = 1.0f; a.y = 2.0f;
		S_vec2_normalize(&a);
		f = S_vec2_dot(&a, &a);
	, S_epsilon(EPSILON, f, 1.0f)
	, "S_vec2_dot (unit)");

	TEST(
		a.x = 1.0f; a.y = 2.0f;
		f = S_vec2_dot(&a, &a);
	, S_epsilon(EPSILON, f, 5.0f)
	, "S_vec2_dot (arbitrary)");

	TEST(
		S_vec2_zero(&a);
		f = S_vec2_dot(&a, &a);
	, S_epsilon(EPSILON, f, 0.0f)
	, "S_vec2_dot (0)");

	TEST(
		a.x = 4.0f;  a.y = 1.0f;
		b.x = 0.25f; b.y = 1.0f;
		S_vec2_inverse(&a);
	, S_vec2_equals(EPSILON, &a, &b)
	, "S_vec2_inverse");

	TEST(
		S_vec2_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f;
		S_vec2_lerp(&b, &a, 0.0f);
	, S_vec2_equals(EPSILON, &a, &b)
	, "S_vec2_lerp (0.0)");

	TEST(
		S_vec2_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f;
		c.x = sqrt2on2/4.0f; c.y = 0.0575f;
		S_vec2_copy(&tmp, &b);
		S_vec2_lerp(&b, &a, 0.25f);
	, S_vec2_equals(EPSILON, &b, &c)
	, "S_vec2_lerp (0.25)");

	TEST(
		S_vec2_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f;
		c.x = sqrt2on2/2.0f; c.y = 0.115f;
		S_vec2_copy(&tmp, &b);
		S_vec2_lerp(&b, &a, 0.5f);
	, S_vec2_equals(EPSILON, &b, &c)
	, "S_vec2_lerp (0.5)");

	TEST(
		S_vec2_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f;
		S_vec2_copy(&tmp, &b);
		S_vec2_lerp(&b, &a, 1.0f);
	, S_vec2_equals(EPSILON, &b, &tmp)
	, "S_vec2_lerp (1.0)");

	FREE();

	return EXIT_SUCCESS;
}

