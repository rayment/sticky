/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * memtrace.c
 * Debug memory tracing tool source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 23/10/2021
 */

/*
 * WARNING: THIS IS TRUSTED CODE. AS SUCH, _S_CALL IS NOT TO BE USED.
 */

#include <stdio.h>
#include <stdlib.h>

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/memory/memtrace.h"

#ifdef DEBUG

#define MEMTRACE S_COLOR_BOLD S_COLOR_YELLOW "memtrace" S_COLOR_RESET ": "

#ifdef DEBUG_TRACE
#include "sticky/concurrency/mutex.h"

THREAD_LOCAL struct _S_memtrace_stack_frame_s *stack_frames;
struct _S_memtrace_memory_frame_s *mem_frames;
#endif /* DEBUG_TRACE */

static Ssize_t num_allocated_bytes, num_allocations, num_resizes, num_frees;
static Smutex trace_lock;

void
_S_memtrace_init(void)
{
	num_allocated_bytes = 0;
	num_allocations = 0;
	num_resizes = 0;
	num_frees = 0;
#ifdef DEBUG_TRACE
	stack_frames = NULL;
	mem_frames = NULL;
#endif /* DEBUG_TRACE */
	trace_lock = _S_mutex_new(S_FALSE);
}

void
_S_memtrace_init_thread(void)
{
#ifdef DEBUG_TRACE
	stack_frames = NULL;
#endif /* DEBUG_TRACE */
}

#ifdef DEBUG_TRACE
static
struct _S_memtrace_stack_frame_s *
_S_memtrace_push_stack(struct _S_memtrace_stack_frame_s *parent,
                       const Schar *callee,
                       const Schar *location,
                       Suint32 line)
{
	struct _S_memtrace_stack_frame_s *frame;

#if DEBUG_TRACE > 1
	fprintf(stderr, MEMTRACE "%s called at %s:%d\n",
	        callee, location, line);
#endif /* DEBUG_TRACE > 1 */
	frame = (struct _S_memtrace_stack_frame_s *)
		malloc(sizeof(struct _S_memtrace_stack_frame_s));
	if (!frame)
	{
		fprintf(stderr, MEMTRACE "failed to allocate stack trace frame\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		/*fprintf(stdout, MEMTRACE "entered frame (%s) from %s:%d\n",
		        callee, location, line);*/
		frame->callee = callee;
		frame->location = location;
		frame->line = line;
		frame->next = parent;
	}
	return frame;
}

static
struct
_S_memtrace_stack_frame_s *
_S_memtrace_pop_stack(struct _S_memtrace_stack_frame_s *frame)
{
	struct _S_memtrace_stack_frame_s *next;

	if (!frame)
		return NULL;

	next = frame->next;
	free(frame);
	/*fprintf(stdout, MEMTRACE "exited frame (%s) from %s:%d\n",
	        frame->callee, frame->location, frame->line);*/
	return next;
}

static
void
_S_memtrace_popall_stack(struct _S_memtrace_stack_frame_s *frame)
{
	struct _S_memtrace_stack_frame_s *tmp;
	while (frame)
	{
		tmp = frame;
		frame = frame->next;
		free(tmp);
	}
}

static
struct
_S_memtrace_stack_frame_s *
_S_memtrace_copy_stack_frame(struct _S_memtrace_stack_frame_s *frame)
{
	struct _S_memtrace_stack_frame_s *root;
	if (!frame)
		return NULL;
	root = (struct _S_memtrace_stack_frame_s *)
	       malloc(sizeof(struct _S_memtrace_stack_frame_s));
	if (!root)
		return NULL; /* don't throw error? just go with it? */
	root->next = NULL;
	root->callee = frame->callee;
	root->location = frame->location;
	root->line = frame->line;
	return root;
}

static
struct
_S_memtrace_stack_frame_s *
_S_memtrace_copy_stack(struct _S_memtrace_stack_frame_s *frame)
{
	struct _S_memtrace_stack_frame_s *root, *tmp, *tmp2, *prev;
	if (!stack_frames)
		return NULL;
	root = NULL;
	tmp = frame;
	tmp2 = NULL;
	prev = NULL;
	while (tmp)
	{
		tmp2 = _S_memtrace_copy_stack_frame(tmp);
		if (!tmp2)
			break; /* don't throw error? just go with it? */
		if (prev)
			prev->next = tmp2;
		else
			root = tmp2;
		prev = tmp2;
		tmp = tmp->next;
	}
	return root;
}

static
void
_S_memtrace_stack_trace(struct _S_memtrace_stack_frame_s *frame,
                        Sbool freestand)
{
	if (!frame)
	{
		if (freestand)
			fprintf(stderr, MEMTRACE "no stack trace available\n");
		return;
	}

	if (freestand)
		fprintf(stderr, MEMTRACE "stack trace:\n");
	while (frame)

	{
		if (freestand)
			fprintf(stderr, "  %s called from %s on line %d\n",
		            frame->callee, frame->location, frame->line);
		else
			fprintf(stderr, "      ... %s called from %s on line %d\n",
		            frame->callee, frame->location, frame->line);
		frame = frame->next;
	}
}

void
_S_memtrace_push_stack_local(const Schar *callee,
                             const Schar *location,
                             Suint32 line)
{
	stack_frames = _S_memtrace_push_stack(stack_frames, callee, location, line);
}

void
_S_memtrace_pop_stack_local(void)
{
	stack_frames = _S_memtrace_pop_stack(stack_frames);
}

void
_S_memtrace_stack_trace_local(Sbool freestand)
{
	_S_memtrace_stack_trace(stack_frames, freestand);
}
#endif /* DEBUG_TRACE */

void
_S_memtrace_add_frame(const void *ptr,
                      Ssize_t size,
                      const Schar *location,
                      Suint32 line)
{
#ifdef DEBUG_TRACE
	struct _S_memtrace_memory_frame_s *frame;

	frame = (struct _S_memtrace_memory_frame_s *)
		malloc(sizeof(struct _S_memtrace_memory_frame_s));
	if (!frame)
	{
		fprintf(stderr, MEMTRACE "failed to allocate memory trace frame\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		S_mutex_lock(trace_lock);
		++num_allocations;
		num_allocated_bytes += size;
		fprintf(stdout, MEMTRACE "alloc'd (%p) %ldb at %s:%d\n",
		        ptr, size, location, line);
		frame->ptr = ptr;
		frame->size = size;
		frame->location = location;
		frame->line = line;
		frame->next = mem_frames;
		frame->call = _S_memtrace_copy_stack(stack_frames);
		mem_frames = frame;
		S_mutex_unlock(trace_lock);
	}

#else /* DEBUG_TRACE */
	S_mutex_lock(trace_lock);
	++num_allocations;
	num_allocated_bytes += size;
	S_mutex_unlock(trace_lock);
	(void) ptr;
	(void) location;
	(void) line;
#endif /* DEBUG_TRACE */
}

void
_S_memtrace_resize_frame(const void *ptrold,
                         const void *ptrnew,
                         Ssize_t size,
                         const Schar *location,
                         Suint32 line)
{
#ifdef DEBUG_TRACE
	struct _S_memtrace_memory_frame_s *frame;

	S_mutex_lock(trace_lock);

	frame = mem_frames;
	while (frame)
	{
		if (frame->ptr == ptrold)
		{
			fprintf(stdout,
			        MEMTRACE "resized (%p -> %p) %ldb -> %ldb at %s:%d\n",
			        ptrold, ptrnew, frame->size, size, location, line);
			frame->ptr = ptrnew;
			frame->size = size;
			++num_resizes;
			S_mutex_unlock(trace_lock);
			return;
		}
		frame = frame->next;
	}
	/* no frame found, don't bother unlocking the mutex and raise an error */
	fprintf(stdout,
	        MEMTRACE "tried resize on unregistered block (%p) at %s:%d\n",
	        ptrold, location, line);
	exit(EXIT_FAILURE);
#else /* DEBUG_TRACE */
	/* if tracing is not enabled then its not possible to update the allocated
       number of bytes because there's no way to trace the original size */
	S_mutex_lock(trace_lock);
	++num_resizes;
	S_mutex_unlock(trace_lock);
	(void) ptrold;
	(void) ptrnew;
	(void) size;
	(void) location;
	(void) line;
#endif /* DEBUG_TRACE */
}

void
_S_memtrace_remove_frame(const void *ptr,
                         const Schar *location,
                         Suint32 line)
{
#ifdef DEBUG_TRACE
	struct _S_memtrace_memory_frame_s *frame, *tmpframe;

	S_mutex_lock(trace_lock);

	frame = mem_frames;
	tmpframe = NULL;
	while (frame)
	{
		if (frame->ptr == ptr)
		{
			if (tmpframe)
				tmpframe->next = frame->next;
			else
				mem_frames = frame->next;
			fprintf(stdout, MEMTRACE "free'd (%p) %ldb at %s:%d\n",
			        ptr, frame->size, location, line);
			++num_frees;
			if (frame->call)
			{
				/* free the associated call stack if one exists */
				_S_memtrace_popall_stack(frame->call);
			}
			free(frame);
			S_mutex_unlock(trace_lock);
			return;
		}
		tmpframe = frame;
		frame = frame->next;
	}
	/* no frame found, don't bother unlocking the mutex and raise an error */
	fprintf(stdout, MEMTRACE "tried free on unregistered block (%p) at %s:%d\n",
	        ptr, location, line);
	exit(EXIT_FAILURE);
#else /* DEBUG_TRACE */
	if (++num_frees > num_allocations)
	{
		fprintf(stdout,
		        MEMTRACE "tried to free (%p) at %s:%d, exceeding alloc's\n",
		        ptr, location, line);
		exit(EXIT_FAILURE);
	}
#endif /* DEBUG_TRACE */
}

Sbool
_S_memtrace_all_free(void)
{
	return num_allocations == num_frees;
}

void
_S_memtrace_free_thread(void)
{
	if (stack_frames)
	{
		fprintf(stdout,
		        MEMTRACE "unexpected stack frame at end of thread\n");
		_S_memtrace_stack_trace(stack_frames, S_TRUE);
		_S_memtrace_popall_stack(stack_frames);
		stack_frames = NULL;
	}
}

void
_S_memtrace_free(void)
{
#ifdef DEBUG_TRACE
	struct _S_memtrace_memory_frame_s *frame, *tmpframe;
#endif /* DEBUG_TRACE */

	/* wait until we can acquire a lock, then release it for disposal */
	S_mutex_lock(trace_lock);
	S_mutex_unlock(trace_lock);
	_S_mutex_delete(trace_lock, S_FALSE);

	fprintf(stdout,
	        S_COLOR_BOLD
	        "=============================================="
	        S_COLOR_RESET "\n");
	fprintf(stdout,
	        S_COLOR_YELLOW
	        "               MEMORY ALLOCATOR               "
	        S_COLOR_RESET "\n");
	fprintf(stdout,
	        S_COLOR_BOLD
	        "=============================================="
	        S_COLOR_RESET "\n");
	/* statistical readout */
	if (num_allocated_bytes != 0)
	{
		fprintf(stdout, "  Num. bytes allocated: %ld\n", num_allocated_bytes);
		fprintf(stdout, "    in %ld allocations\n", num_allocations);
		if (num_resizes > 0)
			fprintf(stdout, "    of which %ld were resized\n", num_resizes);
		fprintf(stdout, "    of which %ld were free'd.\n", num_frees);
		if (num_allocations != num_frees)
			fprintf(stdout,
			        S_COLOR_RED "  Memory leak detected!\n" S_COLOR_RESET);
	}
	else
	{
		fprintf(stdout, "  No allocations were made.\n");
	}
#ifdef DEBUG_TRACE
	frame = mem_frames;
	if (frame)
	{
		fprintf(stdout, "\n  "
		        S_COLOR_RED "The following allocations were not free'd"
		        S_COLOR_RESET ":\n");
		while (frame)
		{
			fprintf(stdout, "    (%p) %ldb at %s:%d\n",
			        frame->ptr, frame->size, frame->location, frame->line);
			if (frame->call)
			{
				_S_memtrace_stack_trace(frame->call, S_FALSE);
				_S_memtrace_popall_stack(frame->call);
			}
			tmpframe = frame;
			frame = frame->next;
			/* free(tmpframe->ptr); */
			free(tmpframe);
		}
	}
#endif /* DEBUG_TRACE */
	fprintf(stdout,
	        S_COLOR_BOLD
	        "=============================================="
	        S_COLOR_RESET "\n");
}

#endif /* DEBUG */

