/*
 * st_macros.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

/**
 * @file
 * @brief Compiler attribute wrappers, class helpers and assertions.
 */

#ifndef STICKY_ST_MACROS_H
#define STICKY_ST_MACROS_H

#include "st_types.h"

#if !defined(__GNUC__) && !defined(__clang__)
#error "Unsupported compiler: GCC or Clang required!"
#endif // !defined(__GNUC__) && !defined(__clang__)

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif // __has_attribute
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif // __has_builtin

#ifndef __cplusplus
#if !__has_builtin(__builtin_types_compatible_p)
#error "Compiler does not support __builtin_types_compatible_p!"
#endif // !__has_builtin(__builtin_types_compatible_p)
#if !__has_builtin(__builtin_choose_expr)
#error "Compiler does not support __builtin_choose_expr!"
#endif // !__has_builtin(__builtin_choose_expr)
#endif // __cplusplus

/** @addtogroup core
 *  @{
 */

#if __has_attribute(hot)
#define ST_HOT __attribute__((hot))
#else // __has_attribute(hot)
#define ST_HOT
#endif // __has_attribute(hot)

#if __has_attribute(cold)
#define ST_COLD __attribute__((cold))
#else // __has_attribute(cold)
#define ST_COLD
#endif // __has_attribute(cold)

#if __has_builtin(__builtin_expect)
#define ST_LIKELY(x) __builtin_expect(!!(x), 1)
#define ST_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else // __has_builtin(__builtin_expect)
#define ST_LIKELY(x) (x)
#define ST_UNLIKELY(x) (x)
#endif // __has_builtin(__builtin_expect)

#if __has_attribute(aligned)
#define ST_ALIGNED(n) __attribute__((aligned(n)))
#else // __has_attribute(aligned)
#define ST_ALIGNED(n)
#endif // __has_attribute(aligned)

#if __has_attribute(packed)
#define ST_PACKED __attribute__((packed))
#else // __has_attribute(packed)
#define ST_PACKED
#endif // __has_attribute(packed)

#if __has_attribute(pure)
#define ST_PURE __attribute__((pure))
#else // __has_attribute(pure)
#define ST_PURE
#endif // __has_attribute(pure)

#if __has_attribute(const)
#define ST_CONST __attribute__((const))
#else // __has_attribute(const)
#define ST_CONST
#endif // __has_attribute(const)

#if __has_attribute(always_inline)
#define ST_INLINE __attribute__((always_inline))
#else // __has_attribute(always_inline)
#define ST_INLINE
#endif // __has_attribute(always_inline)

#if __has_attribute(noinline)
#define ST_NOINLINE __attribute__((noinline))
#else // __has_attribute(noinline)
#define ST_NOINLINE
#endif // __has_attribute(noinline)

#if __has_attribute(noreturn)
#define ST_NORETURN __attribute__((noreturn))
#else // __has_attribute(noreturn)
#define ST_NORETURN
#endif // __has_attribute(noreturn)

#if __has_attribute(unused)
#define ST_UNUSED __attribute__((unused))
#else // __has_attribute(unused)
#define ST_UNUSED
#endif // __has_attribute(unused)

#if __has_builtin(__builtin_unreachable)
#define ST_UNREACHABLE() __builtin_unreachable()
#else // __has_builtin(__builtin_unreachable)
#define ST_UNREACHABLE() ((void) 0)
#endif // __has_builtin(__builtin_unreachable)

#ifdef __cplusplus
#define ST_TYPEOF(x) decltype(x)
#else // __cplusplus
#define ST_TYPEOF(x) __typeof__(x)
#endif // __cplusplus

/** Expands `x` and turns the result into a string literal. */
#define ST_STRINGIFY(x) ST_STRINGIFY_IMPL(x)
#define ST_STRINGIFY_IMPL(x) #x

/** Declares a function and opens its definition, so that C files with `-Wmissing-prototypes` need no header. */
#define ST_EXTERN_PROTOTYPE(prototype)                                                                                 \
    prototype;                                                                                                         \
    prototype

#ifdef __cplusplus
// NOLINTBEGIN(bugprone-macro-parentheses)
/** Deletes the copy constructor and copy assignment operator of `classname`. */
#define ST_DELETE_COPY_CTORS(classname)                                                                                \
    classname(const classname &) = delete;                                                                             \
    classname &operator=(const classname &) = delete
/** Deletes the move constructor and move assignment operator of `classname`. */
#define ST_DELETE_MOVE_CTORS(classname)                                                                                \
    classname(classname &&) = delete;                                                                                  \
    classname &operator=(classname &&) = delete
// NOLINTEND(bugprone-macro-parentheses)
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Reports a failed assertion and aborts the program.
 *
 * Called by #ST_ASSERT, not intended to be called directly.
 */
ST_NORETURN void st_assert_fail(const st_char *expression,
                                const st_char *file,
                                st_int32       line);

#ifdef __cplusplus
}
#endif

/**
 * @brief Checks an invariant that only a programming error can break.
 *
 * In debug builds a failed check reports the expression and aborts. In release builds the expression is not evaluated.
 * Conditions that depend on input, files or the system must be handled and reported instead.
 */
#ifdef NDEBUG
#define ST_ASSERT(expr) ((void) 0)
#else // NDEBUG
#define ST_ASSERT(expr)                                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        if (ST_UNLIKELY(!(expr)))                                                                                      \
            st_assert_fail(#expr, __FILE__, __LINE__);                                                                 \
    }                                                                                                                  \
    while (0)
#endif // NDEBUG

/** @} */

#endif // STICKY_ST_MACROS_H
