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

Sstring     *S_string_new(void);
Sstring     *S_string_load(const Schar *, Ssize_t);
void         S_string_delete(Sstring *);

void         S_string_concat(Sstring *, const Sstring *);

Sbool        S_string_equals(const Sstring *, const Sstring *);
Ssize_t      S_string_length(const Sstring *);
const Schar *S_string_ptr(const Sstring *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STRING_H */

