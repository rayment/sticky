/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * allocator.h
 * Custom memory allocator header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 05/09/2021
 */

/**
 * @file allocator.h
 * @addtogroup memory
 * @{
 */

#ifndef FR_RAYMENT_STICKY_ALLOCATOR_H
#define FR_RAYMENT_STICKY_ALLOCATOR_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/types.h"

void  *_S_memory_new(const Ssize_t, const Schar *, const Suint32);
void   _S_memory_delete(void *, const Schar *, const Suint32);
void   _S_out_of_memory(const Schar *, const Suint32);

/**
 * @brief Allocate a given number of bytes on the heap.
 * @hideinitializer
 *
 * Internally calls the standard function <b><c>malloc(size_t)</c></b> and
 * allocates a given number of bytes on the heap, returning a
 * <b><c>void *</c></b> which may be cast to a given type by the user.
 *
 * @param[in] x The number of bytes to allocate on the heap.
 * @return A <b><c>void *</c></b> to a block of contiguous memory containing
 * <b>x</b> number of bytes.
 * @exception S_INVALID_VALUE If 0 bytes are to be allocated.
 * @since 1.0.0
 */
#define S_memory_new(x)    _S_memory_new(x, __FILE__, __LINE__)
/**
 * @brief Free a pointer from memory.
 * @hideinitializer
 *
 * Internall calls the standard function <b><c>free(void *)</c></b> and frees a
 * block of memory from the heap.
 *
 * Note that this function should only be used with memory previously allocated
 * by {@link S_memory_new(x)}. Using any other pointer allocated by another
 * memory manager or standard library function is undefined.
 *
 * @param[in,out] x A pointer to a currently allocated block of memory to free.
 * @exception S_INVALID_VALUE If a <c>NULL</c> pointer is provided.
 * @since 1.0.0
 */
#define S_memory_delete(x) _S_memory_delete(x, __FILE__, __LINE__)

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_ALLOCATOR_H */

