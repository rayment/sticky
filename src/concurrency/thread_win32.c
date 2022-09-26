/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * thread_win32.c
 * Windows thread source file.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 08/06/2022
 */

#include "sticky/common/error.h"
#include "sticky/concurrency/thread.h"
#include "sticky/memory/allocator.h"
#include "sticky/memory/exception.h"

#ifndef STICKY_WINDOWS
#error This source file cannot be compiled on non-Windows systems.
#endif /* STICKY_WINDOWS */

#include <time.h>
#include <windows.h>

static
DWORD WINAPI
_S_thread_func_wrapper(LPVOID *arg)
{
	Sthread thread;
	thread = (Sthread) arg;
	/* create new environment for the thread */
	_S_CALL("_S_exception_env_init", _S_exception_env_init());
	thread->ret = thread->func(thread->arg);
	_S_CALL("_S_exception_env_free", _S_exception_env_free());
	return 0;
}

Sthread
S_thread_new(Sthread_func func,
             void *arg)
{
	Sthread thread;
	if (!func)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_thread_new");
		return NULL;
	}
	thread = S_memory_new(sizeof(_Sthread_raw));
	thread->func = func;
	thread->arg = arg;
	if (!(thread->handle =
	    CreateThread(NULL, 0, _S_thread_func_wrapper, thread, 0, NULL)))
	{
		/* TODO: GetLastError */
		_S_SET_ERROR(S_INVALID_OPERATION, "S_thread_new");
		return NULL;
	}
	return thread;
}

void
S_thread_sleep(Suint64 sec)
{
	Sleep((DWORD) sec * 1000);
}

void
S_thread_msleep(Suint64 msec)
{
	Sleep((DWORD) msec);
}

void *
S_thread_join(Sthread thread)
{
	void *ptr;
	if (!thread)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_thread_join");
		return NULL;
	}
	else if (WaitForSingleObject(thread->handle, INFINITE) == WAIT_FAILED)
	{
		/* TODO: GetLastError */
		_S_SET_ERROR(S_INVALID_OPERATION, "S_thread_join");
		return NULL;
	}
	ptr = thread->ret;
	S_memory_delete(thread);
	return ptr;
}

