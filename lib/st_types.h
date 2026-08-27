/*
 * st_types.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#ifndef STICKY_ST_TYPES_H
#define STICKY_ST_TYPES_H

#ifdef __cplusplus
#include <cstdint>
#else // __cplusplus
#include <stdbool.h>
#include <stdint.h>
#endif // __cplusplus

typedef int8_t   st_int8;
typedef int16_t  st_int16;
typedef int32_t  st_int32;
typedef int64_t  st_int64;

typedef uint8_t  st_uint8;
typedef uint16_t st_uint16;
typedef uint32_t st_uint32;
typedef uint64_t st_uint64;

typedef char     st_char;

typedef float    st_float32;
typedef double   st_float64;

typedef size_t   st_size;
typedef size_t   st_enum;

#ifndef __cplusplus
typedef _Bool    st_bool;

#define nullptr ((void*)0)
#else // __cplusplus
typedef bool     st_bool;
#endif // __cplusplus

#ifdef __cplusplus
namespace st
{
    typedef st_int8    int8;
    typedef st_int16   int16;
    typedef st_int32   int32;
    typedef st_int64   int64;

    typedef st_uint8   uint8;
    typedef st_uint16  uint16;
    typedef st_uint32  uint32;
    typedef st_uint64  uint64;

    typedef st_char    char_t;

    typedef st_float32 float32;
    typedef st_float64 float64;

    typedef st_size    size;
    typedef st_size    enum_t;

    typedef bool       bool_t;
}
#endif // __cplusplus

#endif // STICKY_ST_TYPES_H
