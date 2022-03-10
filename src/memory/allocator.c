/*
 * allocator.c
 * Custom memory allocator source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 05/09/2021
 */

#include <stdio.h>
#include <stdlib.h>

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/memory/allocator.h"
#include "sticky/memory/memtrace.h"

void *
_S_memory_new(Ssize_t size,
              const Schar *location,
              Suint32 line)
{
	void *ptr;
	if (size == 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_memory_new");
		return NULL;
	}
	ptr = malloc(size);
	if (!ptr)
		_S_out_of_memory(location, line);
#ifdef DEBUG
	_S_memtrace_add_frame(ptr, size, location, line);
#endif /* DEBUG */
	return ptr;
}

void *
_S_memory_resize(void *ptr,
                 Ssize_t size,
                 const Schar *location,
                 Suint32 line)
{
	void *newptr;
	if (size == 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_memory_resize");
		return NULL;
	}
	if (!ptr)
	{
		fprintf(stderr, "%s:%d: %s\n", location, line,
		        "cannot resize NULL pointer!");
		exit(EXIT_FAILURE);
	}
	else
	{
		newptr = realloc(ptr, size);
#ifdef DEBUG
		_S_memtrace_resize_frame(ptr, newptr, size, location, line);
#endif /* DEBUG */
		return newptr;
	}
}

void
_S_memory_delete(void *ptr,
                 const Schar *location,
                 Suint32 line)
{
	if (ptr)
	{
		free(ptr);
#ifdef DEBUG
		_S_memtrace_remove_frame(ptr, location, line);
#endif /* DEBUG */
	}
	else
	{
		fprintf(stderr, "%s:%d: %s\n", location, line,
		        "cannot delete NULL pointer!");
		exit(EXIT_FAILURE);
	}
}

void
_S_out_of_memory(const Schar *location,
                 Suint32 line)
{
	fprintf(stderr, "%s:%d: %s\n", location, line,
	        "could not allocate enough memory!");
	exit(EXIT_FAILURE);
}

