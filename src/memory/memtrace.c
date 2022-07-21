/*
 * memtrace.c
 * Debug memory tracing tool source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 23/10/2021
 */

#include <stdio.h>
#include <stdlib.h>

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/memory/memtrace.h"

#ifdef DEBUG

#define MEMTRACE S_COLOR_BOLD S_COLOR_YELLOW "memtrace" S_COLOR_RESET ": "

#ifdef DEBUG_TRACE
struct _S_memtrace_stack_frame_s *stack_frames;
struct _S_memtrace_memory_frame_s *mem_frames;
static Ssize_t stack_depth;
#endif /* DEBUG_TRACE */

static Ssize_t num_allocated_bytes, num_allocations, num_resizes, num_frees;

void
_S_memtrace_init(void)
{
	num_allocated_bytes = 0;
	num_allocations = 0;
	num_resizes = 0;
	num_frees = 0;
#ifdef DEBUG_TRACE
	stack_depth = 0;
	stack_frames = NULL;
	mem_frames = NULL;
#endif /* DEBUG_TRACE */
}

#ifdef DEBUG_TRACE
void
_S_memtrace_push_stack(const Schar *callee,
                       const Schar *location,
                       Suint32 line)
{
	struct _S_memtrace_stack_frame_s *frame;

#if defined(DEBUG_TRACE) && DEBUG_TRACE > 1
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
		++stack_depth;
		frame->callee = callee;
		frame->location = location;
		frame->line = line;
		frame->next = stack_frames;
		stack_frames = frame;
	}
}

struct
_S_memtrace_stack_frame_s *
_S_memtrace_pop_stack(void)
{
	struct _S_memtrace_stack_frame_s *frame;

	if (stack_depth == 0)
		return NULL;

	frame = stack_frames;
	if (frame)
		stack_frames = frame->next;
	/*fprintf(stdout, MEMTRACE "exited frame (%s) from %s:%d\n",
	        frame->callee, frame->location, frame->line);*/

	--stack_depth;
	return frame; /* WARNING: NOT DEALLOCATED, DO IT YOURSELF */
}

void
_S_memtrace_stack_trace(void)
{
	struct _S_memtrace_stack_frame_s *frame;

	if (stack_depth == 0)
	{
		fprintf(stderr, MEMTRACE "no stack trace available\n");
		return;
	}

	fprintf(stderr, MEMTRACE "stack trace (depth: %ld):\n",
	        stack_depth);
	while (stack_depth > 0)
	{
		frame = _S_memtrace_pop_stack();
		fprintf(stderr, "  %s called from %s on line %d\n",
		        frame->callee, frame->location, frame->line);
		free(frame);
	}
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
		++num_allocations;
		num_allocated_bytes += size;
		fprintf(stdout, MEMTRACE "alloc'd (%p) %ldb at %s:%d\n",
		        ptr, size, location, line);
		frame->ptr = ptr;
		frame->size = size;
		frame->location = location;
		frame->line = line;
		frame->next = mem_frames;
		mem_frames = frame;
	}
#else /* DEBUG_TRACE */
	++num_allocations;
	num_allocated_bytes += size;
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
			return;
		}
		frame = frame->next;
	}
	/* no frame found, raise an error */
	fprintf(stdout,
	        MEMTRACE "tried resize on unregistered block (%p) at %s:%d\n",
	        ptrold, location, line);
	exit(EXIT_FAILURE);
#else /* DEBUG_TRACE */
	/* if tracing is not enabled then its not possible to update the allocated
       number of bytes because there's no way to trace the original size */
	++num_resizes;
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
			free(frame);
			return;
		}
		tmpframe = frame;
		frame = frame->next;
	}
	/* no frame found, raise an error */
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
_S_memtrace_free(void)
{
#ifdef DEBUG_TRACE
	struct _S_memtrace_memory_frame_s *frame, *tmpframe;
#endif /* DEBUG_TRACE */

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

