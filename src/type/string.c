/*
 * string.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/08/2026
 */

#include <ctype.h>
#include <string.h>

#include "type/string.h"

st_bool
st_string_empty(st_string_t str)
{
    return str.len == 0;
}

st_comparator
st_string_compare(st_string_t lhs,
                  st_string_t rhs)
{
    st_size min_len = lhs.len < rhs.len ? lhs.len : rhs.len;
    for (st_size i = 0; i < min_len; ++i)
    {
        if (lhs.data[i] > rhs.data[i])
            return 1;
        if (lhs.data[i] < rhs.data[i])
            return -1;
    }
    return lhs.len > rhs.len ? 1 : lhs.len < rhs.len ? -1 : 0;
}

st_bool
st_string_equal(st_string_t lhs,
                st_string_t rhs)
{
    if (lhs.len != rhs.len)
        return false;
    { // Assumption: lhs.len == rhs.len
        if (lhs.len == 0 || lhs.data == rhs.data)
            return true;
        return memcmp(lhs.data, rhs.data, lhs.len) == 0;
    }
}

st_bool
st_string_equal_ignore_case(st_string_t lhs,
                            st_string_t rhs)
{
    if (lhs.len != rhs.len)
        return false;
    { // Assumption: lhs.len == rhs.len
        for (st_index i = 0; i < lhs.len; ++i)
        {
            if (tolower(ST_CAST(int, lhs.data[i])) != tolower(ST_CAST(int, rhs.data[i])))
                return false;
        }
    }
    return true;
}

st_bool
st_string_starts_with(st_string_t str,
                      st_string_t prefix)
{
    if (str.len < prefix.len)
        return false;
    { // Assumption: prefix.len <= str.len
        return memcmp(str.data, prefix.data, prefix.len) == 0;
    }
}

st_bool
st_string_ends_with(st_string_t str,
                    st_string_t suffix)
{
    if (str.len < suffix.len)
        return false;
    { // Assumption: suffix.len <= str.len
        return memcmp(str.data + str.len - suffix.len, suffix.data, suffix.len) == 0;
    }
}

ST_DEPRECATED("Unimplemented")
st_bool
st_string_contains(ST_UNUSED st_string_t str,
                   ST_UNUSED st_string_t needle)
{
    return false;
}

st_bool
st_string_contains_char(st_string_t str,
                        st_char needle)
{
    for (st_index i = 0; i < str.len; ++i)
    {
        if (str.data[i] == needle)
            return true;
    }
    return false;
}

st_index
st_string_find(st_string_t str,
               st_string_t needle)
{
    if (needle.len == 0)
        return 0;
    if (needle.len > str.len)
        return ST_INDEX_NONE;
    { // Assumption: needle.len <= str.len
        for (st_index i = 0; i <= str.len - needle.len; ++i)
        {
            st_index j;
            for (j = 0; j < needle.len; ++j)
            {
                if (str.data[i + j] != needle.data[j])
                    break;
            }
            if (j == needle.len)
                return i;
        }
    }
    return ST_INDEX_NONE;
}

st_index
st_string_find_char(st_string_t str,
                    st_char needle)
{
    if (str.len == 0)
        return ST_INDEX_NONE;
    {
        // Assumption: str.len > 0
        for (st_index i = 0; i < str.len; ++i)
        {
            if (str.data[i] == needle)
                return i;
        }
    }
    return ST_INDEX_NONE;
}

st_index
st_string_find_last(st_string_t str,
                    st_string_t needle)
{
    if (needle.len == 0)
        return str.len;
    if (needle.len > str.len)
        return ST_INDEX_NONE;
    { // Assumption: needle.len <= str.len
        for (st_index i = str.len - needle.len + 1; i-- > 0;)
        {
            st_index j;
            for (j = 0; j < needle.len; ++j)
            {
                if (str.data[i + j] != needle.data[j])
                    break;
            }
            if (j == needle.len)
                return i;
        }
    }
    return ST_INDEX_NONE;
}

st_index
st_string_find_last_char(st_string_t str,
                         st_char needle)
{
    if (str.len == 0)
        return ST_INDEX_NONE;
    { // Assumption: str.len > 0
        for (st_index i = str.len; i-- > 0;)
        {
            if (str.data[i] == needle)
                return i;
        }
    }
    return ST_INDEX_NONE;
}

st_string_t
st_string_substring(st_string_t str,
                    st_index start,
                    st_size len)
{
    if (str.len == 0 || start >= str.len)
        return ST_STRING_EMPTY;
    if (len > str.len - ST_CAST(st_size, start))
        len = str.len - ST_CAST(st_size, start);
    return (st_string_t){ .data = str.data + start, .len = len };
}

st_string_t
st_string_left(st_string_t str,
               st_size len)
{
    if (str.len == 0 || len == 0)
        return ST_STRING_EMPTY;
    if (len >= str.len)
        return str;
    { // Assumption: str.len > 0 && 0 < len < str.len
        return (st_string_t){ .data = str.data, .len = len };
    }
}

st_string_t
st_string_right(st_string_t str,
                st_size len)
{
    if (str.len == 0 || len == 0)
        return ST_STRING_EMPTY;
    if (len >= str.len)
        return str;
    { // Assumption: str.len > 0 && 0 < len < str.len
        return (st_string_t){ .data = str.data + str.len - len, .len = len };
    }
}

st_string_t
st_string_trim(st_string_t str)
{
    if (str.len == 0)
        return ST_STRING_EMPTY;
    { // Assumption: str.len > 0
        st_index start = 0, end = str.len;
        while (start < end && (isspace(ST_CAST(int, str.data[start])) != 0 || str.data[start] == '\0'))
        {
            ++start;
        }
        while (end > start && (isspace(ST_CAST(int, str.data[end - 1])) != 0 || str.data[end - 1] == '\0'))
        {
            --end;
        }
        return st_string_substring(str, start, end - start);
    }
}

st_string_t
st_string_trim_left(st_string_t str)
{
    if (str.len == 0)
        return ST_STRING_EMPTY;
    { // Assumption: str.len > 0
        st_index i;
        for (i = 0; i < str.len; ++i)
        {
            if (isspace(ST_CAST(int, str.data[i])) == 0 && str.data[i] != '\0')
                break;
        }
        return st_string_right(str, str.len - i);
    }
}

st_string_t
st_string_trim_right(st_string_t str)
{
    if (str.len == 0)
        return ST_STRING_EMPTY;
    { // Assumption: str.len > 0
        st_index i;
        for (i = str.len; i > 0; --i)
        {
            if (isspace(ST_CAST(int, str.data[i - 1])) == 0 && str.data[i - 1] != '\0')
                break;
        }
        return st_string_substring(str, 0, i);
    }
}
