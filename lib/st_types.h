/*
 * st_types.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

/**
 * @file
 * @brief Fixed-width primitive types used throughout the engine.
 *
 * C code uses the `st_` prefixed typedefs. C++ code uses the equivalents in the `st` namespace.
 */

#ifndef STICKY_ST_TYPES_H
#define STICKY_ST_TYPES_H

#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#else // __cplusplus
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#endif // __cplusplus

/** @addtogroup core
 *  @{
 */

typedef int8_t  st_int8; ///< Signed 8-bit integer.
typedef int16_t st_int16; ///< Signed 16-bit integer.
typedef int32_t st_int32; ///< Signed 32-bit integer.
typedef int64_t st_int64; ///< Signed 64-bit integer.

typedef uint8_t  st_uint8; ///< Unsigned 8-bit integer.
typedef uint16_t st_uint16; ///< Unsigned 16-bit integer.
typedef uint32_t st_uint32; ///< Unsigned 32-bit integer.
typedef uint64_t st_uint64; ///< Unsigned 64-bit integer.

typedef char st_char; ///< Character of a UTF-8 string.

typedef float  st_float32; ///< 32-bit floating point number.
typedef double st_float64; ///< 64-bit floating point number.

typedef size_t st_size; ///< Size of an object or count of elements.

#ifndef __cplusplus
typedef _Bool st_bool; ///< Boolean.

#define nullptr ((void *) 0)
#else // __cplusplus
typedef bool st_bool; ///< Boolean.
#endif // __cplusplus

#ifdef __cplusplus
namespace st
{
    typedef st_int8  int8; ///< Signed 8-bit integer.
    typedef st_int16 int16; ///< Signed 16-bit integer.
    typedef st_int32 int32; ///< Signed 32-bit integer.
    typedef st_int64 int64; ///< Signed 64-bit integer.

    typedef st_uint8  uint8; ///< Unsigned 8-bit integer.
    typedef st_uint16 uint16; ///< Unsigned 16-bit integer.
    typedef st_uint32 uint32; ///< Unsigned 32-bit integer.
    typedef st_uint64 uint64; ///< Unsigned 64-bit integer.

    typedef st_char char_t; ///< Character of a UTF-8 string.

    typedef st_float32 float32; ///< 32-bit floating point number.
    typedef st_float64 float64; ///< 64-bit floating point number.

    typedef st_size size; ///< Size of an object or count of elements.

    typedef bool bool_t; ///< Boolean.
} // namespace st
#endif // __cplusplus

/** @} */

#endif // STICKY_ST_TYPES_H
