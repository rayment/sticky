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

#ifndef FR_RAYMENT_STICKY_STICKY_H
#define FR_RAYMENT_STICKY_STICKY_H 1

/**
 * @addtogroup memory Memory management
 * @{
 */

/*
 * WARNING: Don't put this include section in an __cplusplus extern guard
 * because any code loaded from external libaries may be affected.
 */

/* these must come before all other includes */
#include "sticky/common/defines.h"
#include "sticky/common/includes.h"

#include "sticky/audio/listener.h"
#include "sticky/audio/sound.h"
#include "sticky/audio/speaker.h"

#include "sticky/common/error.h"
#include "sticky/common/types.h"

#include "sticky/collections/linkedlist.h"

#include "sticky/concurrency/mutex.h"
#include "sticky/concurrency/thread.h"

#include "sticky/input/gamepad.h"
#include "sticky/input/keyboard.h"
#include "sticky/input/mouse.h"

#include "sticky/math/frustum.h"
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

#include "sticky/net/socket.h"
#include "sticky/net/tcp.h"

#include "sticky/util/random.h"
#include "sticky/util/string.h"

#include "sticky/video/camera.h"
#include "sticky/video/font.h"
#include "sticky/video/mesh.h"
#include "sticky/video/pencil.h"
#include "sticky/video/shader.h"
#include "sticky/video/texture.h"
#include "sticky/video/window.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

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
STICKY_API void S_sticky_init(void);

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
STICKY_API void S_sticky_free(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
 * @}
 */

#endif /* FR_RAYMENT_STICKY_STICKY_H */

