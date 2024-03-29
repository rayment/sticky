/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * thread_posix.c
 * POSIX thread source file.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/02/2022
 */

#include "sticky/common/error.h"
#include "sticky/concurrency/thread.h"
#include "sticky/memory/allocator.h"

#ifndef STICKY_POSIX
#error This source file cannot be compiled on non-POSIX systems.
#endif /* STICKY_POSIX */

#include <pthread.h>
#include <time.h>

static
void *
_S_thread_func_wrapper(void *arg)
{
	Sthread thread;
	thread = (Sthread) arg;
	return thread->func(thread->arg);
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
	if (pthread_create(&thread->handle, NULL, _S_thread_func_wrapper, thread)
	    != 0)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_thread_new");
		return NULL;
	}
	return thread;
}

void
S_thread_sleep(Suint64 sec)
{
	struct timespec ts;
	ts.tv_sec  = sec;
	ts.tv_nsec = 0;
	nanosleep(&ts, NULL);
}

void
S_thread_msleep(Suint64 msec)
{
	struct timespec ts;
	ts.tv_sec  =  msec / 1000;
	ts.tv_nsec = (msec % 1000) * 1000000;
	nanosleep(&ts, NULL);
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
	else if (pthread_join(thread->handle, &ptr) != 0)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_thread_join");
		return NULL;
	}
	S_memory_delete(thread);
	return ptr;
}

