/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * string.h
 * Character string header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 10/03/2022
 */

#ifndef FR_RAYMENT_STICKY_STRING_H
#define FR_RAYMENT_STICKY_STRING_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/types.h"

typedef struct
Sstring_s
{
	Schar *ptr;
	Ssize_t len, ptrlen;
} Sstring;

STICKY_API Sstring     *S_string_new(void);
STICKY_API Sstring     *S_string_load(const Schar *, Ssize_t);
STICKY_API void         S_string_delete(Sstring *);

STICKY_API void         S_string_reserve(Sstring *, Ssize_t);

STICKY_API void         S_string_set(Sstring *, const Schar *, Ssize_t);
STICKY_API void         S_string_setat(Sstring *, Schar, Ssize_t);
STICKY_API void         S_string_set_format(Sstring *, const Schar *, ...);
STICKY_API void         S_string_set_bool(Sstring *, Sbool);
STICKY_API void         S_string_set_float(Sstring *, Sfloat, Suint8);
STICKY_API void         S_string_set_double(Sstring *, Sdouble, Suint8);
STICKY_API void         S_string_set_uint8 (Sstring *, Suint8);
STICKY_API void         S_string_set_uint16(Sstring *, Suint16);
STICKY_API void         S_string_set_uint32(Sstring *, Suint32);
STICKY_API void         S_string_set_uint64(Sstring *, Suint64);
STICKY_API void         S_string_set_int8  (Sstring *, Sint8);
STICKY_API void         S_string_set_int16 (Sstring *, Sint16);
STICKY_API void         S_string_set_int32 (Sstring *, Sint32);
STICKY_API void         S_string_set_int64 (Sstring *, Sint64);

STICKY_API void         S_string_concat(Sstring *, const Sstring *);
STICKY_API void         S_string_insert(Sstring *, const Sstring *, Ssize_t);
STICKY_API void         S_string_remove(Sstring *, Ssize_t, Ssize_t);
STICKY_API void         S_string_substring(Sstring *, const Sstring *, Ssize_t,
                                           Ssize_t);
STICKY_API void         S_string_trim(Sstring *);
STICKY_API void         S_string_upper(Sstring *);
STICKY_API void         S_string_lower(Sstring *);
STICKY_API void         S_string_reverse(Sstring *);
STICKY_API Sbool        S_string_startswith(const Sstring *, const Sstring *);
STICKY_API Sbool        S_string_endswith(const Sstring *, const Sstring *);
STICKY_API Sbool        S_string_find(const Sstring *, const Sstring *,
                                      Ssize_t *);
STICKY_API Sbool        S_string_findlast(const Sstring *, const Sstring *,
                                          Ssize_t *);

STICKY_API Sbool        S_string_equals(const Sstring *, const Sstring *);
STICKY_API Scomparator  S_string_compare(const Sstring *, const Sstring *);
STICKY_API Ssize_t      S_string_length(const Sstring *);
STICKY_API void         S_string_copy(Sstring *, const Sstring *);
STICKY_API Schar        S_string_charat(const Sstring *, Ssize_t);
STICKY_API Sbool        S_string_indexof(const Sstring *, Schar, Ssize_t *);
STICKY_API const Schar *S_string_ptr(const Sstring *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STRING_H */

