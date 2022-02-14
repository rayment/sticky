/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mutex.h
 * Generic mutex header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 01/05/2021
 */

/**
 * @file mutex.h
 * @addtogroup concurrency
 * @{
 * @addtogroup mutex
 * @{
 */

#ifndef FR_RAYMENT_STICKY_MUTEX_H
#define FR_RAYMENT_STICKY_MUTEX_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* force-load defines for OS check */
#include "sticky/common/defines.h"

#if defined(STICKY_WINDOWS)
#include <windows.h>
#elif defined(STICKY_POSIX)
#include <pthread.h>
typedef pthread_mutex_t _Smutex_raw;
#endif /* STICKY_WINDOWS */
/**
 * @brief The generic mutex data-type.
 *
 * The typedef for <b>_Smutex_raw</b> is platform-dependant and will
 * automatically select a native mutex implementation for the compiling system.
 *
 * @since 1.0.0
 */
typedef _Smutex_raw *Smutex;

/**
 * @brief Allocate a new mutex in memory.
 *
 * A new mutex will be allocated on the heap and returned from the function.
 * In the case that not enough memory is available to allocate the mutex, an
 * error will be displayed and the program forcefully closed.
 *
 * @return A mutex allocated on the heap. To correctly destroy the mutex, call
 * {@link S_mutex_delete(Smutex)}.
 * @since 1.0.0
 */
Smutex S_mutex_new(void);
/**
 * @brief Free a mutex from memory.
 *
 * Take a given mutex and free's it from memory. Once this function is called
 * for a given mutex, that mutex becomes invalid and may not be used again in
 * any other mutex function.
 *
 * Note that the mutex should not be currently locked, and this function called
 * only from the same thread that created the mutex. Other circumstances are
 * undefined, or may depend on the implementing operating system.
 *
 * @param[in] lock The mutex to be free'd.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid mutex is provided to
 * the function.
 * @exception S_INVALID_OPERATION If a native error occurs while trying to
 * delete the mutex.
 * @since 1.0.0
 */
void   S_mutex_delete(Smutex);
/**
 * @brief Block and wait to lock a mutex.
 *
 * Block the current thread from execution until the given mutex is available
 * (unlocked), then lock it and continue.
 *
 * Note that this function should not be called if doing so would cause a
 * deadlock. Other circumstances are undefined, or may depend on the
 * implementing operating system.
 *
 * @param[in,out] lock The mutex to be locked.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid mutex is provided to
 * the function.
 * @exception S_INVALID_OPERATION If a native error occurs while trying to
 * lock the mutex.
 * @since 1.0.0
 */
void   S_mutex_lock(Smutex);

/**
 * @brief Attempt to lock a mutex without blocking.
 *
 * If a mutex is currently locked, then this function will immediately return
 * {@link S_FALSE}. If it is not locked, then the function will succeed and lock
 * the mutex before returning {@link S_TRUE}.
 *
 * @param[in,out] lock The mutex to attempt the lock on.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid mutex is provided to
 * the function.
 * @since 1.0.0
 */
Sbool  S_mutex_trylock(Smutex);
/**
 * @brief Unlock a mutex.
 *
 * Unlock a mutex currently locked by the same thread, relinquishing ownership
 * and allowing other threads to lock it. Calling this function from a given
 * thread may release the block on exactly one thread that is currently
 * blocking via. the function {@link S_mutex_lock(Smutex)} or attempting a lock
 * via. {@link S_mutex_trylock(Smutex)}.
 *
 * Note that this function should not be called from any other thread than the
 * current owner of the lock. Other circumstances are undefined, or may depend
 * on the implementing operating system.
 *
 * @param[in,out] lock The mutex to be unlocked.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid mutex is provided to
 * the function.
 * @exception S_INVALID_OPERATION If a native error occurs while trying to
 * unlock the mutex.
 * @since 1.0.0
 */
void   S_mutex_unlock(Smutex);

/**
 * @}
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MUTEX_H */

