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

#ifndef FR_RAYMENT_STICKY_ALLOCATOR_H
#define FR_RAYMENT_STICKY_ALLOCATOR_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/types.h"

/**
 * @addtogroup memory
 * @{
 */

STICKY_API void  *_S_memory_new(Ssize_t, const Schar *, Suint32);
STICKY_API void  *_S_memory_resize(void *, Ssize_t, const Schar *, Suint32);
STICKY_API void   _S_memory_delete(void *, const Schar *, Suint32);
void   _S_out_of_memory(const Schar *, Suint32);

/**
 * @brief Allocate a given number of bytes on the heap.
 * @hideinitializer
 *
 * Internally calls the standard function <c>malloc(size_t)</c> and allocates a
 * given number of bytes on the heap, returning a void pointer which may be cast
 * to a given type by the user.
 *
 * @param[in] x The number of bytes to allocate on the heap.
 * @return A <c>void *</c> to a block of contiguous memory containing @p x
 * bytes.
 * @exception S_INVALID_VALUE If 0 bytes are to be allocated.
 * @since 1.0.0
 */
#define S_memory_new(x)    _S_memory_new(x, __FILE__, __LINE__)

/**
 * @brief Resize an allocated block of memory on the heap.
 * @hideinitializer
 *
 * Internally calls the standard function <c>realloc(void *, size_t)</c> and
 * resized a given block of memory on the heap, returning a void pointer to the
 * new location (if it changed) of the block of memory, which may be cast to a
 * given type by the user.
 *
 * If the block of memory is enlarged, the new area will not be initialised. If
 * the block of memory is shrunk, the portion no longer in range will no longer
 * be accessible. No memory that resides at the pointer address and remains in
 * range will be modified. The memory may however, be moved to a new location.
 *
 * @param[in] x The pointer to resize.
 * @param[in] y The new size of the pointer in bytes.
 * @return A <c>void *</c> to the block of memory that contains the resized
 * pointer @p x.
 * @exception S_INVALID_VALUE If the block is to be resized to 0 bytes.
 * @since 1.0.0
 */
#define S_memory_resize(x,y) _S_memory_resize(x, y, __FILE__, __LINE__)

/**
 * @brief Free a pointer from memory.
 * @hideinitializer
 *
 * Internall calls the standard function <c>free(void *)</c> and frees a block
 * of memory from the heap.
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

