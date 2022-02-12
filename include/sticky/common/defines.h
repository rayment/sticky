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

#endif /* FR_RAYMENT_STICKY_DEFINES_H */

