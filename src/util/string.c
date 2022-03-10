/*
 * string.c
 * Character string source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 10/03/2022
 */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/memory/allocator.h"
#include "sticky/util/string.h"

#define DEFAULT_LEN  64 /* default string length upon allocation */
#define ALIGNMENT    64 /* number of bytes to align each allocation by */
#define STR_ALIGN(x) ((x)%ALIGNMENT == 0 ? (x) : ((x)-(x)%ALIGNMENT)+ALIGNMENT)

static inline
void
_S_string_resize(Sstring *str,
                 Ssize_t len)
{
	len = STR_ALIGN(len);
	if (len <= str->ptrlen)
		return;
	str->ptrlen = len;
	str->ptr = (Schar *) S_memory_resize(str->ptr, sizeof(Schar) * str->ptrlen);
}

static inline
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
		_S_CALL("_S_string_resize", _S_string_resize(str, len));
	}
	else /* double */
	{
		_S_CALL("_S_string_resize", _S_string_resize(str, str->ptrlen*2));
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

