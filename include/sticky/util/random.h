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

#include "sticky/common/types.h"

/**
 * @file random.h
 */

/**
 * @addtogroup random
 * @{
 */

void    S_random_set_seed(Suint32);

Suint32 S_random_get_seed(void);

Sbool   S_random_next_bool(void);

Schar   S_random_next_char(void);

Sfloat  S_random_next_float(void);

Sdouble S_random_next_double(void);

Suint8  S_random_next_uint8(void);

Suint16 S_random_next_uint16(void);

Suint32 S_random_next_uint32(void);

Suint64 S_random_next_uint64(void);

Sint8   S_random_next_int8(void);

Sint16  S_random_next_int16(void);

Sint32  S_random_next_int32(void);

Sint64  S_random_next_int64(void);

#define S_random_range_char(min, max)   \
	(S_random_next_char()%((max)-(min))+(min))

#define S_random_range_float(min, max)  \
	(S_random_next_float()*((max)-(min))+(min))

#define S_random_range_double(min, max) \
	(S_random_next_double()*((max)-(min))+(min))

#define S_random_range_uint8(min, max)  \
	(S_random_next_uint8()%((max)-(min))+(min))

#define S_random_range_uint16(min, max) \
	(S_random_next_uint16()%((max)-(min))+(min))

#define S_random_range_uint32(min, max) \
	(S_random_next_uint32()%((max)-(min))+(min))

#define S_random_range_uint64(min, max) \
	(S_random_next_uint64()%((max)-(min))+(min))

#define S_random_range_int8(min, max)   \
	(S_random_next_int8()%((max)-(min))+(min))

#define S_random_range_int16(min, max)  \
	(S_random_next_int16()%((max)-(min))+(min))

#define S_random_range_int32(min, max)  \
	(S_random_next_int32()%((max)-(min))+(min))

#define S_random_range_int64(min, max)  \
	(S_random_next_int64()%((max)-(min))+(min))

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_RANDOM_H */

