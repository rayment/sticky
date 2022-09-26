/*
 * fileio.h
 * File I/O header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 26/09/2022
 */

#ifndef FR_RAYMENT_STICKY_FILEIO_H
#define FR_RAYMENT_STICKY_FILEIO_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/types.h"

/**
 * @addtogroup fileio
 * @{
 */

/**
 * @brief Read all text from a file.
 *
 * Reads all of the contents of a given file and allocates a block of memory on
 * the heap to hold the contents. This memory must be deallocated afterwards
 * via. {@link S_memory_delete}.
 *
 * @return A pointer to the beginning of the text contained within the file.
 * @param[in] filename The file path to read from.
 * @param[out] len The length in bytes of the read file.
 * @exception S_IO_ERROR If an I/O error occurs while reading the file.
 * @since 1.0.0
 */
STICKY_API Schar *S_file_read_all(const Schar *, Sint64 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_FILEIO_H */

