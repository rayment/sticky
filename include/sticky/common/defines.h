/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * defines.h
 * Common project defines header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/02/2022
 */

#ifndef FR_RAYMENT_STICKY_DEFINES_H
#define FR_RAYMENT_STICKY_DEFINES_H 1

/* OS detection */
#if defined(__linux__)
#define STICKY_LINUX 1
#define STICKY_POSIX 1
#endif /* __linux__ */

#if defined(_WIN32) || defined(WIN32)
#define STICKY_WINDOWS 1
/* disable superfluous Windows headers */
#define WIN32_LEAN_AND_MEAN
#endif /* _WIN32 || WIN32 */

#if defined(_WIN64)
#define STICKY_WINDOWS 1
#endif /* _WIN64 */

#if defined(__APPLE__) || defined(__MACH__)
#define STICKY_MACOS 1
#define STICKY_POSIX 1
#endif /* __APPLE__ || __MACH__ */

#if (defined(__unix__) || defined(__posix__)) && !defined(STICKY_POSIX)
#define STICKY_POSIX 1
#endif /* __unix__ */

#if !defined(STICKY_LINUX) && !defined(STICKY_WINDOWS) && !defined(STICKY_MACOS)
#error Unsupported operating system! Please compile on Linux, Windows or macOS.
#endif /* !(STICKY_WINDOWS || STICKY_LINUX || STICKY_MACOS) */

/* GNU / MSVC detection and relevant macros */
#if defined(__GNUC__)
#define PACK(...) __VA_ARGS__ __attribute__((__packed__))
#define ALIGN(x,...) __VA_ARGS__ __attribute__((aligned(x)))
#define THREAD_LOCAL __thread
#if __GNUC__ >= 4
#define STICKY_API __attribute__((visibility("default")))
#else /* __GNUC__ >= 4 */
#define STICKY_API
#endif /* __GNUC__ >= 4 */
#elif defined(_MSC_VER) /* __GNUC__ and _MSC_VER */
#define PACK(...) __pragma(pack(push, 1)) __VA_ARGS__ __pragma(pack(pop))
#define ALIGN(x,...) __VA_ARGS__ __declspec(align(x))
#define THREAD_LOCAL __declspec(thread)
#ifdef STICKY_ISMAKELIB /* STICKY_ISMAKELIB */
#define STICKY_API __declspec(dllexport)
#else /* STICKY_ISMAKELIB */
#define STICKY_API __declspec(dllimport)
#endif /* STICKY_ISMAKELIB */
#else /* __GNUC__ and _MSC_VER */
#error Unsupported compiler! Please use a GCC or MSVC compatible compiler.
#endif /* __GNUC__ and _MSC_VER */

#endif /* FR_RAYMENT_STICKY_DEFINES_H */

