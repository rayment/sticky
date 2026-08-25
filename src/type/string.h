/*
 * string.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/08/2026
 */

#ifndef STICKY_TYPE_STRING_H
#define STICKY_TYPE_STRING_H

#include <string.h>

#include "st_macros.h"
#include "st_primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

// PRIVATE
#ifdef __cplusplus
#define ST_PRIV_STRING_LIT(d, l) (st_string_t{ (d), (l) })
#else
#define ST_PRIV_STRING_LIT(d, l) ((st_string_t){ .data = (d), .len = (l) })
#endif
#define ST_PRIV_STRING_1(x) \
    (((void)sizeof(char[(ST_IS_ARRAY_OF_TYPE(char, x)) ? 1 : -1])), \
    ST_PRIV_STRING_LIT((x), ST_SIZEOF(x) - 1))
#define ST_PRIV_STRING_2(ptr, size) \
    (((void)sizeof(char[(ST_IS_PTR_OR_ARRAY_OF_TYPE(char, ptr)) ? 1 : -1])), \
    ST_PRIV_STRING_LIT((ptr), (size)))
#define ST_PRIV_STRING_VAARG(_1, _2, NAME, ...) NAME

/**
 * This macro creates an st_string_t struct from a fixed-length character array,
 *   or from a pointer to a character array with a known length.
 *
 * The macro accepts either one or two arguments. Argument signatures are
 *   `ST_STRING(arr)` or `ST_STRING(ptr, len)`, where `arr` is a fixed-length
 *   character array, and `ptr` is a pointer to a character array with a known
 *   length `len`.
 *
 * Where a fixed-length character array is supplied without a specific length,
 *   the length is inferred from the size of the array and subtracted by one, as
 *   these strings are typically static and null-terminated. If you wish to include
 *   the null terminator in the string, you must explicitly specify the length with
 *   the second `len` parameter.
 *
 * Null arrays and pointers are explicitly forbidden and must be checked
 *   before calling this macro. Compile-time protection will prevent null
 *   pointers and arrays but only if the type is not a deriviative of `st_char`.
 *
 * @param arr  Fixed-length character array. Mutually exclusive with `ptr` and `len`.
 * @param ptr  Pointer to a character array. Mutually exclusive with `arr`.
 * @param len  Length of the character array pointed to by `ptr`.
 * @returns The created st_string_t struct.
 */
#define ST_STRING(...) \
    ST_PRIV_STRING_VAARG( \
        __VA_ARGS__, \
        ST_PRIV_STRING_2, \
        ST_PRIV_STRING_1, \
    )(__VA_ARGS__)

#ifdef __cplusplus
#define ST_STRING_EMPTY ST_PRIV_STRING_LIT(static_cast<const char *>(nullptr), 0)
#else
#define ST_STRING_EMPTY ST_PRIV_STRING_LIT(nullptr, 0)
#endif

#define ST_STRING_SAFE_CSTR(str, varname) \
    st_char varname[(str).len + 1]; \
    memcpy(varname, (str).data, (str).len); \
    varname[(str).len] = '\0';

#define ST_STRING_SAFE_CONCAT(a, b, varname) \
    st_char st_string_intermediate_##varname[(a).len + (b).len + 1]; \
    memcpy(st_string_intermediate_##varname, (a).data, (a).len); \
    memcpy(st_string_intermediate_##varname + (a).len, (b).data, (b).len); \
    st_string_intermediate_##varname[(a).len + (b).len] = '\0'; \
    st_string_t varname = ST_STRING(st_string_intermediate_##varname);

#define ST_STRING_SAFE_CONCAT_CSTR(a, b, varname) \
    st_char varname[(a).len + (b).len + 1]; \
    memcpy(varname, (a).data, (a).len); \
    memcpy(varname + (a).len, (b).data, (b).len); \
    varname[(a).len + (b).len] = '\0';

typedef struct st_string
{
    const st_char *data;
          st_size  len;
} st_string_t;

st_comparator  st_string_compare          (st_string_t lhs, st_string_t rhs);
st_bool        st_string_contains         (st_string_t str, st_string_t needle);
st_bool        st_string_contains_char    (st_string_t str, st_char needle);
st_bool        st_string_empty            (st_string_t str);
st_bool        st_string_ends_with        (st_string_t str, st_string_t suffix);
st_bool        st_string_equal            (st_string_t lhs, st_string_t rhs);
st_bool        st_string_equal_ignore_case(st_string_t lhs, st_string_t rhs);
st_index       st_string_find             (st_string_t str, st_string_t needle);
st_index       st_string_find_char        (st_string_t str, st_char needle);
st_index       st_string_find_last        (st_string_t str, st_string_t needle);
st_index       st_string_find_last_char   (st_string_t str, st_char needle);
st_string_t    st_string_left             (st_string_t str, st_size len);
st_string_t    st_string_right            (st_string_t str, st_size len);
st_bool        st_string_starts_with      (st_string_t str, st_string_t prefix);
st_string_t    st_string_substring        (st_string_t str, st_index start, st_size len);
st_string_t    st_string_trim             (st_string_t str);
st_string_t    st_string_trim_left        (st_string_t str);
st_string_t    st_string_trim_right       (st_string_t str);

#ifdef __cplusplus
}
#endif

#endif // STICKY_TYPE_STRING_H
