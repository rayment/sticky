/*
 * arithmetic.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 16/08/2026
 */

#ifndef STICKY_MATH_ARITHMETIC_H
#define STICKY_MATH_ARITHMETIC_H

#include <float.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define st_float32_abs(x) fabsf(x)
#define st_float64_abs(x) fabs(x)
// Assumption: Macro doesn't care for types and implementations are all the same.
// TODO: Document that lowest value (e.g. -128 for int8_t) is not handled.
#define st_int8_abs(x) ((x) >= 0 ? (x) : ST_CAST(st_int8, -(x)))
#define st_int16_abs(x) st_int8_abs(x)
#define st_int32_abs(x) st_int8_abs(x)
#define st_int64_abs(x) st_int8_abs(x)

#define st_float32_min(x, y) fminf((x), (y))
#define st_float64_min(x, y) fmin((x), (y))
// Assumption: Macro doesn't care for types and implementations are all the same.
#define st_uint8_min(x, y) ((x) < (y) ? (x) : (y))
#define st_int8_min(x, y)   st_uint8_min((x), (y))
#define st_uint16_min(x, y) st_uint8_min((x), (y))
#define st_int16_min(x, y)  st_uint8_min((x), (y))
#define st_uint32_min(x, y) st_uint8_min((x), (y))
#define st_int32_min(x, y)  st_uint8_min((x), (y))
#define st_uint64_min(x, y) st_uint8_min((x), (y))
#define st_int64_min(x, y)  st_uint8_min((x), (y))

#define st_float32_max(x, y) fmaxf((x), (y))
#define st_float64_max(x, y) fmax((x), (y))
// Assumption: Macro doesn't care for types and implementations are all the same.
#define st_uint8_max(x, y) ((x) > (y) ? (x) : (y))
#define st_int8_max(x, y)   st_uint8_max((x), (y))
#define st_uint16_max(x, y) st_uint8_max((x), (y))
#define st_int16_max(x, y)  st_uint8_max((x), (y))
#define st_uint32_max(x, y) st_uint8_max((x), (y))
#define st_int32_max(x, y)  st_uint8_max((x), (y))
#define st_uint64_max(x, y) st_uint8_max((x), (y))
#define st_int64_max(x, y)  st_uint8_max((x), (y))

#define st_float32_clamp(x, min, max) (st_float32_min((max), st_float32_max((min), (x))))
#define st_float64_clamp(x, min, max) (st_float64_min((max), st_float64_max((min), (x))))
#define st_uint8_clamp(x, min, max)   (st_uint8_min((max),   st_uint8_max((min), (x))))
#define st_int8_clamp(x, min, max)    (st_int8_min((max),    st_int8_max((min), (x))))
#define st_uint16_clamp(x, min, max)  (st_uint16_min((max),  st_uint16_max((min), (x))))
#define st_int16_clamp(x, min, max)   (st_int16_min((max),   st_int16_max((min), (x))))
#define st_uint32_clamp(x, min, max)  (st_uint32_min((max),  st_uint32_max((min), (x))))
#define st_int32_clamp(x, min, max)   (st_int32_min((max),   st_int32_max((min), (x))))
#define st_uint64_clamp(x, min, max)  (st_uint64_min((max),  st_uint64_max((min), (x))))
#define st_int64_clamp(x, min, max)   (st_int64_min((max),   st_int64_max((min), (x))))

#define st_float32_floor(x) floorf(x)
#define st_float64_floor(x) floor(x)

#define st_float32_ceil(x) ceilf(x)
#define st_float64_ceil(x) ceil(x)

#define st_float32_round(x) roundf(x)
#define st_float64_round(x) round(x)

#ifdef __cplusplus
}
#endif

#endif // STICKY_MATH_ARITHMETIC_H
