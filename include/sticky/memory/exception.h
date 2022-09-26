/*
 * exception.h
 * Exception handler header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 26/09/2022
 */

#ifndef FR_RAYMENT_STICKY_EXCEPTION_H
#define FR_RAYMENT_STICKY_EXCEPTION_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <setjmp.h>

#include "sticky/common/defines.h"
#include "sticky/common/error.h"
#include "sticky/common/types.h"

struct
_S_exception_frame_s
{
	jmp_buf env;
	struct _S_exception_frame_s *next;
};

STICKY_API extern THREAD_LOCAL struct _S_exception_frame_s *_S_except_stack;

#define S_TRY \
	do \
	{ \
		Senum _S_error_store; \
		_S_error_store = SERRNO; \
		_S_CALL("_S_exception_push_frame", _S_exception_push_frame()); \
		switch(setjmp(_S_except_stack->env)) \
		{ \
		case 0:

#define S_CATCH(x) \
	break; case (x):
#define S_ENDTRY \
	break; \
	default: \
	if (SERRNO != S_NO_ERROR) \
	{ \
		_S_CALL("_S_exception_pop_frame", _S_exception_pop_frame()); \
		S_error("Uncaught exception: '%s'!\n", S_error_string()); \
	} \
	break; \
	} \
	_S_CALL("_S_exception_pop_frame", _S_exception_pop_frame()); \
	SERRNO = _S_error_store; \
	} while (0)
#define S_THROW(x) \
	if (_S_except_stack) \
	{ \
		_S_SET_ERROR((x), "exception"); \
		longjmp(_S_except_stack->env, (x)); \
	}

STICKY_API void _S_exception_env_init(void);
STICKY_API void _S_exception_env_free(void);
STICKY_API void _S_exception_push_frame(void);
STICKY_API void _S_exception_pop_frame(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_EXCEPTION_H */

