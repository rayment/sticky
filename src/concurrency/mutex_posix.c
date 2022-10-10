/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mutex_posix.c
 * POSIX mutex source file.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 01/05/2021
 */

#include "sticky/common/error.h"
#include "sticky/concurrency/mutex.h"
#include "sticky/memory/allocator.h"

#ifndef STICKY_POSIX
#error This source file cannot be compiled on non-POSIX systems.
#endif /* STICKY_POSIX */

#include <errno.h>
#include <pthread.h>

Smutex
_S_mutex_new(Sbool report)
{
	Smutex lock;

	lock = (Smutex) _S_memory_new(sizeof(_Smutex_raw),
	                              __FILE__, __LINE__, report);
	if (pthread_mutex_init(lock, NULL) != 0)
	{
		if (errno == ENOMEM)
		{
			_S_out_of_memory(_S_ERR_LOC);
		}
		else
		{
			_S_SET_ERROR(S_UNKNOWN_ERROR, "S_mutex_new");
		}
		_S_memory_delete(lock, __FILE__, __LINE__, report);
		return NULL;
	}
	return lock;
}

Smutex
S_mutex_new(void)
{
	Smutex lock;
	_S_CALL("_S_mutex_new", lock = _S_mutex_new(S_TRUE));
	return lock;
}

void
_S_mutex_delete(Smutex lock,
                Sbool report)
{
	if (!lock)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mutex_delete");
		return;
	}
	else if (pthread_mutex_destroy(lock) != 0)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_mutex_delete");
		return;
	}
	_S_memory_delete(lock, __FILE__, __LINE__, report);
}

void
S_mutex_delete(Smutex lock)
{
	_S_CALL("_S_mutex_delete", _S_mutex_delete(lock, S_TRUE));
}

void
S_mutex_lock(Smutex lock)
{
	if (!lock)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mutex_lock");
	}
	else if (pthread_mutex_lock(lock) != 0)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_mutex_lock");
	}
}

Sbool
S_mutex_trylock(Smutex lock)
{
	if (!lock)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mutex_trylock");
		return S_FALSE;
	}
	return pthread_mutex_trylock(lock) == 0;
}

void
S_mutex_unlock(Smutex lock)
{
	if (!lock)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mutex_unlock");
	}
	else if (pthread_mutex_unlock(lock) != 0)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_mutex_unlock");
	}
}

