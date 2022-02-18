/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * vec4.c
 * 4D vector test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/02/2022
 */

#include "test_common.h"

#define EPSILON S_EPSILON

int
main(void)
{
	Svec4 a, b, c, tmp;
	Sfloat sqrt2on2, f;

	INIT();

	sqrt2on2 = S_sqrt(2.0f) / 2.0f;

	TEST(
		S_vec4_zero(&a);
	, a.x == 0.0f && a.y == 0.0f && a.z == 0.0f && a.w == 0.0f
	, "S_vec4_zero");

	TEST(
		S_vec4_zero(&a);
	, S_vec4_equals(EPSILON, &a, &a)
	, "S_vec4_equals (0 = 0)");

	TEST(
		S_vec4_zero(&b);
	, S_vec4_equals(EPSILON, &a, &b) && S_vec4_equals(EPSILON, &b, &a)
	, "S_vec4_equals (0 = 0, different var)");

	a.x = 0.0f; a.y = 1.0f; a.z = 2.0f; a.w = 3.0f;

	TEST(
		S_vec4_zero(&tmp);
		S_vec4_copy(&tmp, &a);
	, S_vec4_equals(EPSILON, &a, &tmp)
	, "S_vec4_copy (V1 = V2)");

	TEST(
		S_vec4_fill(&a, S_PI);
	, a.x == S_PI && a.y == S_PI && a.z == S_PI && a.w == S_PI
	, "S_vec4_fill");

	TEST(
		S_vec4_set(&a, S_PI, S_HALFPI, 0.5f, -1.3f);
	, a.x == S_PI && a.y == S_HALFPI && a.z == 0.5f && a.w == -1.3f
	, "S_vec4_set");

	TEST(
		a.x = 0.0f; a.y = S_HALFPI; a.z = S_PI; a.w = S_TWOPI;
		b.x = S_PI; b.y = S_HALFPI; b.z = 0.0f; b.w = S_TWOPI;
		c.x = S_PI; c.y = S_PI;     c.z = S_PI; c.w = 2.0f*S_TWOPI;
		S_vec4_add(&a, &b);
	, S_vec4_equals(EPSILON, &a, &c)
	, "S_vec4_add");

	TEST(
		a.x = 2.0f; a.y = -1.5f;  a.z = -10.0f;   a.w = S_PI;
		b.x = 4.0f; b.y = -4.5f;  b.z = 100.0f;   b.w = 0.0f;
		c.x = 8.0f; c.y = 6.75f;  c.z = -1000.0f; c.w = 0.0f;
		S_vec4_multiply(&a, &b);
	, S_vec4_equals(EPSILON, &a, &c)
	, "S_vec4_multiply");

	TEST(
		a.x = 2.0f; a.y = -1.5f;  a.z = -10.0f;   a.w = 1.0f;
		b.x = 5.0f; b.y = -3.75f; b.z = -25.0f;   b.w = 2.5f;
		S_vec4_scale(&a, 2.5f);
	, S_vec4_equals(EPSILON, &a, &b)
	, "S_vec4_scale");

	TEST(
		a.x = 3.0f; a.y = 2.0f;  a.z = -1.0f; a.w = 4.0f;
		b.x = S_sqrt(3.0f/10.0f);
		b.y = S_sqrt(2.0f/15.0f);
		b.z = -1.0f/S_sqrt(30.0f);
		b.w = 2.0f*S_sqrt(2.0f/15.0f);
		S_vec4_normalize(&a);
	, S_vec4_equals(EPSILON, &a, &b)
	, "S_vec4_normalize");

	TEST(
		a.x = 1.0f; a.y = 2.0f; a.z = 3.0f; a.w = 4.0f;
		S_vec4_normalize(&a);
		f = S_vec4_dot(&a, &a);
	, S_epsilon(EPSILON, f, 1.0f)
	, "S_vec4_dot (unit)");

	TEST(
		a.x = 1.0f; a.y = 2.0f; a.z = 3.0f; a.w = 4.0f;
		f = S_vec4_dot(&a, &a);
	, S_epsilon(EPSILON, f, 30.0f)
	, "S_vec4_dot (arbitrary)");

	TEST(
		S_vec4_zero(&a);
		f = S_vec4_dot(&a, &a);
	, S_epsilon(EPSILON, f, 0.0f)
	, "S_vec4_dot (0)");

	TEST(
		a.x = 4.0f;  a.y = 1.0f; a.z = -2.0f; a.w = 10.0f;
		b.x = 0.25f; b.y = 1.0f; b.z = -0.5f; b.w = 0.1f;
		S_vec4_inverse(&a);
	, S_vec4_equals(EPSILON, &a, &b)
	, "S_vec4_inverse");

	TEST(
		S_vec4_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f; b.z = -0.536f; b.w = S_PI;
		S_vec4_lerp(&b, &a, 0.0f);
	, S_vec4_equals(EPSILON, &a, &b)
	, "S_vec4_lerp (0.0)");

	TEST(
		S_vec4_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f; b.z = -0.536f; b.w = S_PI;
		c.x = sqrt2on2/4.0f; c.y = 0.0575f; c.z = -0.134f; c.w = S_HALFPI*0.5f;
		S_vec4_copy(&tmp, &b);
		S_vec4_lerp(&b, &a, 0.25f);
	, S_vec4_equals(EPSILON, &b, &c)
	, "S_vec4_lerp (0.25)");

	TEST(
		S_vec4_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f; b.z = -0.536f; b.w = S_PI;
		c.x = sqrt2on2/2.0f; c.y = 0.115f; c.z = -0.268f; c.w = S_HALFPI;
		S_vec4_copy(&tmp, &b);
		S_vec4_lerp(&b, &a, 0.5f);
	, S_vec4_equals(EPSILON, &b, &c)
	, "S_vec4_lerp (0.5)");

	TEST(
		S_vec4_zero(&a);
		b.x = sqrt2on2; b.y = 0.23f; b.z = -0.536f; b.w = S_PI;
		S_vec4_copy(&tmp, &b);
		S_vec4_lerp(&b, &a, 1.0f);
	, S_vec4_equals(EPSILON, &b, &tmp)
	, "S_vec4_lerp (1.0)");

	FREE();

	return EXIT_SUCCESS;
}

