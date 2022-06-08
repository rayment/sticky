/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

 /*
  * mutex_win32.c
  * Windows mutex source file.
  *
  * Author       : Finn Rayment <finn@rayment.fr>
  * Date created : 08/06/2022
  */

#include "sticky/common/error.h"
#include "sticky/concurrency/mutex.h"
#include "sticky/memory/allocator.h"

#ifndef STICKY_WINDOWS
#error This source file cannot be compiled on non-Windows systems.
#endif /* STICKY_WINDOWS */

#include <errno.h>
#include <windows.h>

Smutex
S_mutex_new(void)
{
	Smutex lock;

	lock = (Smutex) S_memory_new(sizeof(_Smutex_raw));
	if (!(*lock = CreateMutex(NULL, FALSE, NULL)))
	{
		/* TODO: GetLastError */
		if (errno == ENOMEM)
		{
			_S_out_of_memory(_S_ERR_LOC);
		}
		else
		{
			_S_SET_ERROR(S_UNKNOWN_ERROR, "S_mutex_new");
		}
		S_memory_delete(lock);
		return NULL;
	}
	return lock;
}

void
S_mutex_delete(Smutex lock)
{
	if (!lock)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mutex_delete");
		return;
	}
	else if (!CloseHandle(*lock))
	{
		/* TODO: GetLastError */
		_S_SET_ERROR(S_INVALID_OPERATION, "S_mutex_delete");
		return;
	}
	S_memory_delete(lock);
}

void
S_mutex_lock(Smutex lock)
{
	if (!lock)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mutex_lock");
	}
	else if (WaitForSingleObject(*lock, INFINITE) == WAIT_FAILED)
	{
		/* TODO: GetLastError */
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
	return WaitForSingleObject(*lock, 0) != WAIT_FAILED;
}

void
S_mutex_unlock(Smutex lock)
{
	if (!lock)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mutex_unlock");
	}
	else if (!ReleaseMutex(*lock))
	{
		/* TODO: GetLastError */
		_S_SET_ERROR(S_INVALID_OPERATION, "S_mutex_unlock");
	}
}

