/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * sticky.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/02/2022
 */

/**
 * @file sticky.h
 * @addtogroup memory Memory management
 * @{
 */

#ifndef FR_RAYMENT_STICKY_STICKY_H
#define FR_RAYMENT_STICKY_STICKY_H 1

/* these must come before all other includes */
#include "sticky/common/defines.h"
#include "sticky/common/includes.h"

#include "sticky/common/error.h"
#include "sticky/common/types.h"

#include "sticky/collections/linkedlist.h"

#include "sticky/concurrency/mutex.h"
#include "sticky/concurrency/thread.h"

#include "sticky/math/math.h"
#include "sticky/math/mat3.h"
#include "sticky/math/mat4.h"
#include "sticky/math/quat.h"
#include "sticky/math/transform.h"
#include "sticky/math/vec2.h"
#include "sticky/math/vec3.h"
#include "sticky/math/vec4.h"

#include "sticky/memory/allocator.h"
#include "sticky/memory/memtrace.h"

/**
 * @brief Initialise core engine components.
 *
 * Most of <b>sticky</b> can be accessed directly, however some core components
 * such as error handling require that this function be called before they be
 * used.
 *
 * @warning Given the integrated nature of the components of <b>sticky</b>,
 * <i>not</i> calling this function at the beginning of a program will most
 * certainly lead to undefined behaviour. <b>Do not call any other function
 * before calling this. You have been warned.</b>
 * @since 1.0.0
 */
void S_sticky_init(void);

/**
 * @brief Clean up engine at the end of program execution.
 *
 * When the program should exit gracefully (that is, no error forced it to
 * shutdown), then this function should be called. Any memory in use will be
 * deallocated, and any statistics will be dumped to the terminal if debugging
 * was enabled on build.
 *
 * @see S_sticky_init(void)
 * @since 1.0.0
 */
void S_sticky_free(void);

/**
 * @}
 */

#endif /* FR_RAYMENT_STICKY_STICKY_H */

