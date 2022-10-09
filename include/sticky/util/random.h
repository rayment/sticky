/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * random.h
 * Random number generator header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/09/2021
 */

#ifndef FR_RAYMENT_STICKY_RANDOM_H
#define FR_RAYMENT_STICKY_RANDOM_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/types.h"

/**
 * @addtogroup random
 * @{
 */

/**
 * @brief Set the seed for the random number generator.
 *
 * Using a given seed will lead to deterministic results from random functions.
 * That is, choosing ten random numbers after setting a given seed will always
 * give the exact same ten numbers being generated. As such, this function is
 * useful for testing non-deterministic components of a program that rely on
 * random numbers.
 *
 * @param[in] seed The seed to initialise the number generator.
 * @since 1.0.0
 */
STICKY_API void    S_random_set_seed(Suint32);

/**
 * @brief Get the current seed of the random number generator.
 *
 * Returns the seed set by {@link S_random_set_seed(Suint32)} or the default
 * seed set by <b>sticky</b> in {@link S_sticky_init(void)}.
 *
 * @return The current seed for the number generator.
 * @since 1.0.0
 */
STICKY_API Suint32 S_random_get_seed(void);

/**
 * @brief Randomly generate a boolean value.
 *
 * Randomly pick either {@link S_TRUE} or {@link S_FALSE} and return it.
 *
 * @return A random boolean value.
 * @since 1.0.0
 */
STICKY_API Sbool   S_random_next_bool(void);

/**
 * @brief Randomly generate a char value.
 *
 * Randomly generate a {@link Schar} value between 0 and {@link S_CHAR_MAX}.
 *
 * @return A random char value.
 * @since 1.0.0
 */
STICKY_API Schar   S_random_next_char(void);

/**
 * @brief Randomly generate a single-precision floating-point value.
 *
 * Randomly generate a {@link Sfloat} value.
 *
 * @return A random single-precision floating-point value.
 * @since 1.0.0
 */
STICKY_API Sfloat  S_random_next_float(void);

/**
 * @brief Randomly generate a double-precision floating-point value.
 *
 * Randomly generate a {@link Sdouble} value.
 *
 * @return A random double-precision floating-point value.
 * @since 1.0.0
 */
STICKY_API Sdouble S_random_next_double(void);

/**
 * @brief Randomly generate an 8-bit unsigned integer value.
 *
 * Randomly generate a {@link Suint8} value between 0 and {@link S_UINT8_MAX}.
 *
 * @return A random 8-bit unsigned integer value.
 * @since 1.0.0
 */
STICKY_API Suint8  S_random_next_uint8(void);

/**
 * @brief Randomly generate an 16-bit unsigned integer value.
 *
 * Randomly generate a {@link Suint16} value between 0 and {@link S_UINT16_MAX}.
 *
 * @return A random 16-bit unsigned integer value.
 * @since 1.0.0
 */
STICKY_API Suint16 S_random_next_uint16(void);

/**
 * @brief Randomly generate an 32-bit unsigned integer value.
 *
 * Randomly generate a {@link Suint32} value between 0 and {@link S_UINT32_MAX}.
 *
 * @return A random 32-bit unsigned integer value.
 * @since 1.0.0
 */
STICKY_API Suint32 S_random_next_uint32(void);

/**
 * @brief Randomly generate an 64-bit unsigned integer value.
 *
 * Randomly generate a {@link Suint64} value between 0 and {@link S_UINT64_MAX}.
 *
 * @return A random 64-bit unsigned integer value.
 * @since 1.0.0
 */
STICKY_API Suint64 S_random_next_uint64(void);

/**
 * @brief Randomly generate an 8-bit signed integer value.
 *
 * Randomly generate a {@link Sint8} value between 0 and {@link S_INT8_MAX}.
 *
 * @return A random 8-bit signed integer value.
 * @since 1.0.0
 */
STICKY_API Sint8   S_random_next_int8(void);

/**
 * @brief Randomly generate an 16-bit signed integer value.
 *
 * Randomly generate a {@link Sint16} value between 0 and {@link S_INT16_MAX}.
 *
 * @return A random 16-bit signed integer value.
 * @since 1.0.0
 */
STICKY_API Sint16  S_random_next_int16(void);

/**
 * @brief Randomly generate an 32-bit signed integer value.
 *
 * Randomly generate a {@link Sint32} value between 0 and {@link S_INT32_MAX}.
 *
 * @return A random 32-bit signed integer value.
 * @since 1.0.0
 */
STICKY_API Sint32  S_random_next_int32(void);

/**
 * @brief Randomly generate an 64-bit signed integer value.
 *
 * Randomly generate a {@link Sint64} value between 0 and {@link S_INT64_MAX}.
 *
 * @return A random 64-bit signed integer value.
 * @since 1.0.0
 */
STICKY_API Sint64  S_random_next_int64(void);

/**
 * @brief Randomly generate a char between two values.
 * @hideinitializer
 *
 * If a given value @p max is less than a given value @p min, or either
 * parameter is negative, then this function is undefined.
 *
 * @param[in] min The minimum value (inclusive) for the random number.
 * @param[in] max The maximum value (exclusive) for the random number.
 * @return A random char value within a given range.
 * @since 1.0.0
 */
#define S_random_range_char(min, max)   \
	(S_random_next_char()%((max)-(min))+(min))

/**
 * @brief Randomly generate a single-precision floating-point number between two
 * values.
 * @hideinitializer
 *
 * If a given value @p max is less than a given value @p min, or either
 * parameter is negative, then this function is undefined.
 *
 * @param[in] min The minimum value (inclusive) for the random number.
 * @param[in] max The maximum value (exclusive) for the random number.
 * @return A random single-precision floating-point value within a given range.
 * @since 1.0.0
 */
#define S_random_range_float(min, max)  \
	(S_random_next_float()*((max)-(min))+(min))

/**
 * @brief Randomly generate a double-precision floating-point number between two
 * values.
 * @hideinitializer
 *
 * If a given value @p max is less than a given value @p min, or either
 * parameter is negative, then this function is undefined.
 *
 * @param[in] min The minimum value (inclusive) for the random number.
 * @param[in] max The maximum value (exclusive) for the random number.
 * @return A random double-precision floating-point value within a given range.
 * @since 1.0.0
 */
#define S_random_range_double(min, max) \
	(S_random_next_double()*((max)-(min))+(min))

/**
 * @brief Randomly generate a 8-bit unsigned integer between two values.
 * @hideinitializer
 *
 * If a given value @p max is less than a given value @p min, or either
 * parameter is negative, then this function is undefined.
 *
 * @param[in] min The minimum value (inclusive) for the random number.
 * @param[in] max The maximum value (exclusive) for the random number.
 * @return A random 8-bit unsigned integer within a given range.
 * @since 1.0.0
 */
#define S_random_range_uint8(min, max)  \
	(S_random_next_uint8()%((max)-(min))+(min))

/**
 * @brief Randomly generate a 16-bit unsigned integer between two values.
 * @hideinitializer
 *
 * If a given value @p max is less than a given value @p min, or either
 * parameter is negative, then this function is undefined.
 *
 * @param[in] min The minimum value (inclusive) for the random number.
 * @param[in] max The maximum value (exclusive) for the random number.
 * @return A random 16-bit unsigned integer within a given range.
 * @since 1.0.0
 */
#define S_random_range_uint16(min, max) \
	(S_random_next_uint16()%((max)-(min))+(min))

/**
 * @brief Randomly generate a 32-bit unsigned integer between two values.
 * @hideinitializer
 *
 * If a given value @p max is less than a given value @p min, or either
 * parameter is negative, then this function is undefined.
 *
 * @param[in] min The minimum value (inclusive) for the random number.
 * @param[in] max The maximum value (exclusive) for the random number.
 * @return A random 32-bit unsigned integer within a given range.
 * @since 1.0.0
 */
#define S_random_range_uint32(min, max) \
	(S_random_next_uint32()%((max)-(min))+(min))

/**
 * @brief Randomly generate a 64-bit unsigned integer between two values.
 * @hideinitializer
 *
 * If a given value @p max is less than a given value @p min, or either
 * parameter is negative, then this function is undefined.
 *
 * @param[in] min The minimum value (inclusive) for the random number.
 * @param[in] max The maximum value (exclusive) for the random number.
 * @return A random 64-bit unsigned integer within a given range.
 * @since 1.0.0
 */
#define S_random_range_uint64(min, max) \
	(S_random_next_uint64()%((max)-(min))+(min))

/**
 * @brief Randomly generate a 8-bit signed integer between two values.
 * @hideinitializer
 *
 * If a given value @p max is less than a given value @p min, or either
 * parameter is negative, then this function is undefined.
 *
 * @param[in] min The minimum value (inclusive) for the random number.
 * @param[in] max The maximum value (exclusive) for the random number.
 * @return A random 8-bit signed integer within a given range.
 * @since 1.0.0
 */
#define S_random_range_int8(min, max)   \
	(S_random_next_int8()%((max)-(min))+(min))

/**
 * @brief Randomly generate a 16-bit signed integer between two values.
 * @hideinitializer
 *
 * If a given value @p max is less than a given value @p min, or either
 * parameter is negative, then this function is undefined.
 *
 * @param[in] min The minimum value (inclusive) for the random number.
 * @param[in] max The maximum value (exclusive) for the random number.
 * @return A random 16-bit signed integer within a given range.
 * @since 1.0.0
 */
#define S_random_range_int16(min, max)  \
	(S_random_next_int16()%((max)-(min))+(min))

/**
 * @brief Randomly generate a 32-bit signed integer between two values.
 * @hideinitializer
 *
 * If a given value @p max is less than a given value @p min, or either
 * parameter is negative, then this function is undefined.
 *
 * @param[in] min The minimum value (inclusive) for the random number.
 * @param[in] max The maximum value (exclusive) for the random number.
 * @return A random 32-bit signed integer within a given range.
 * @since 1.0.0
 */
#define S_random_range_int32(min, max)  \
	(S_random_next_int32()%((max)-(min))+(min))

/**
 * @brief Randomly generate a 64-bit signed integer between two values.
 * @hideinitializer
 *
 * If a given value @p max is less than a given value @p min, or either
 * parameter is negative, then this function is undefined.
 *
 * @param[in] min The minimum value (inclusive) for the random number.
 * @param[in] max The maximum value (exclusive) for the random number.
 * @return A random 64-bit signed integer within a given range.
 * @since 1.0.0
 */
#define S_random_range_int64(min, max)  \
	(S_random_next_int64()%((max)-(min))+(min))

/**
 * @brief Randomly shuffle the contents of an array.
 *
 * Randomly shuffle the order of elements of an array.
 *
 * @param[in,out] ptr The array to shuffle.
 * @param[in] nmemb The number of elements in the array to shuffle.
 * @param[in] size The size of each element in the array in bytes.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid array is provided to
 * the function, or if @p nmemb or @p size is equal to <c>0</c>.
 * @since 1.0.0
 */
STICKY_API void    S_random_shuffle_array(void *, Ssize_t, Ssize_t);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_RANDOM_H */

