/*
 * float.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 16/08/2026
 */

#ifndef STICKY_MATH_BASE_H
#define STICKY_MATH_BASE_H

#include "math/arithmetic.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ST_FLOAT32_EPSILON FLT_EPSILON
#define ST_FLOAT64_EPSILON DBL_EPSILON

#define st_float32_equal(a, b) \
    (st_float32_abs((a) - (b)) <= \
     ST_FLOAT32_EPSILON * st_float32_max(1.0f, st_float32_max(st_float32_abs(a), st_float32_abs(b))))
#define st_float32_gt(a, b) \
    ((a) > (b) && !st_float32_equal((a), (b)))
#define st_float32_ge(a, b) \
    ((a) > (b) || st_float32_equal((a), (b)))
#define st_float32_lt(a, b) \
    ((a) < (b) && !st_float32_equal((a), (b)))
#define st_float32_le(a, b) \
    ((a) < (b) || st_float32_equal((a), (b)))

#define st_float64_equal(a, b) \
    (st_float64_abs((a) - (b)) <= \
     ST_FLOAT64_EPSILON * st_float64_max(1.0, st_float64_max(st_float64_abs(a), st_float64_abs(b))))
#define st_float64_gt(a, b) \
    ((a) > (b) && !st_float64_equal((a), (b)))
#define st_float64_ge(a, b) \
    ((a) > (b) || st_float64_equal((a), (b)))
#define st_float64_lt(a, b) \
    ((a) < (b) && !st_float64_equal((a), (b)))
#define st_float64_le(a, b) \
    ((a) < (b) || st_float64_equal((a), (b)))

#ifdef __cplusplus
}
#endif

#endif // STICKY_MATH_BASE_H
