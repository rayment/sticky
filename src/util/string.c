/*
 * string.c
 * Character string source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 10/03/2022
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/math.h"
#include "sticky/memory/allocator.h"
#include "sticky/util/string.h"

#define DEFAULT_LEN  64 /* default string length upon allocation */
#define ALIGNMENT    64 /* number of bytes to align each allocation by */
#define STR_ALIGN(x) ((x)%ALIGNMENT == 0 ? (x) : ((x)-(x)%ALIGNMENT)+ALIGNMENT)

static
void
_S_string_grow(Sstring *str,
               Ssize_t len)
{
	/* strategy: attempt to double size. if length still greater, just align to
	             the length instead */
	if (len <= str->ptrlen) /* no need to resize */
		return;
	if (len > str->ptrlen*2) /* manually resize */
	{
		_S_CALL("S_string_reserve", S_string_reserve(str, len));
	}
	else /* double */
	{
		_S_CALL("S_string_reserve", S_string_reserve(str, str->ptrlen*2));
	}
}

static inline
Sstring *
_S_string_new(Ssize_t len)
{
	Sstring *str;
	str = (Sstring *) S_memory_new(sizeof(Sstring));
	str->len = 0;
	if (len == 0)
		str->ptrlen = DEFAULT_LEN;
	else
		str->ptrlen = STR_ALIGN(len);
	str->ptr = (Schar *) S_memory_new(sizeof(Schar) * str->ptrlen);
	return str;
}

Sstring *
S_string_new(void)
{
	Sstring *str;
	_S_CALL("_S_string_new", str = _S_string_new(0));
	return str;
}

Sstring *
S_string_load(const Schar *buf,
              Ssize_t len)
{
	Sstring *str;
	if (!buf || len == 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_load");
		return NULL;
	}
	_S_CALL("_S_string_new", str = _S_string_new(len+1));
	memcpy(str->ptr, buf, len);
	*(str->ptr+len) = '\0';
	str->len = len;
	return str;
}

void
S_string_delete(Sstring *str)
{
	if (!str)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_delete");
		return;
	}
	S_memory_delete(str->ptr);
	S_memory_delete(str);
}

void
S_string_reserve(Sstring *str,
                 Ssize_t len)
{
	if (!str)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_reserve");
		return;
	}
	len = STR_ALIGN(len);
	if (len <= str->ptrlen)
		return;
	str->ptrlen = len;
	str->ptr = (Schar *) S_memory_resize(str->ptr, sizeof(Schar) * str->ptrlen);
}

void
S_string_set(Sstring *dest,
             const Schar *buf,
             Ssize_t len)
{
	if (!dest || !buf || len == 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set");
		return;
	}
	_S_CALL("_S_string_grow", _S_string_grow(dest, len+1));
	memcpy(dest->ptr, buf, len);
	dest->len = len;
	*(dest->ptr+dest->len) = '\0';
}

void
S_string_set_format(Sstring *dest,
                    const Schar *format,
                    ...)
{
	Ssize_t len;
	va_list va;
	va_start(va, format);
	len = vsnprintf(dest->ptr, 0, format, va);
	_S_CALL("_S_string_grow", _S_string_grow(dest, len+1));
	dest->len = len;
	va_end(va);
	va_start(va, format);
	vsnprintf(dest->ptr, len+1, format, va);
	va_end(va);
	*(dest->ptr+dest->len) = '\0';
}

void
S_string_set_bool(Sstring *dest,
                  Sbool val)
{
	if (!dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set_bool");
		return;
	}
	if (val)
	{
		_S_CALL("S_string_set", S_string_set(dest, "true", 4));
	}
	else
	{
		_S_CALL("S_string_set", S_string_set(dest, "false", 5));
	}
}

void
S_string_set_float(Sstring *dest,
                   Sfloat val,
                   Suint8 precision)
{
	if (!dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set_float");
		return;
	}
	_S_CALL("S_string_set_format",
	        S_string_set_format(dest, "%.*f", precision, val));
}

void
S_string_set_double(Sstring *dest,
                    Sdouble val,
                    Suint8 precision)
{
	if (!dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set_double");
		return;
	}
	_S_CALL("S_string_set_format",
	        S_string_set_format(dest, "%.*f", precision, val));
}

void
S_string_set_uint8(Sstring *dest,
                   Suint8 val)
{
	if (!dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set_uint8");
		return;
	}
	_S_CALL("S_string_set_format",
	        S_string_set_format(dest, "%u", val));
}

void
S_string_set_uint16(Sstring *dest,
                    Suint16 val)
{
	if (!dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set_uint16");
		return;
	}
	_S_CALL("S_string_set_format",
	        S_string_set_format(dest, "%u", val));
}

void
S_string_set_uint32(Sstring *dest,
                    Suint32 val)
{
	if (!dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set_uint32");
		return;
	}
	_S_CALL("S_string_set_format",
	        S_string_set_format(dest, "%u", val));
}

void
S_string_set_uint64(Sstring *dest,
                    Suint64 val)
{
	if (!dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set_uint64");
		return;
	}
	_S_CALL("S_string_set_format",
	        S_string_set_format(dest, "%lu", val));
}

void
S_string_set_int8(Sstring *dest,
                  Sint8 val)
{
	if (!dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set_int8");
		return;
	}
	_S_CALL("S_string_set_format",
	        S_string_set_format(dest, "%d", val));
}

void
S_string_set_int16(Sstring *dest,
                   Sint16 val)
{
	if (!dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set_int16");
		return;
	}
	_S_CALL("S_string_set_format",
	        S_string_set_format(dest, "%d", val));
}

void
S_string_set_int32(Sstring *dest,
                   Sint32 val)
{
	if (!dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set_int32");
		return;
	}
	_S_CALL("S_string_set_format",
	        S_string_set_format(dest, "%d", val));
}

void
S_string_set_int64(Sstring *dest,
                   Sint64 val)
{
	if (!dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_set_int64");
		return;
	}
	_S_CALL("S_string_set_format",
	        S_string_set_format(dest, "%ld", val));
}

void
S_string_concat(Sstring *dest,
                const Sstring *src)
{
	Ssize_t origlen;
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_concat");
		return;
	}
	origlen = dest->len;
	_S_CALL("_S_string_grow", _S_string_grow(dest, dest->len+src->len+1));
	memcpy(dest->ptr+origlen, src->ptr, src->len);
	dest->len += src->len;
	*(dest->ptr+dest->len) = '\0';
}

void
S_string_substring(Sstring *dest,
                   const Sstring *src,
                   Ssize_t start,
                   Ssize_t len)
{
	if (!dest || !src || len == 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_substring");
		return;
	}
	else if (start+len >= src->len)
	{
		_S_SET_ERROR(S_INVALID_INDEX, "S_string_substring");
		return;
	}
	if (dest != src)
		_S_CALL("_S_string_grow", _S_string_grow(dest, len+1));
	if (dest != src || start != 0) /* no need to move if shrinking src string */
		memmove(dest->ptr, src->ptr+start, len);
	dest->len = len;
	*(dest->ptr+dest->len) = '\0';
}

void
S_string_trim(Sstring *str)
{
	Ssize_t i, start, len, tmplen;
	Schar c;
	if (!str)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_trim");
		return;
	}
	for (i = start = len = tmplen = 0; i < str->len; ++i)
	{
		c = *(str->ptr+i);
		if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f')
		{
			++tmplen;
			if (len == 0)
				++start;
		}
		else
		{
			if (len != 0)
				len += tmplen;
			++len;
			tmplen = 0;
		}
	}
	if (start != 0 && len != str->len)
	{
		_S_CALL("S_string_substring",
		        S_string_substring(str, str, start, len));
	}
}

void
S_string_upper(Sstring *str)
{
	Ssize_t i;
	if (!str)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_upper");
		return;
	}
	for (i = 0; i < str->len; ++i)
	{
		if (*(str->ptr+i) >= 97 && *(str->ptr+i) <= 122)
			*(str->ptr+i) -= 32; /* ascii lower to upper offset */
	}
}

void
S_string_lower(Sstring *str)
{
	Ssize_t i;
	if (!str)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_lower");
		return;
	}
	for (i = 0; i < str->len; ++i)
	{
		if (*(str->ptr+i) >= 65 && *(str->ptr+i) <= 90)
			*(str->ptr+i) += 32; /* ascii upper to lower offset */
	}
}

Sbool
S_string_startswith(const Sstring *str,
                    const Sstring *prefix)
{
	Ssize_t i;
	if (!str || !prefix)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_startswith");
		return S_FALSE;
	}
	if (str->len < prefix->len)
		return S_FALSE; /* prefix can't be a substring if its larger */
	for (i = 0; i < prefix->len; ++i)
	{
		if (*(str->ptr+i) != *(prefix->ptr+i))
			return S_FALSE;
	}
	return S_TRUE;
}

Sbool
S_string_endswith(const Sstring *str,
                  const Sstring *suffix)
{
	Ssize_t i;
	if (!str || !suffix)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_endswith");
		return S_FALSE;
	}
	if (str->len < suffix->len)
		return S_FALSE; /* suffix can't be a substring if its larger */
	for (i = 0; i < suffix->len; ++i)
	{
		if (*(str->ptr+str->len-suffix->len+i) != *(suffix->ptr+i))
			return S_FALSE;
	}
	return S_TRUE;
}

/*
 * See:
 *
 * Fast Pattern Matching in Strings,
 * Donald E. Knuth, James H. Morris Jr., Vaughan R. Pratt
 * Siam J. Comput. Vol. 6, No. 2, June 1977
 */
Sbool
S_string_find(const Sstring *haystack,
              const Sstring *needle,
              Ssize_t *idx)
{
	Sbool b;
	Ssize_t i, s, t, *fail;
	if (!haystack || !needle || !idx)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_find");
		return S_FALSE;
	}
	if (haystack->len < needle->len)
	{
		return S_FALSE;
	}
	else if (haystack->len == needle->len)
	{
		_S_CALL("S_string_equals", b = S_string_equals(haystack, needle));
		return b;
	}
	/* generate the failure function */
	fail = S_memory_new(sizeof(Ssize_t) * needle->len);
	*(fail) = 0;
	for (s = 1, t = 0; s < needle->len; ++s)
	{
		while (t > 0 && *(needle->ptr+s) != *(needle->ptr+t))
			t = *(fail+t-1);
		if (*(needle->ptr+s) == *(needle->ptr+t))
			++t;
		*(fail+s) = t;
	}
	/* find the substring */
	s = i = 0;
	while (i < haystack->len && s < needle->len)
	{
		if (*(haystack->ptr+i) == *(needle->ptr+s))
		{
			++s;
			++i;
		}
		else if (s == 0)
		{
			++i;
		}
		else
		{
			s = *(fail+s-1);
		}
	}
	S_memory_delete(fail);
	if (s < needle->len)
		return S_FALSE;
	*idx = i - needle->len;
	return S_TRUE;
}

Sbool
S_string_equals(const Sstring *a,
                const Sstring *b)
{
	Ssize_t i;
	if ((!a && !b) || (a == b))
		return S_TRUE;
	else if (!a || !b || a->len != b->len)
		return S_FALSE;
	for (i = 0; i < a->len; ++i)
	{
		if (*(a->ptr+i) != *(b->ptr+i))
			return S_FALSE;
	}
	return S_TRUE;
}

Scomparator
S_string_compare(const Sstring *a,
                 const Sstring *b)
{
	Ssize_t i;
	if (!a || !b)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_compare");
		return -1;
	}
	if (a->len < b->len)
		return -1;
	else if (b->len < a->len)
		return 1;
	for (i = 0; i < a->len; ++i)
	{
		if (*(a->ptr+i) < *(b->ptr+i))
			return -1;
		else if (*(b->ptr+i) < *(a->ptr+i))
			return 1;
	}
	return 0;
}

Ssize_t
S_string_length(const Sstring *str)
{
	if (!str)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_length");
		return 0;
	}
	return str->len;
}

void
S_string_copy(Sstring *dest,
              const Sstring *src)
{
	if (!dest || !src)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_copy");
		return;
	}
	if (dest == src)
		return;
	_S_CALL("_S_string_grow", _S_string_grow(dest, src->len+1));
	memcpy(dest->ptr, src->ptr, src->len);
	dest->len = src->len;
	*(dest->ptr+dest->len) = '\0';
}

Schar
S_string_charat(const Sstring *str,
                Ssize_t idx)
{
	if (!str)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_charat");
		return '\0';
	}
	else if (idx >= str->len)
	{
		_S_SET_ERROR(S_INVALID_INDEX, "S_string_charat");
		return '\0';
	}
	return *(str->ptr+idx);
}

Sbool
S_string_indexof(const Sstring *str,
                 Schar c,
                 Ssize_t *idx)
{
	Ssize_t i;
	if (!str || !idx)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_indexof");
		return S_FALSE;
	}
	for (i = 0; i < str->len; ++i)
	{
		if (*(str->ptr+i) == c)
		{
			*idx = i;
			return S_TRUE;
		}
	}
	return S_FALSE;
}

const Schar *
S_string_ptr(const Sstring *str)
{
	if (!str)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_string_ptr");
		return NULL;
	}
	return str->ptr;
}

