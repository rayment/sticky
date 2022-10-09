/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * random.c
 * Random number generator source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/09/2021
 */

#include <limits.h>
#ifdef STICKY_WINDOWS
#include <malloc.h>
#endif /* STICKY_WINDOWS */
#include <stdlib.h>
#include <string.h>

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/util/random.h"

static Suint32 rand_seed;

void
S_random_set_seed(Suint32 seed)
{
	rand_seed = seed;
	srand(seed);
}

Suint32
S_random_get_seed(void)
{
	return rand_seed;
}

Sbool
S_random_next_bool(void)
{
	return (Sbool) (rand() % 2);
}

Schar
S_random_next_char(void)
{
	return (Schar) (rand() % S_CHAR_MAX);
}

Sfloat
S_random_next_float(void)
{
	return (Sfloat) (rand() / (Sfloat) RAND_MAX);
}

Sdouble
S_random_next_double(void)
{
	return (Sdouble) (rand() / (Sfloat) RAND_MAX);
}

Suint8
S_random_next_uint8(void)
{
	return (Suint8) (rand() % S_UINT8_MAX);
}

Suint16
S_random_next_uint16(void)
{
	return (Suint16) (rand() % S_UINT16_MAX);
}

Suint32
S_random_next_uint32(void)
{
	return (Suint32) rand();
}

Suint64
S_random_next_uint64(void)
{
	Suint32 lo, hi;
	lo = (Suint32) rand();
	hi = (Suint32) rand();
	return lo | ((Suint64) hi << 32);
}

Sint8
S_random_next_int8(void)
{
	return (Sint8) (rand() % S_INT8_MAX);
}

Sint16
S_random_next_int16(void)
{
	return (Sint16) (rand() % S_INT16_MAX);
}

Sint32
S_random_next_int32(void)
{
	return (Sint32) rand();
}

Sint64
S_random_next_int64(void)
{
	Sint32 lo, hi;
	lo = (Sint32) rand();
	hi = (Sint32) rand();
	return lo | ((Suint64) hi << 32);
}

/* Fisher-Yates shuffle algorithm. */
void
S_random_shuffle_array(void *ptr,
                       Ssize_t nmemb,
					   Ssize_t size)
{
	Ssize_t idx, i;
	char *charptr;
	if (!ptr || nmemb == 0 || size == 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_random_shuffle_array");
		return;
	}
#ifdef STICKY_WINDOWS
	/* MSVC does not support VLA but we can manually use their stack allocator,
	   which will automatically use the heap if it exceeds a certain size. */
	char *tmp = (char *) _malloca(size);
#else
	/* UNIX supports VLA in C99 */
	char tmp[size];
#endif /* STICKY_WINDOWS */
	charptr = (char *) ptr;
	for (i = nmemb - 1; i > 0; --i)
	{
#ifdef STICKY_64BIT
		_S_CALL("S_random_range_int64", idx = S_random_range_int64(0, i+1));
#else /* STICKY_64BIT */
		_S_CALL("S_random_range_int32", idx = S_random_range_int32(0, i+1));
#endif /* STICKY_64BIT */
		if (i != idx)
		{
			memcpy(tmp, charptr+(idx*size), size);
			memcpy(charptr+(idx*size), charptr+(i*size), size);
			memcpy(charptr+(i*size), tmp, size);
		}
	}
#ifdef STICKY_WINDOWS
	/* _malloca needs to be followed by a _freea call. */
	_freea(tmp);
#endif /* STICKY_WINDOWS */
}

