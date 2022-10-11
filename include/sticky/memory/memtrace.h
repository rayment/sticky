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

#include <stdlib.h>

#include "sticky/common/types.h"

#ifdef DEBUG_TRACE
struct
_S_memtrace_stack_frame_s
{
	struct _S_memtrace_stack_frame_s *next;
	const Schar *callee;
	const Schar *location;
	Suint32 line;
};

struct
_S_memtrace_memory_frame_s
{
	struct _S_memtrace_memory_frame_s *next;
	struct _S_memtrace_stack_frame_s *call;
	const void *ptr;
	const Schar *location;
	Ssize_t size;
	Suint32 line;
};

STICKY_API void _S_memtrace_push_stack_local(const Schar *, const Schar *,
                                             Suint32);
STICKY_API void _S_memtrace_pop_stack_local(void);
STICKY_API void _S_memtrace_stack_trace_local(Sbool);

#define _S_CALL(name, call) \
	_S_memtrace_push_stack_local(name, __FILE__, __LINE__); \
	call;                                                   \
	_S_memtrace_pop_stack_local()
#else /* DEBUG_TRACE */
#define _S_CALL(name, call) call
#endif /* DEBUG_TRACE */

void    _S_memtrace_init(void);
void    _S_memtrace_init_thread(void);
void    _S_memtrace_free(void);
void    _S_memtrace_free_thread(void);
Sbool   _S_memtrace_all_free(void);

void    _S_memtrace_add_frame(const void *, Ssize_t,
                              const Schar *, Suint32);
void    _S_memtrace_resize_frame(const void *, const void *, Ssize_t,
                                 const Schar *, Suint32);
void    _S_memtrace_remove_frame(const void *, const Schar *, Suint32);

#else /* DEBUG */

#define _S_CALL(name, call) call

#endif /* DEBUG */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MEMTRACE_H */

