/*
 * vec3.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 16/08/2026
 */

#ifndef STICKY_MATH_VEC3_H
#define STICKY_MATH_VEC3_H

#include "st_macros.h"
#include "st_primitives.h"
#include "math/arithmetic.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ST_PRIV_TYPEDEF_VEC3(type, name, len) \
    typedef struct ST_PACKED ST_ALIGNED(len) st_vec3_##name \
    { \
        type x; \
        type y; \
        type z; \
    } st_vec3_##name##_t

#define ST_PRIV_ZERO_VEC3(name, zero) (st_vec3_##name##_t){(zero), (zero), (zero)}

#define ST_PRIV_ADD_VEC3(type, name) \
    inline st_vec3_##name##_t st_vec3_##name##_add(st_vec3_##name##_t v1, st_vec3_##name##_t v2) \
    { \
        st_vec3_##name##_t a; \
        a.x = v1.x + v2.x; \
        a.y = v1.y + v2.y; \
        a.z = v1.z + v2.z; \
        return a; \
    }

#define ST_PRIV_SUB_VEC3(type, name) \
    inline st_vec3_##name##_t st_vec3_##name##_sub(st_vec3_##name##_t v1, st_vec3_##name##_t v2) \
    { \
        st_vec3_##name##_t a; \
        a.x = v1.x - v2.x; \
        a.y = v1.y - v2.y; \
        a.z = v1.z - v2.z; \
        return a; \
    }

#define ST_PRIV_MUL_VEC3(type, name) \
    inline st_vec3_##name##_t st_vec3_##name##_mul(st_vec3_##name##_t v1, st_vec3_##name##_t v2) \
    { \
        st_vec3_##name##_t a; \
        a.x = v1.x * v2.x; \
        a.y = v1.y * v2.y; \
        a.z = v1.z * v2.z; \
        return a; \
    }

#define ST_PRIV_DIV_VEC3(type, name) \
    inline st_vec3_##name##_t st_vec3_##name##_div(st_vec3_##name##_t v1, st_vec3_##name##_t v2) \
    { \
        st_vec3_##name##_t a; \
        a.x = v1.x / v2.x; \
        a.y = v1.y / v2.y; \
        a.z = v1.z / v2.z; \
        return a; \
    }

#define ST_PRIV_MOD_VEC3(type, name) \
    inline st_vec3_##name##_t st_vec3_##name##_mod(st_vec3_##name##_t v1, type i) \
    { \
        st_vec3_##name##_t a; \
        a.x = v1.x % i; \
        a.y = v1.y % i; \
        a.z = v1.z % i; \
        return a; \
    }

#define ST_PRIV_ABS_VEC3(type, name) \
    inline st_vec3_##name##_t st_vec3_##name##_abs(st_vec3_##name##_t v) \
    { \
        st_vec3_##name##_t a; \
        a.x = st_##name##_abs(v.x); \
        a.y = st_##name##_abs(v.y); \
        a.z = st_##name##_abs(v.z); \
        return a; \
    }

#define ST_PRIV_MIN_VEC3(type, name) \
    inline type st_vec3_##name##_min(st_vec3_##name##_t v1) \
    { \
        return st_##name##_min(st_##name##_min(v1.x, v1.y), v1.z); \
    }

#define ST_PRIV_MAX_VEC3(type, name) \
    inline type st_vec3_##name##_max(st_vec3_##name##_t v1) \
    { \
        return st_##name##_max(st_##name##_max(v1.x, v1.y), v1.z); \
    }

#define ST_PRIV_CLAMP_VEC3(type, name) \
    inline st_vec3_##name##_t st_vec3_##name##_clamp(st_vec3_##name##_t v, type min, type max) \
    { \
        st_vec3_##name##_t a; \
        a.x = st_##name##_clamp(v.x, min, max); \
        a.y = st_##name##_clamp(v.y, min, max); \
        a.z = st_##name##_clamp(v.z, min, max); \
        return a; \
    }

#define ST_PRIV_FLOOR_VEC3(type, name) \
    inline st_vec3_##name##_t st_vec3_##name##_floor(st_vec3_##name##_t v) \
    { \
        st_vec3_##name##_t a; \
        a.x = st_##name##_floor(v.x); \
        a.y = st_##name##_floor(v.y); \
        a.z = st_##name##_floor(v.z); \
        return a; \
    }

#define ST_PRIV_CEIL_VEC3(type, name) \
    inline st_vec3_##name##_t st_vec3_##name##_ceil(st_vec3_##name##_t v) \
    { \
        st_vec3_##name##_t a; \
        a.x = st_##name##_ceil(v.x); \
        a.y = st_##name##_ceil(v.y); \
        a.z = st_##name##_ceil(v.z); \
        return a; \
    }

#define ST_PRIV_IMPL_VEC3_UINT(type, name, len) \
    ST_PRIV_TYPEDEF_VEC3(type, name, len); \
    ST_PRIV_ADD_VEC3    (type, name) \
    ST_PRIV_SUB_VEC3    (type, name) \
    ST_PRIV_MUL_VEC3    (type, name) \
    ST_PRIV_DIV_VEC3    (type, name) \
    ST_PRIV_MOD_VEC3    (type, name) \
    ST_PRIV_MIN_VEC3    (type, name) \
    ST_PRIV_MAX_VEC3    (type, name) \
    ST_PRIV_CLAMP_VEC3  (type, name)

#define ST_PRIV_IMPL_VEC3_INT(type, name, len) \
    ST_PRIV_TYPEDEF_VEC3(type, name, len); \
    ST_PRIV_ADD_VEC3    (type, name) \
    ST_PRIV_SUB_VEC3    (type, name) \
    ST_PRIV_MUL_VEC3    (type, name) \
    ST_PRIV_DIV_VEC3    (type, name) \
    ST_PRIV_MOD_VEC3    (type, name) \
    ST_PRIV_MIN_VEC3    (type, name) \
    ST_PRIV_MAX_VEC3    (type, name) \
    ST_PRIV_ABS_VEC3    (type, name) \
    ST_PRIV_CLAMP_VEC3  (type, name)

#define ST_PRIV_IMPL_VEC3_FLOAT(type, name, len) \
    ST_PRIV_TYPEDEF_VEC3(type, name, len); \
    ST_PRIV_ADD_VEC3    (type, name) \
    ST_PRIV_SUB_VEC3    (type, name) \
    ST_PRIV_MUL_VEC3    (type, name) \
    ST_PRIV_DIV_VEC3    (type, name) \
    ST_PRIV_MIN_VEC3    (type, name) \
    ST_PRIV_MAX_VEC3    (type, name) \
    ST_PRIV_ABS_VEC3    (type, name) \
    ST_PRIV_CLAMP_VEC3  (type, name) \
    ST_PRIV_FLOOR_VEC3  (type, name) \
    ST_PRIV_CEIL_VEC3   (type, name)

ST_PRIV_IMPL_VEC3_UINT (st_uint8,   uint8,   8)
ST_PRIV_IMPL_VEC3_UINT (st_uint16,  uint16,  16)
ST_PRIV_IMPL_VEC3_UINT (st_uint32,  uint32,  32)
ST_PRIV_IMPL_VEC3_UINT (st_uint64,  uint64,  64)
#define ST_VEC3_UINT8_ZERO  ST_PRIV_ZERO_VEC3(uint8, 0)
#define ST_VEC3_UINT16_ZERO ST_PRIV_ZERO_VEC3(uint16, 0)
#define ST_VEC3_UINT32_ZERO ST_PRIV_ZERO_VEC3(uint32, 0)
#define ST_VEC3_UINT64_ZERO ST_PRIV_ZERO_VEC3(uint64, 0)

ST_PRIV_IMPL_VEC3_INT  (st_int8,    int8,    8)
ST_PRIV_IMPL_VEC3_INT  (st_int16,   int16,   16)
ST_PRIV_IMPL_VEC3_INT  (st_int32,   int32,   32)
ST_PRIV_IMPL_VEC3_INT  (st_int64,   int64,   64)
#define ST_VEC3_INT8_ZERO  ST_PRIV_ZERO_VEC3(int8, 0)
#define ST_VEC3_INT16_ZERO ST_PRIV_ZERO_VEC3(int16, 0)
#define ST_VEC3_INT32_ZERO ST_PRIV_ZERO_VEC3(int32, 0)
#define ST_VEC3_INT64_ZERO ST_PRIV_ZERO_VEC3(int64, 0)

ST_PRIV_IMPL_VEC3_FLOAT(st_float32, float32, 32)
ST_PRIV_IMPL_VEC3_FLOAT(st_float64, float64, 64)
#define ST_VEC3_FLOAT32_ZERO ST_PRIV_ZERO_VEC3(float32, 0.0f)
#define ST_VEC3_FLOAT64_ZERO ST_PRIV_ZERO_VEC3(float64, 0.0l)

#ifdef __cplusplus
}
#endif

#endif // STICKY_MATH_VEC3_H
