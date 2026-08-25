/*
 * st_macros.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/08/2026
 */

#ifndef STICKY_ST_MACROS_H
#define STICKY_ST_MACROS_H

#include <assert.h>

#ifdef __cplusplus
#include <type_traits>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__GNUC__) && !defined(__clang__)
#error "Unsupported compiler: GCC or Clang required!"
#endif

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#ifndef __cplusplus
#if !__has_builtin(__builtin_types_compatible_p)
#error "Compiler does not support __builtin_types_compatible_p!"
#endif
#if !__has_builtin(__builtin_choose_expr)
#error "Compiler does not support __builtin_choose_expr!"
#endif
#endif

#if __has_attribute(hot)
#define ST_HOT __attribute__((hot))
#else
#define ST_HOT
#endif

#if __has_attribute(cold)
#define ST_COLD __attribute__((cold))
#else
#define ST_COLD
#endif

#if __has_builtin(__builtin_expect)
#define ST_LIKELY(x) __builtin_expect(!!(x), 1)
#define ST_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define ST_LIKELY(x)
#define ST_UNLIKELY(x)
#endif

#if __has_attribute(aligned)
#define ST_ALIGNED(n) __attribute__((aligned(n)))
#else
#define ST_ALIGNED(n)
#endif

#if __has_attribute(packed)
#define ST_PACKED __attribute__((packed))
#else
#define ST_PACKED
#endif

#if __has_attribute(pure)
#define ST_PURE __attribute__((pure))
#else
#define ST_PURE
#endif

#if __has_attribute(const)
#define ST_CONST __attribute__((const))
#else
#define ST_CONST
#endif

#if __has_attribute(always_inline)
#define ST_INLINE __attribute__((always_inline))
#else
#define ST_INLINE
#endif

#if __has_attribute(noinline)
#define ST_NOINLINE __attribute__((noinline))
#else
#define ST_NOINLINE
#endif

#if __has_attribute(noreturn)
#define ST_NORETURN __attribute__((noreturn))
#else
#define ST_NORETURN
#endif

#if __has_attribute(unused)
#define ST_UNUSED __attribute__((unused))
#else
#define ST_UNUSED
#endif

#if __has_attribute(format)
#define ST_FORMAT_PRINTF(fmt, args) __attribute__((__format__(__printf__, fmt, args)))
#else
#define ST_FORMAT_PRINTF(fmt, args)
#endif

#if __has_builtin(__builtin_unreachable)
#define ST_UNREACHABLE() __builtin_unreachable()
#else
#define ST_UNREACHABLE() ((void) 0)
#endif

#define ST_CAST(type, value) ((type)(value))

#define ST_SIZEOF(x) sizeof(x)

#ifdef __cplusplus
#define ST_TYPEOF(x) decltype(x)

#define ST_TYPES_COMPATIBLE(a, b) \
    (std::is_same_v<std::remove_cv_t<a>, std::remove_cv_t<b>>)

#define ST_IS_ARRAY(x) \
    (std::is_array_v<std::remove_reference_t<decltype(x)>>)

#define ST_IS_ARRAY_OF_TYPE(type, x) \
    (ST_IS_ARRAY(x) && \
     std::is_same_v< \
     std::remove_cv_t<std::remove_reference_t<decltype((x)[0])>>, \
     std::remove_cv_t<type>>)

#define ST_IS_PTR_OR_ARRAY_OF_TYPE(type, x) \
    (std::is_same_v<decltype(&(x)[0]), type *> || \
     std::is_same_v<decltype(&(x)[0]), const type *>)
#else
#define ST_TYPEOF(x) __typeof__(x)

#define ST_TYPES_COMPATIBLE(a, b) __builtin_types_compatible_p(a, b)

#define ST_IS_ARRAY(x) (!ST_TYPES_COMPATIBLE(ST_TYPEOF(x), ST_TYPEOF(&(x)[0])))

#define ST_IS_ARRAY_OF_TYPE(type, x) \
    (ST_IS_ARRAY(x) && \
    (ST_TYPES_COMPATIBLE(ST_TYPEOF((x)[0]), type)))

#define ST_IS_PTR_OR_ARRAY_OF_TYPE(type, x) \
    (ST_TYPES_COMPATIBLE(ST_TYPEOF(&(x)[0]), type *) || \
     ST_TYPES_COMPATIBLE(ST_TYPEOF(&(x)[0]), const type *))
#endif

#if __has_attribute(warning)
#define ST_WARNING(x) __attribute__((warning(x)))
#else
#define ST_WARNING(x)
#endif

#if __has_attribute(deprecated)
#define ST_DEPRECATED(x) __attribute__((deprecated(x)))
#else
#define ST_DEPRECATED(x) ST_WARNING(x)
#endif

#if __has_attribute(error)
#define ST_ERROR(x) __attribute__((error(x)))
#else
#define ST_ERROR(x)
#endif

#define ST_ASSERT(x) assert(x)

#ifdef __cplusplus
}
#endif

#endif // STICKY_ST_MACROS_H
