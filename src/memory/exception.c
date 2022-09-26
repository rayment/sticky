/*
 * exception.c
 * Exception handler source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 26/09/2022
 */

#include "sticky/common/defines.h"
#include "sticky/common/types.h"
#include "sticky/memory/allocator.h"
#include "sticky/memory/exception.h"

THREAD_LOCAL struct _S_exception_frame_s *_S_except_stack;

void
_S_exception_env_init(void)
{
	_S_except_stack = NULL;
}

void
_S_exception_env_free(void)
{
	while (_S_except_stack)
		_S_exception_pop_frame();
}

void
_S_exception_push_frame(void)
{
	struct _S_exception_frame_s *frame;
	frame = S_memory_new(sizeof(struct _S_exception_frame_s));
	frame->next = _S_except_stack;
	_S_except_stack = frame;
}

void
_S_exception_pop_frame(void)
{
	struct _S_exception_frame_s *tmp;
	tmp = NULL;
	if (_S_except_stack)
	{
		tmp = _S_except_stack->next;
		S_memory_delete(_S_except_stack);
	}
	_S_except_stack = tmp;
}

