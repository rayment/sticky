/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * memtrace.h
 * Debug memory tracing utility header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 23/10/2021
 */

#ifndef FR_RAYMENT_STICKY_MEMTRACE_H
#define FR_RAYMENT_STICKY_MEMTRACE_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifdef DEBUG

#include "sticky/common/types.h"

#ifdef DEBUG_TRACE
struct
_S_memtrace_memory_frame_s
{
	struct _S_memtrace_memory_frame_s *next;
	const void *ptr;
	const Schar *location;
	Ssize_t size;
	Suint32 line;
};

struct
_S_memtrace_stack_frame_s
{
	struct _S_memtrace_stack_frame_s *next;
	const Schar *callee;
	const Schar *location;
	Suint32 line;
};

void    _S_memtrace_push_stack(const Schar *, const Schar *, const Suint32);
struct  _S_memtrace_stack_frame_s *
        _S_memtrace_pop_stack(void);
void    _S_memtrace_stack_trace(void);

#define _S_CALL(name, call) \
	_S_memtrace_push_stack(name, __FILE__, __LINE__); \
	call;                                             \
	_S_memtrace_pop_stack()
#else /* DEBUG_TRACE */
#define _S_CALL(name, call) call
#endif /* DEBUG_TRACE */

void    _S_memtrace_init(void);
void    _S_memtrace_free(void);

void    _S_memtrace_add_frame(const void *, const Ssize_t,
                              const Schar *, const Suint32);
void    _S_memtrace_remove_frame(const void *, const Schar *, const Suint32);

#endif /* DEBUG */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MEMTRACE_H */

