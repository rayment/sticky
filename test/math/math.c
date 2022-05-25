/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * math.c
 * Math function test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 14/03/2022
 */

#include "test_common.h"

#define EPSILON S_EPSILON
#define EQ(x,y) S_epsilon(EPSILON, (x), (y))

int
main(void)
{
	Sfloat f;

	INIT();

	TEST(
	, S_epsilon(EPSILON, 0.0001f, 0.0002f-0.0001f)
	, "S_epsilon");

	TEST(
	, S_epsilon(EPSILON, 0.0001f, 0.0003f/3.0f)
	, "S_epsilon");

	TEST(
	, S_epsilon(EPSILON, S_PI, S_TWOPI / 2.0f)
	, "S_epsilon");

	TEST(
	, S_epsilon(EPSILON, S_PI, S_HALFPI * 2.0f)
	, "S_epsilon");

	TEST(
	, S_epsilon(EPSILON, 0.0f, -0.0f)
	, "S_epsilon");

	TEST(
	, S_epsilon(EPSILON, 0.0f, 0.0000001f)
	, "S_epsilon");

	TEST(
	, !S_epsilon(EPSILON, 0.0f, 0.001f)
	, "S_epsilon");

	TEST(
	, !S_epsilon(EPSILON, -0.0f, -0.001f)
	, "S_epsilon");

	TEST(
	, !S_epsilon(EPSILON, 10.0f, -10.0f)
	, "S_epsilon");

	TEST(
	, !S_epsilon(EPSILON, 10.0f, 20.0f)
	, "S_epsilon");

	TEST(
	, S_isnan(S_NAN+10.0f) && S_isnan(S_NAN-10.0f) &&
	  S_isnan(S_NAN*10.0f) && S_isnan(S_NAN/10.0f) &&
	  S_isnan(S_NAN+S_NAN) && S_isnan(S_NAN-S_NAN) &&
	  S_isnan(S_NAN*S_NAN) && S_isnan(S_NAN/S_NAN) &&
	  S_isnan(S_NAN*-S_NAN) && S_isnan(S_NAN) && S_isnan(-S_NAN)
	, "S_isnan");

	TEST(
	, EQ(S_clamp(1.0f, 0.0f, 1.0f), 1.0f)
	, "S_clamp");

	TEST(
	, EQ(S_clamp(-10.0f, 0.0f, 1.0f), 0.0f)
	, "S_clamp");

	TEST(
	, EQ(S_clamp(10.0f, 0.0f, 1.0f), 1.0f)
	, "S_clamp");

	TEST(
	, EQ(S_clamp(0.215f, 0.0f, 1.0f), 0.215f)
	, "S_clamp");

	TEST(
	, EQ(S_clamp(15.6f, 10.0f, 20.0f), 15.6f)
	, "S_clamp");

	TEST(
	, !S_sign(0.0f)
	, "S_sign");

	TEST(
	, !S_sign(1.0f)
	, "S_sign");

	TEST(
	, S_sign(-0.0f)
	, "S_sign");

	TEST(
	, S_sign(-1.0f)
	, "S_sign");

	TEST(
		f = S_copysign(0.0f, -0.0f);
	, EQ(f, -0.0f) && S_sign(f)
	, "S_copysign");

	TEST(
		f = S_copysign(10.0f, -0.0f);
	, EQ(f, -10.0f)
	, "S_copysign");

	TEST(
		f = S_copysign(-0.0f, 0.0f);
	, EQ(f, 0.0f) && !S_sign(f)
	, "S_copysign");

	TEST(
		f = S_copysign(-10.0f, 0.0f);
	, EQ(f, 10.0f)
	, "S_copysign");

	TEST(
		f = S_copysign(-10.0f, 10.0f);
	, EQ(f, 10.0f)
	, "S_copysign");

	TEST(
		f = S_copysign(-10.0f, -10.0f);
	, EQ(f, -10.0f)
	, "S_copysign");

	TEST(
		f = S_copysign(10.0f, 50.0f);
	, EQ(f, 10.0f)
	, "S_copysign");

	TEST(
	, EQ(S_abs(10.0f), 10.0f)
	, "S_abs");

	TEST(
	, !EQ(S_abs(-10.0f), -10.0f)
	, "S_abs");

	TEST(
	, EQ(S_abs(0.0f), 0.0f)
	, "S_abs");

	TEST(
		f = S_abs(-0.0f);
	, EQ(f, 0.0f) && !S_sign(f)
	, "S_abs");

	TEST(
	, EQ(S_min(0.0f, -0.0f), 0.0f)
	, "S_min");

	TEST(
	, EQ(S_min(-5.0f, 5.0001f), -5.0f)
	, "S_min");

	TEST(
	, EQ(S_min(-0.5f, 5.0f), -0.5f)
	, "S_min");

	TEST(
	, EQ(S_min(-1.0f, -1.00001f), -1.00001f)
	, "S_min");

	TEST(
	, EQ(S_min(-2.0f, -1.0f), -2.0f)
	, "S_min");

	TEST(
	, EQ(S_min(5.0f, 5.0001f), 5.0f)
	, "S_min");

	TEST(
	, EQ(S_min(5.0002f, 5.0001f), 5.0001f)
	, "S_min");

	TEST(
	, EQ(S_imin(0, -0), 0)
	, "S_imin");

	TEST(
	, EQ(S_imin(-5, 5), -5)
	, "S_imin");

	TEST(
	, EQ(S_imin(-0, 5), -0)
	, "S_imin");

	TEST(
	, EQ(S_imin(-2, -1), -2)
	, "S_imin");

	TEST(
	, EQ(S_max(0.0f, -0.0f), 0.0f)
	, "S_max");

	TEST(
	, EQ(S_max(-5.0f, 5.0001f), 5.0001f)
	, "S_max");

	TEST(
	, EQ(S_max(-0.5f, 5.0f), 5.0f)
	, "S_max");

	TEST(
	, EQ(S_max(-1.0f, -1.00001f), -1.0f)
	, "S_max");

	TEST(
	, EQ(S_max(-2.0f, -1.0f), -1.0f)
	, "S_max");

	TEST(
	, EQ(S_max(5.0f, 5.0001f), 5.0001f)
	, "S_max");

	TEST(
	, EQ(S_max(5.0002f, 5.0001f), 5.0002f)
	, "S_max");

	TEST(
	, EQ(S_imax(0, -0), 0)
	, "S_imin");

	TEST(
	, EQ(S_imax(-5, 5), 5)
	, "S_imin");

	TEST(
	, EQ(S_imax(-0, 5), 5)
	, "S_imin");

	TEST(
	, EQ(S_imax(-2, -1), -1)
	, "S_imin");

	TEST(
	, EQ(S_sqrt(4.0f), 2.0f)
	, "S_sqrt");

	TEST(
	, EQ(S_sqrt(2.0f), S_SQRT2)
	, "S_sqrt");

	TEST(
	, EQ(S_sqrt(1.0f), 1.0f)
	, "S_sqrt");

	TEST(
	, EQ(S_sqrt(0.0f), 0.0f)
	, "S_sqrt");

	TEST(
		f = S_sqrt(-0.0f);
	, EQ(f, -0.0f) && S_sign(f)
	, "S_sqrt");

	TEST(
	, EQ(S_radians(0.0f), 0.0f)
	, "S_radians");

	TEST(
	, EQ(S_radians(90.0f), S_HALFPI)
	, "S_radians");

	TEST(
	, EQ(S_radians(180.0f), S_PI)
	, "S_radians");

	TEST(
	, EQ(S_radians(360.0f), S_TWOPI)
	, "S_radians");

	TEST(
	, EQ(S_degrees(0.0f), 0.0f)
	, "S_degrees");

	TEST(
	, EQ(S_degrees(S_HALFPI), 90.0f)
	, "S_degrees");

	TEST(
	, EQ(S_degrees(S_PI), 180.0f)
	, "S_degrees");

	TEST(
	, EQ(S_degrees(S_TWOPI), 360.0f)
	, "S_degrees");

	TEST(
	, EQ(S_sin(0.0f), 0.0f)
	, "S_sin");

	TEST(
	, EQ(S_sin(S_HALFPI), 1.0f)
	, "S_sin");

	TEST(
	, EQ(S_sin(S_QUARTERPI), S_HALFSQRT2)
	, "S_sin");

	TEST(
	, EQ(S_sin(S_PI), 0.0f)
	, "S_sin");

	TEST(
	, EQ(S_sin(S_PI + S_HALFPI), -1.0f)
	, "S_sin");

	TEST(
	, EQ(S_sin(S_TWOPI), 0.0f)
	, "S_sin");

	TEST(
	, EQ(S_cos(0.0f), 1.0f)
	, "S_cos");

	TEST(
	, EQ(S_cos(S_HALFPI), 0.0f)
	, "S_cos");

	TEST(
	, EQ(S_cos(S_QUARTERPI), S_HALFSQRT2)
	, "S_cos");

	TEST(
	, EQ(S_cos(S_PI), -1.0f)
	, "S_cos");

	TEST(
	, EQ(S_cos(S_PI + S_HALFPI), 0.0f)
	, "S_cos");

	TEST(
	, EQ(S_cos(S_TWOPI), 1.0f)
	, "S_cos");

	TEST(
	, EQ(S_tan(0.0f), 0.0f)
	, "S_tan");

	TEST(
		f = S_tan(S_HALFPI);
	, S_abs(f) > 999999.0f || S_isnan(f)
	, "S_tan");

	TEST(
	, EQ(S_tan(S_QUARTERPI), 1.0f)
	, "S_tan");

	TEST(
	, EQ(S_tan(S_PI), 0.0f)
	, "S_tan");

	TEST(
		f = S_tan(S_PI + S_HALFPI);
	, S_abs(f) > 999999.0f || S_isnan(f)
	, "S_tan");

	TEST(
	, EQ(S_tan(S_TWOPI), 0.0f)
	, "S_tan");

	TEST(
	, EQ(S_arcsin(0.0f), 0.0f)
	, "S_arcsin");

	TEST(
	, EQ(S_arcsin(-1.0f), -S_HALFPI)
	, "S_arcsin");

	TEST(
	, EQ(S_arcsin(1.0f), S_HALFPI)
	, "S_arcsin");

	TEST(
	, EQ(S_arcsin(0.5f), S_PI/6.0f)
	, "S_arcsin");

	TEST(
	, EQ(S_arcsin(-0.5f), -S_PI/6.0f)
	, "S_arcsin");

	TEST(
	, S_isnan(S_arcsin(2.0f))
	, "S_arcsin");

	TEST(
	, S_isnan(S_arcsin(-2.0f))
	, "S_arcsin");

	TEST(
	, EQ(S_arccos(0.0f), S_HALFPI)
	, "S_arccos");

	TEST(
	, EQ(S_arccos(-1.0f), S_PI)
	, "S_arccos");

	TEST(
	, EQ(S_arccos(1.0f), 0.0f)
	, "S_arccos");

	TEST(
	, EQ(S_arccos(0.5f), S_PI/3.0f)
	, "S_arccos");

	TEST(
	, EQ(S_arccos(-0.5f), S_TWOPI/3.0f)
	, "S_arccos");

	TEST(
	, S_isnan(S_arccos(2.0f))
	, "S_arccos");

	TEST(
	, S_isnan(S_arccos(-2.0f))
	, "S_arccos");

	TEST(
	, EQ(S_arctan(0.0f), 0.0f)
	, "S_arctan");

	TEST(
	, EQ(S_arctan(-1.0f), -S_QUARTERPI)
	, "S_arctan");

	TEST(
	, EQ(S_arctan(1.0f), S_QUARTERPI)
	, "S_arctan");

	TEST(
	, EQ(S_arctan(200000.0f), S_HALFPI)
	, "S_arctan");

	TEST(
	, EQ(S_arctan(-200000.0f), -S_HALFPI)
	, "S_arctan");

	TEST(
	, EQ(S_arctan2(200000.0f, 0.0f), S_HALFPI)
	, "S_arctan2");

	TEST(
	, EQ(S_arctan2(-200000.0f, 0.0f), -S_HALFPI)
	, "S_arctan2");

	TEST(
	, EQ(S_arctan2(-200000.0f, 100.0f), S_arctan(-200000.0f / 100.0f))
	, "S_arctan2");

	TEST(
	, EQ(S_arctan2(200000.0f, -100.0f), S_arctan(200000.0f / -100.0f) + S_PI)
	, "S_arctan2");

	TEST(
	, EQ(S_arctan2(-200000.0f, -100.0f), S_arctan(-200000.0f / -100.0f) - S_PI)
	, "S_arctan2");

	TEST(
	, S_sign(5.0f) == S_FALSE
	, "S_sign");

	TEST(
	, S_sign(-5.0f) == S_TRUE
	, "S_sign");

	TEST(
	, S_sign(0.0f) == S_FALSE
	, "S_sign");

	TEST(
	, S_sign(-0.0f) == S_TRUE
	, "S_sign");

	FREE();

	return EXIT_SUCCESS;
}

