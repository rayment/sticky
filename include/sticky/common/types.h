/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * types.h
 * Data types header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

/**
 * @file types.h
 * @addtogroup types Generic types
 * @{
 */

#ifndef FR_RAYMENT_STICKY_TYPES_H
#define FR_RAYMENT_STICKY_TYPES_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Boolean value that represents statement truth.
 *
 * Opposite of {@link S_FALSE}.
 *
 * @since 1.0.0
 */
#define S_TRUE        1
/**
 * @brief Boolean value that represents statement falsity.
 *
 * Opposite of {@link S_TRUE}.
 *
 * @since 1.0.0
 */
#define S_FALSE       0

/**
 * @brief Boolean data-type.
 *
 * Supports {@link S_TRUE} or {@link S_FALSE} values.
 *
 * @since 1.0.0
 */
typedef uint8_t       Sbool;

/**
 * @brief 8-bit character data-type.
 *
 * Supports values from 0 - 255. Used for storing ASCII characters.
 *
 * @since 1.0.0
 */
typedef char          Schar;

/**
 * @brief Single-precision IEEE-754 floating point data-type.
 *
 * @since 1.0.0
 */
typedef float         Sfloat;
/**
 * @brief Double-precision IEEE-754 floating point data-type.
 *
 * @since 1.0.0
 */
typedef double        Sdouble;

/**
 * @brief Wrapper for system-dependant <b><c>size_t</c></b> data-type.
 *
 * An undefined integer guaranteed to be able to represent the largest size an
 * object can be in bytes for a given system.
 *
 * @since 1.0.0
 */
typedef size_t        Ssize_t;
/**
 * @brief Comparison operation data-type.
 *
 * Supports values -1, 0 and 1.
 *
 * @since 1.0.0
 */
typedef int8_t        Scomparator;
/**
 * @brief Enum data-type.
 *
 * Enum values in C are implemented as integer constants. This data-type should
 * be used to store them.
 *
 * @since 1.0.0
 */
typedef uint32_t      Senum;

/**
 * @brief Fixed-width signed 8-bit integer data-type.
 *
 * Supports values from -128 to 127.
 *
 * @since 1.0.0
 */
typedef int8_t        Sint8;
/**
 * @brief Fixed-width signed 16-bit integer data-type.
 *
 * Supports values from -32768 to 32767.
 *
 * @since 1.0.0
 */
typedef int16_t       Sint16;
/**
 * @brief Fixed-width signed 32-bit integer data-type.
 *
 * Supports values from -2147483648 to 2147483647.
 *
 * @since 1.0.0
 */
typedef int32_t       Sint32;
/**
 * @brief Fixed-width signed 64-bit integer data-type.
 *
 * Supports values from -9223372036854775808 to 9223372036854775807.
 *
 * @since 1.0.0
 */
typedef int64_t       Sint64;

/**
 * @brief Fixed-width unsigned 8-bit integer data-type.
 *
 * Supports values from 0 to 255.
 *
 * @since 1.0.0
 */
typedef uint8_t       Suint8;
/**
 * @brief Fixed-width unsigned 16-bit integer data-type.
 *
 * Supports values from 0 to 65535.
 *
 * @since 1.0.0
 */
typedef uint16_t      Suint16;
/**
 * @brief Fixed-width unsigned 32-bit integer data-type.
 *
 * Supports values from 0 to 4294967295.
 *
 * @since 1.0.0
 */
typedef uint32_t      Suint32;
/**
 * @brief Fixed-width unsigned 64-bit integer data-type.
 *
 * Supports values from 0 to 18446744073709551615.
 *
 * @since 1.0.0
 */
typedef uint64_t      Suint64;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_TYPES_H */

