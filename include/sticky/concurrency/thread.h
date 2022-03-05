/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * thread.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/02/2022
 */

#ifndef FR_RAYMENT_STICKY_THREAD_H
#define FR_RAYMENT_STICKY_THREAD_H 1

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
typedef pthread_t _Sthread_raw;
typedef void *(*Sthread_func)(void *);
#elif defined(DOXYGEN)

/**
 * @addtogroup thread
 * @{
 */

/**
 * @brief Function type to be used for thread calls.
 *
 * The function should return a <b><c>void *</c></b> which can be obtained by
 * reading the return value from {@link S_thread_join(Sthread)} from the parent
 * thread.
 *
 * @since 1.0.0
 */
typedef void *(*Sthread_func)(void *);
#endif /* STICKY_WINDOWS */
/**
 * @brief The generic thread data-type.
 *
 * The typedef for <b>_Sthread_raw</b> is platform-dependant and will
 * automatically select a native thread implementation for the compiling system.
 *
 * @since 1.0.0
 */
typedef _Sthread_raw *Sthread;

/**
 * @brief Allocate and spawn a new thread.
 *
 * A new thread will be allocated on the heap and spawned once this function is
 * called. The thread shall execute a given function and optionally take a
 * <b><c>void *</c></b> parameter which is then passed to the called function as
 * an argument.
 *
 * To deallocate a thread, the calling thread should call
 * {@link S_thread_join(Sthread)} for clean up purposes, as well as to retrieve
 * the results of the thread call.
 *
 * @param[in] func Pointer to a function to be spawned as a new thread.
 * @param[in,out] arg An argument to pass to the spawned function, or
 * <c>NULL</c> if none is desired.
 * @return A new thread that shall simultaneously be spawned.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid function pointer is
 * provided to the function.
 * @exception S_INVALID_OPERATION If a native error occurs while trying to join
 * the thread.
 * @since 1.0.0
 */
Sthread S_thread_new(Sthread_func, void *);

/**
 * @brief Sleep for a number of seconds in the current thread.
 *
 * Pauses the current thread execution by sleeping for a given number of
 * seconds. This function does not require an actual {@link Sthread} to exist,
 * as it can also be called on the main program thread as a cross-platform way
 * of pausing program execution.
 *
 * @param[in] sec The number of seconds to sleep for.
 * @since 1.0.0
 */
void    S_thread_sleep(Suint64);

/**
 * @brief Sleep for a number of milliseconds in the current thread.
 *
 * Pauses the current thread execution by sleeping for a given number of
 * milliseconds. This function does not require an actual {@link Sthread} to
 * exist, as it can also be called on the main program thread as a
 * cross-platform way of pausing program execution.
 *
 * @param[in] msec The number of milliseconds to sleep for.
 * @since 1.0.0
 */
void    S_thread_msleep(Suint64);

/**
 * @brief Sleep for a number of nanoseconds in the current thread.
 *
 * Pauses the current thread execution by sleeping for a given number of
 * nanoseconds. This function does not require an actual {@link Sthread} to
 * exist, as it can also be called on the main program thread as a
 * cross-platform way of pausing program execution.
 *
 * @param[in] nsec The number of nanoseconds to sleep for.
 * @since 1.0.0
 */
void    S_thread_nsleep(Suint64);

/**
 * @brief Wait for a thread to join and finish, and then deallocate it.
 *
 * The calling thread will suspend execution until a given thread terminates. In
 * the case that the thread has already terminated, no suspension will occur.
 * Once this function is called for a given thread, that thread becomes invalid
 * and may not be used again in any other thread function.
 *
 * After the thread has terminated, it shall be free'd from memory and finally
 * the <b><c>void *</c></b> that was returned by the thread shall be returned
 * by this function.
 *
 * @param[in,out] thread The thread to wait for and finally deallocate.
 * @return The <b><c>void *</c></b> that was returned from the spawned thread.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid thread is provided to
 * the function.
 * @exception S_INVALID_OPERATION If a native error occurs while trying to join
 * the thread.
 * @since 1.0
 */
void   *S_thread_join(Sthread);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_THREAD_H */

