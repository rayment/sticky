/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * speaker.h
 * AL speaker header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 30/04/2021
 */

#ifndef FR_RAYMENT_STICKY_SPEAKER_H
#define FR_RAYMENT_STICKY_SPEAKER_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <string.h>

#include "sticky/audio/sound.h"
#include "sticky/common/error.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/concurrency/mutex.h"
#include "sticky/concurrency/thread.h"
#include "sticky/math/vec3.h"

/**
 * @addtogroup speaker
 * @{
 */

/**
 * @brief Defines a static speaker.
 *
 * Static speakers may only be used with loaded audio allocated via. the
 * function {@link S_sound_load_wav(const Schar *)} or likewise.
 *
 * @since 1.0.0
 */
#define S_SPEAKER_STATIC   1
/**
 * @brief Defines a streaming speaker.
 *
 * Streaming speakers may only be used to stream audio loaded via. the function
 * {@link S_sound_stream_wav(const Schar *)} or likewise.
 *
 * @since 1.0.0
 */
#define S_SPEAKER_STREAMER 2

/**
 * @brief Audio speaker structure.
 *
 * The speaker is responsible for emitting audio which is then captured by an
 * audio listener and played to the user. Sound is emitted in three dimensions,
 * thus the position and velocity of the speaker and the position, velocity and
 * rotation of the listener can have dramatic effects on the resulting sound.
 *
 * @since 1.0.0
 */
typedef struct
Sspeaker_s
{
	ALuint source;
	Svec3 pos, vel;
	Sbool streamer;
	Smutex mutex;
	Sthread thread;
	Sbool alive;
} Sspeaker;

/**
 * @brief Create a new audio speaker.
 *
 * Allocates an audio speaker that can be moved around, given a velocity, and
 * emits sound.
 *
 * If the parameter @p mode is set to {@link S_SPEAKER_STATIC}, then the speaker
 * is <b>only</b> capable of playing static sounds, that is, sound loaded via.
 * the function {@link S_sound_load_wav(const Schar *)} or similar.
 *
 * If the parameter @p mode is set to {@link S_SPEAKER_STREAMER}, then the
 * speaker is <b>only</b> capable of playing streamed sounds, that is, sound
 * that is streamed from it's file via. the function
 * {@link S_sound_stream_wav(const Schar *)} or similar.
 *
 * @param[in] mode The speaker mode.
 * @return A new speaker.
 * @exception S_INVALID_ENUM If an invalid mode is provided to the function.
 * @since 1.0.0
 */
Sspeaker *S_speaker_new(Senum);

/**
 * @brief Free an audio speaker from memory.
 *
 * Once this function is called for a given speaker, that speaker becomes
 * invalid and may not be used again in any other speaker or audio function.
 *
 * Note that calling this function will not free the currently associated sound
 * that is being played. The sound must be deleted separately.
 *
 * @param[in,out] speaker The speaker to delete.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker is provided tp
 * the function.
 * @since 1.0.0
 */
void      S_speaker_delete(Sspeaker *);

/**
 * @brief Play a sound through an audio speaker.
 *
 * Plays a sound through a speaker from the beginning. If the sound and speaker
 * are static, the sound is played directly from memory. If the sound and
 * speaker are streams, then a new thread is created which is used to load
 * chunks of audio into small buffers, queue them and play the sound they
 * contain dynamically.
 *
 * @param[in,out] speaker The speaker to play the sound through.
 * @param[in,out] sound The sound to be played.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker or sound is
 * provided to the function.
 * @since 1.0.0
 */
void      S_speaker_play(Sspeaker *, Ssound *);

/**
 * @brief Pause an audio speaker.
 *
 * Pauses the sound being played through an audio speaker without resetting the
 * playback head to the start. Calling {@link S_speaker_resume(Sspeaker *)} will
 * immediately continue playback from it's current location.
 *
 * @param[in,out] speaker The speaker to pause.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker is provided to
 * the function.
 * @since 1.0.0
 */
void      S_speaker_pause(Sspeaker *);

/**
 * @brief Resume playback through an audio speaker.
 *
 * Resumes playback through an audio speaker. If no playback occurred before the
 * resume, then the sound is simply played from the start.
 *
 * @param[in,out] speaker The speaker to resume.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker is provided to
 * the function.
 * @since 1.0.0
 */
void      S_speaker_resume(Sspeaker *);

/**
 * @brief Stop playback through an audio speaker.
 *
 * Stops playback through an audio speaker and resets the playback head to the
 * beginning of the audio clip.
 *
 * @param[in,out] speaker The speaker to stop.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker is provided to
 * the function.
 * @since 1.0.0
 */
void      S_speaker_stop(Sspeaker *);

/**
 * @brief Set the position of an audio speaker.
 *
 * @param[in,out] speaker The speaker whose position shall be set.
 * @param[in,out] pos The position that the speaker should be set to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker or vector is
 * provided to the function.
 * @since 1.0.0
 */
void      S_speaker_set_pos(Sspeaker *, const Svec3 *);

/**
 * @brief Set the velocity of an audio speaker.
 *
 * This is typically measured in metres per second.
 *
 * @param[in,out] speaker The speaker whose velocity shall be set.
 * @param[in,out] vel The velocity that the speaker should be set to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker or vector is
 * provided to the function.
 * @since 1.0.0
 */
void      S_speaker_set_vel(Sspeaker *, const Svec3 *);

/**
 * @brief Add to the position of an audio speaker.
 *
 * @param[in,out] speaker The speaker whose position shall be added to.
 * @param[in,out] pos The position to be added to the speaker's current
 * position.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker or vector is
 * provided to the function.
 * @since 1.0.0
 */
void      S_speaker_add_pos(Sspeaker *, const Svec3 *);

/**
 * @brief Add to the velocity of an audio speaker.
 *
 * @param[in,out] speaker The speaker whose velocity shall be added to.
 * @param[in,out] vel The velocity to be added to the speaker's current
 * velocity.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker or vector is
 * provided to the function.
 * @since 1.0.0
 */
void      S_speaker_add_vel(Sspeaker *, const Svec3 *);

/**
 * @brief Get the current position of an audio speaker.
 *
 * @param[in] speaker The speaker.
 * @param[out] dest The vector in which to store the current speaker position.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker or vector is
 * provided to the function.
 * @since 1.0.0
 */
void      S_speaker_get_pos(const Sspeaker *, Svec3 *);

/**
 * @brief Get the current velocity of an audio speaker.
 *
 * @param[in] speaker The speaker.
 * @param[out] dest The vector in which to store the current speaker velocity.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker or vector is
 * provided to the function.
 * @since 1.0.0
 */
void      S_speaker_get_vel(const Sspeaker *, Svec3 *);

/**
 * @brief Check if a speaker is playing.
 *
 * @param[in] speaker The speaker.
 * @return {@link S_TRUE} If a given speaker is currently emitting a sound.
 * Otherwise {@link S_FALSE}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker is provided to
 * the function.
 * @since 1.0.0
 */
Sbool     S_speaker_is_playing(const Sspeaker *);

/**
 * @brief Check if a speaker is paused.
 *
 * @param[in] speaker The speaker.
 * @return {@link S_TRUE} If a given speaker is currently paused. Otherwise
 * {@link S_FALSE}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker is provided to
 * the function.
 * @since 1.0.0
 */
Sbool     S_speaker_is_paused(const Sspeaker *);

/**
 * @brief Check if a speaker is stopped.
 *
 * @param[in] speaker The speaker.
 * @return {@link S_TRUE} If a given speaker is currently stopped. Otherwise
 * {@link S_FALSE}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid speaker is provided to
 * the function.
 * @since 1.0.0
 */
Sbool     S_speaker_is_stopped(const Sspeaker *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_SPEAKER_H */

