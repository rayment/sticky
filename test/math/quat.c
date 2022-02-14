/*
 * quat.c
 * Quaternion test suite.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 14/02/2022
 */

#include "test_common.h"

#define EPSILON S_EPSILON
#define EULER_EPSILON 0.1f

void
print_2_quats(Squat *a,
              Squat *b)
{
	ATOMIC_PRINT("\n%5.2f %5.2f %5.2f %5.2f -> %5.2f %5.2f %5.2f %5.2f\n",
	             a->r, a->i, a->j, a->k, b->r, b->i, b->j, b->k);
}

void
print_quat_vec(Squat *a,
               Svec3 *b)
{
	ATOMIC_PRINT("\n%5.2f %5.2f %5.2f %5.2f -> %5.2f %5.2f %5.2f\n",
	             a->r, a->i, a->j, a->k, b->x, b->y, b->z);
}

void
print_3_quats(Squat *a,
              Squat *b,
              Squat *c)
{
	ATOMIC_PRINT("\n%5.2f %5.2f %5.2f %5.2f . "
	             "%5.2f %5.2f %5.2f %5.2f = "
	             "%5.2f %5.2f %5.2f %5.2f\n",
	             a->r, a->i, a->j, a->k,
	             b->r, b->i, b->j, b->k,
	             c->r, c->i, c->j, c->k);
}

int
main(void)
{
	Squat a, b, c, tmp;
	Svec3 veca, vecb;
	Sfloat sqrt2on2, angle;

	INIT();

	sqrt2on2 = S_sqrt(2.0f) / 2.0f;

	TEST(
		S_quat_identity(&a);
	, a.r == 1.0f && a.i == 0.0f && a.j == 0.0f && a.k == 0.0f
	, "S_quat_identity");

	TEST(
		S_quat_identity(&a);
	, S_quat_equals(EPSILON, &a, &a)
	, "S_quat_equals (I = I)");

	TEST(
		S_quat_identity(&b);
	, S_quat_equals(EPSILON, &a, &b) && S_quat_equals(EPSILON, &b, &a)
	, "S_quat_equals (I = I, different var)");

	a.r = sqrt2on2; a.i = 0.0f; a.j = 0.0f; a.k = sqrt2on2;

	TEST(
		S_quat_identity(&tmp);
		S_quat_copy(&tmp, &a);
	, S_quat_equals(EPSILON, &a, &tmp)
	, "S_quat_copy (Q1 = Q2)");

	TEST(
		S_quat_copy(&tmp, &a);
		S_quat_conjugate(&a);
		print_2_quats(&tmp, &a);
	, a.r == sqrt2on2 && a.i == 0.0f && a.j == 0.0f && a.k == -sqrt2on2
	, "S_quat_conjugate");

	a.r = 0.0f; a.i = 1.0f; a.j = 2.0f; a.k = 3.0f;

	b.r = 0.0f;
	b.i = 1.0f / S_sqrt(14.0f);
	b.j = S_sqrt(2.0f / 7.0f);
	b.k = 3.0f / S_sqrt(14.0f);

	TEST(
		S_quat_copy(&tmp, &a);
		S_quat_normalize(&a);
		print_2_quats(&tmp, &a);
	, S_quat_equals(EPSILON, &a, &b) && S_quat_equals(EPSILON, &b, &a)
	, "S_quat_normalize");

	TEST(
		S_quat_normalize(&a);
	, S_quat_dot(&a, &a) > (1.0f - EPSILON)
	, "S_quat_dot (correct Q.Q)");

	TEST(
		S_quat_normalize(&a);
		S_quat_identity(&b);
	, S_quat_dot(&a, &b) < (1.0f - EPSILON)
	, "S_quat_dot (wrong Q.I)");

	TEST(
		a.r = sqrt2on2; a.i = 0.0f; a.j = 0.0f; a.k = sqrt2on2;
		b.r = sqrt2on2; b.i = sqrt2on2; b.j = 0.0f; b.k = 0.0f;
	, S_quat_dot(&a, &b) < (1.0f - EPSILON)
	, "S_quat_dot (wrong Q1.Q2)");

	TEST(
		S_quat_identity(&a);
		S_quat_identity(&b);
		S_quat_inverse(&b);
	, S_quat_equals(EPSILON, &a, &b)
	, "S_quat_inverse (I)");

	TEST(
		a.r = sqrt2on2; a.i = 0.0f; a.j = 0.0f; a.k = sqrt2on2;
		b.r = sqrt2on2; b.i = 0.0f; b.j = 0.0f; b.k = -sqrt2on2;
		S_quat_copy(&tmp, &a);
		S_quat_normalize(&a);
		S_quat_inverse(&a);
		print_2_quats(&tmp, &a);
	, S_quat_equals(EPSILON, &a, &b)
	, "S_quat_inverse (Q)");

	TEST(
		a.r = sqrt2on2; a.i = sqrt2on2; a.j = 0.0f; a.k = 0.0f;
		b.r = 0.5f;     b.i = 0.5f;     b.j = 0.5f; b.k = 0.5f;
		c.r = sqrt2on2; c.i = 0.0f;     c.j = 0.0f; c.k = sqrt2on2;
		S_quat_copy(&tmp, &b);
		/* angle from a to b is c */
		angle = S_quat_angle(&b, &a);
		print_3_quats(&a, &tmp, &b);
		ATOMIC_PRINT("Angle: %f degrees.\n", angle);
	, S_epsilon(EPSILON, angle, 90.0f) && S_quat_equals(EPSILON, &b, &c)
	, "S_quat_angle");

	TEST(
		a.r = sqrt2on2; a.i = sqrt2on2; a.j = 0.0f; a.k = 0.0f;
		b.r = 0.5f;     b.i = 0.5f;     b.j = 0.5f; b.k = 0.5f;
		c.r = sqrt2on2; c.i = 0.0f;     c.j = 0.0f; c.k = sqrt2on2;
		/* a times angle c is b */
		S_quat_copy(&tmp, &c);
		S_quat_multiply(&c, &a);
		print_3_quats(&tmp, &a, &c);
	, S_quat_equals(EPSILON, &c, &b)
	, "S_quat_multiply (transition.Q)");

	TEST(
		S_quat_identity(&a);
		S_quat_identity(&b);
		S_quat_copy(&tmp, &a);
		S_quat_multiply(&a, &b);
		print_3_quats(&tmp, &b, &a);
	, S_quat_equals(EPSILON, &a, &b)
	, "S_quat_multiply (I.I)");

	TEST(
		a.r = sqrt2on2; a.i = 0.0f; a.j = 0.0f; a.k = sqrt2on2;
		S_quat_identity(&b);
		S_quat_copy(&tmp, &a);
		S_quat_multiply(&a, &b);
		print_3_quats(&tmp, &b, &a);
	, S_quat_equals(EPSILON, &a, &tmp)
	, "S_quat_multiply (Q.I)");

	TEST(
		a.r = sqrt2on2; a.i = 0.0f; a.j = 0.0f; a.k = sqrt2on2;
		S_quat_identity(&b);
		S_quat_copy(&tmp, &a);
		S_quat_multiply(&b, &a);
		print_3_quats(&b, &tmp, &a);
	, S_quat_equals(EPSILON, &a, &tmp)
	, "S_quat_multiply (I.Q)");

	TEST(
		a.r = sqrt2on2; a.i = 0.0f; a.j = 0.0f; a.k = sqrt2on2;
		b.r = sqrt2on2; b.i = sqrt2on2; b.j = 0.0f; b.k = 0.0f;
		c.r = 0.5f;     c.i = 0.5f;     c.j = 0.5f; c.k = 0.5f;
		S_quat_copy(&tmp, &a);
		S_quat_multiply(&a, &b);
		print_3_quats(&tmp, &b, &a);
	, S_quat_equals(EPSILON, &a, &c)
	, "S_quat_multiply (Q1.Q2)");

	TEST(
		S_quat_identity(&a);
		b.r = sqrt2on2; b.i = 0.23f; b.j = -0.536f; b.k = 0.4f;
		S_quat_lerp(&b, &a, 0.0f);
		print_2_quats(&a, &b);
	, S_quat_equals(EPSILON, &b, &a)
	, "S_quat_lerp (0.0)");

	TEST(
		S_quat_identity(&a);
		b.r = sqrt2on2; b.i = 0.23f; b.j = -0.536f; b.k = 0.4f;
		c.r = 0.98228f; c.i = 0.06094f; c.j = -0.14203f; c.k = 0.10599f;
		S_quat_copy(&tmp, &b);
		S_quat_lerp(&b, &a, 0.25f);
		print_2_quats(&tmp, &b);
	, S_quat_equals(EPSILON, &b, &c)
	, "S_quat_lerp (0.25)");

	TEST(
		S_quat_identity(&a);
		b.r = sqrt2on2; b.i = 0.23f; b.j = -0.536f; b.k = 0.4f;
		c.r = 0.92385f; c.i = 0.12447f; c.j = -0.29007f; c.k = 0.21647f;
		S_quat_copy(&tmp, &b);
		S_quat_lerp(&b, &a, 0.5f);
		print_2_quats(&tmp, &b);
	, S_quat_equals(EPSILON, &b, &c)
	, "S_quat_lerp (0.5)");

	TEST(
		S_quat_identity(&a);
		b.r = sqrt2on2; b.i = 0.23f; b.j = -0.536f; b.k = 0.4f;
		S_quat_copy(&tmp, &b);
		S_quat_lerp(&b, &a, 1.0f);
		print_2_quats(&tmp, &b);
	, S_quat_equals(EPSILON, &b, &tmp)
	, "S_quat_lerp (1.0)");

	TEST(
		S_quat_identity(&a);
		b.r = sqrt2on2; b.i = 0.23f; b.j = -0.536f; b.k = 0.4f;
		S_quat_slerp(&b, &a, 0.0f);
		print_2_quats(&a, &b);
	, S_quat_equals(EPSILON, &b, &a)
	, "S_quat_slerp (0.0)");

	TEST(
		S_quat_identity(&a);
		b.r = sqrt2on2; b.i = 0.23f; b.j = -0.536f; b.k = 0.4f;
		c.r = 0.98078f; c.i = 0.06346f; c.j = -0.14788f; c.k = 0.11036f;
		S_quat_copy(&tmp, &b);
		S_quat_slerp(&b, &a, 0.25f);
		print_2_quats(&tmp, &b);
	, S_quat_equals(EPSILON, &b, &c)
	, "S_quat_slerp (0.25)");

	TEST(
		S_quat_identity(&a);
		b.r = sqrt2on2; b.i = 0.23f; b.j = -0.536f; b.k = 0.4f;
		c.r = 0.92385f; c.i = 0.12447f; c.j = -0.29007f; c.k = 0.21647f;
		S_quat_copy(&tmp, &b);
		S_quat_slerp(&b, &a, 0.5f);
		print_2_quats(&tmp, &b);
		ATOMIC_PRINT("%f %f %f %f\n", b.r, b.i, b.j, b.k);
	, S_quat_equals(EPSILON, &b, &c)
	, "S_quat_slerp (0.5)");

	TEST(
		S_quat_identity(&a);
		b.r = sqrt2on2; b.i = 0.23f; b.j = -0.536f; b.k = 0.4f;
		S_quat_copy(&tmp, &b);
		S_quat_slerp(&b, &a, 1.0f);
		print_2_quats(&tmp, &b);
	, S_quat_equals(EPSILON, &b, &tmp)
	, "S_quat_slerp (1.0)");

	TEST(
		a.r = sqrt2on2; a.i = 0.0f; a.j = 0.0f; a.k = sqrt2on2;
		vecb.x = 0.0f; vecb.y = 0.0f; vecb.z = 90.0f;
		S_quat_to_vec3(&veca, &a);
		print_quat_vec(&a, &veca);
	, S_vec3_equals(EULER_EPSILON, &veca, &vecb)
	, "S_quat_to_vec3 (gimbal lock)");

	TEST(
		a.r = sqrt2on2; a.i = 0.23f; a.j = -0.536f; a.k = 0.4f;
		vecb.x = 48.93121f; vecb.y = -60.87643f; vecb.z = 29.05586f;
		S_quat_to_vec3(&veca, &a);
		print_quat_vec(&a, &veca);
	, S_vec3_equals(EULER_EPSILON, &veca, &vecb)
	, "S_quat_to_vec3 (arbitrary)");

	FREE();

	return EXIT_SUCCESS;
}

