/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * listener.c
 * AL listener header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 04/05/2021
 */

#ifndef FR_RAYMENT_STICKY_LISTENER_H
#define FR_RAYMENT_STICKY_LISTENER_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/quat.h"
#include "sticky/math/vec3.h"

/**
 * @addtogroup audio_listener
 * @{
 */

/**
 * @brief Audio listener.
 *
 * The audio listener is responsible for receiving audio from {@link Sspeaker}
 * instances and playing it to the user. Audio exists in three dimensions thus
 * the position of the speaker or listener can have dramatic effects on the
 * resulting sound that is heard.
 *
 * @since 1.0.0
 */
typedef struct
Slistener_s
{
	Svec3 pos, vel;
	Squat rot;
	Sbool active;
} Slistener;

/**
 * @brief Create a new audio listener.
 *
 * Allocates an audio listener that can be moved around, rotated, or given a
 * velocity.
 *
 * @return A new listener.
 * @since 1.0.0
 */
STICKY_API Slistener *S_listener_new(void);

/**
 * @brief Free an audio listener from memory.
 *
 * Once this function is called for a given listener, that listener becomes
 * invalid and may not be used again in any other audio function.
 *
 * @param[in,out] listener The listener to delete.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid listener is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void       S_listener_delete(Slistener *);

/**
 * @brief Active an audio listener.
 *
 * Sets an audio listener as active. All captured sounds will be played back as
 * if they are being heard from the three-dimensional position, rotation and
 * velocity held by the active listener.
 *
 * If <c>NULL</c> is passed to this function, then no listener will be selected.
 *
 * @param[in,out] listener The listener to active.
 * @since 1.0.0
 */
STICKY_API void       S_listener_set_active(Slistener *);

/**
 * @brief Set the rotation of an audio listener.
 *
 * @param[in,out] listener The listener whose rotation shall be set.
 * @param[in,out] rot The rotation that the listener should be rotated to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid listener or quaternion
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void       S_listener_set_rot(Slistener *, const Squat *);

/**
 * @brief Set the position of an audio listener.
 *
 * @param[in,out] listener The listener whose position shall be set.
 * @param[in,out] pos The position that the listener should be set to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid listener or vector is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void       S_listener_set_pos(Slistener *, const Svec3 *);

/**
 * @brief Set the velocity of an audio listener.
 *
 * This is typically measured in metres per second.
 *
 * @param[in,out] listener The listener whose velocity shall be set.
 * @param[in,out] vel The velocity that the listener should be set to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid listener or vector is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void       S_listener_set_vel(Slistener *, const Svec3 *);

/**
 * @brief Add to the rotation of an audio listener.
 *
 * @param[in,out] listener The listener whose rotation shall be added to.
 * @param[in,out] rot The rotation that the listener should be rotated by.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid listener or quaternion
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void       S_listener_add_rot(Slistener *, const Squat *);

/**
 * @brief Add to the position of an audio listener.
 *
 * @param[in,out] listener The listener whose position shall be added to.
 * @param[in,out] pos The position that should be added to the listener's
 * current position.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid listener or vector is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void       S_listener_add_pos(Slistener *, const Svec3 *);

/**
 * @brief Add to the velocity of an audio listener.
 *
 * This is typically measured in metres per second.
 *
 * @param[in,out] listener The listener whose velocity shall be added to.
 * @param[in,out] vel The velocity that should be added to the listener's
 * current velocity.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid listener or vector is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void       S_listener_add_vel(Slistener *, const Svec3 *);

/**
 * @brief Check if a given audio listener is active.
 *
 * Check if a listener is currently active as set by the function
 * {@link S_listener_set_active(Slistener *)}.
 *
 * If <c>NULL</c> is passed to this function, then {@link S_TRUE} will be
 * returned if no listener is currently selected.
 *
 * @param[in] listener The listener.
 * @return {@link S_TRUE} If a given listener is currently active.
 * {@link S_FALSE} otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid listener is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API Sbool      S_listener_is_active(const Slistener *);

/**
 * @brief Get the current position of an audio listener.
 *
 * @param[in] listener The listener.
 * @param[out] dest The vector in which to store the current listener position.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid listener or vector is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void       S_listener_get_pos(const Slistener *, Svec3 *);

/**
 * @brief Get the current velocity of an audio listener.
 *
 * This is typically measured in metres per second.
 *
 * @param[in] listener The listener.
 * @param[out] dest The vector in which to store the current listener velocity.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid listener or vector is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void       S_listener_get_vel(const Slistener *, Svec3 *);

/**
 * @brief Get the current rotation of an audio listener.
 *
 * @param[in] listener The listener.
 * @param[out] dest The quaternion in which to store the current listener
 * rotation.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid listener or quaternion
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void       S_listener_get_rot(const Slistener *, Squat *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_LISTENER_H */

