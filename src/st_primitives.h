/*
 * st_primitives.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/08/2026
 */

#ifndef STICKY_ST_PRIMITIVES_H
#define STICKY_ST_PRIMITIVES_H

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "st_macros.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
typedef bool st_bool;
#else
typedef _Bool    st_bool;
#endif
typedef int8_t   st_int8;
typedef uint8_t  st_uint8;
typedef int16_t  st_int16;
typedef uint16_t st_uint16;
typedef int32_t  st_int32;
typedef uint32_t st_uint32;
typedef int64_t  st_int64;
typedef uint64_t st_uint64;
typedef char     st_char;
typedef size_t   st_size;
typedef float_t  st_float32;
typedef double_t st_float64;
typedef st_size  st_enum;
typedef int8_t   st_comparator;
typedef st_size  st_index;
typedef void *   st_voidptr;

#if !defined(__bool_true_false_are_defined) || \
(__bool_true_false_are_defined != 1)
#define true ST_CAST(st_bool, 1)
#define false ST_CAST(st_bool, 0)
#endif

#ifndef __cplusplus
#ifndef nullptr
#ifdef NULL
#define nullptr ST_CAST(st_voidptr, NULL)
#else
#define nullptr ST_CAST(void *, 0)
#endif
#endif
#endif

#ifndef ST_INDEX_NONE
#define ST_INDEX_NONE ST_CAST(st_index, -1)
#endif

#ifdef __cplusplus
}
#endif

#endif // STICKY_ST_PRIMITIVES_H
