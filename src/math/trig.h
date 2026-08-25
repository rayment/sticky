/*
 * trig.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 16/08/2026
 */

#ifndef STICKY_MATH_TRIG_H
#define STICKY_MATH_TRIG_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define st_float32_sin(x) (sinf(x))
#define st_float32_cos(x) (cosf(x))
#define st_float32_tan(x) (tanf(x))
#define st_float32_asin(x) (asinf(x))
#define st_float32_acos(x) (acosf(x))
#define st_float32_atan(x) (atanf(x))
#define st_float32_atan2(y, x) (atan2f(y, x))

#define st_float64_sin(x) (sin(x))
#define st_float64_cos(x) (cos(x))
#define st_float64_tan(x) (tan(x))
#define st_float64_asin(x) (asin(x))
#define st_float64_acos(x) (acos(x))
#define st_float64_atan(x) (atan(x))
#define st_float64_atan2(y, x) (atan2(y, x))

#ifdef __cplusplus
}
#endif

#endif // STICKY_MATH_TRIG_H
