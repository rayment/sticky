/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * sound.h
 * AL sound header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 30/04/2021
 */

#ifndef FR_RAYMENT_STICKY_SOUND_H
#define FR_RAYMENT_STICKY_SOUND_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/includes.h"
#include "sticky/common/types.h"

#define S_SOUND_STREAM_BUFFERS 4     /* number of buffers to split the stream */
#define S_SOUND_STREAM_SIZE    65536 /* 64kb mono, 32kb stereo */

/**
 * @addtogroup sound
 * @{
 */

/* NOTE: Ssound has opaque data because it contains dr_libs data which has to be
         built-in at compile-time. See src/audio/sound.c */
typedef struct _Ssound_opaque_data_s _Ssound_opaque_data;

/**
 * @brief Audio structure.
 *
 * The sound structure defines a buffer of audio that is either loaded directly
 * into memory, or alternatively streamed from file, that is, use a second
 * thread to stream data from a file into memory while the sound is playing to
 * ensure minimal heap usage without having to stop playback to load the next
 * portion of an audio clip.
 *
 * @since 1.0.0
 */
typedef struct
Ssound_s
{
	Suint32 sample_rate;
	Suint16 channels;
	Suint16 bits_per_sample;
	Ssize_t size;
	Sbool stream;

	ALuint buffer[S_SOUND_STREAM_BUFFERS];
	Ssize_t queue;
	ALenum format;
	Sfloat pitch, gain;

	_Ssound_opaque_data *data;
} Ssound;

/**
 * @brief Load audio from a WAV file.
 *
 * Loads the entirety of a WAV file into memory by allocating space on the heap
 * for it and then extracting the frames.
 *
 * The following audio formats are supported:
 * <ul>
 *     <li>1 channel, 8 bits per sample (mono)</li>
 *     <li>1 channel, 16 bits per sample (mono)</li>
 *     <li>2 channel, 8 bits per sample (stereo)</li>
 *     <li>2 channel, 16 bits per sample (stereo)</li>
 * </ul>
 *
 * This function should only be used on relatively small or repeated sound
 * effects.
 *
 * To stream a file instead of loading the entire file into memory, see
 * {@link S_sound_stream_wav(const Schar *)}. Note that this second function is
 * highly recommended for large files such as music or long sound effects to
 * avoid hogging memory.
 *
 * @param[in] filename The file from which to load the WAV file.
 * @return A newly allocated sound in memory containing the audio from a given
 * WAV file.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid string is provided to
 * the function.
 * @exception S_INVALID_FORMAT If the requested audio file does not contain 8 or
 * 16 bits per sample and is not in mono or stereo format.
 * @since 1.0.0
 */
STICKY_API Ssound  *S_sound_load_wav(const Schar *);

/**
 * @brief Stream audio from a WAV file.
 *
 * Prepares a WAV file for reading by creating an input stream for it and
 * allocating a small buffer for the file.
 *
 * The following audio formats are supported:
 * <ul>
 *     <li>1 channel, 8 bits per sample (mono)</li>
 *     <li>1 channel, 16 bits per sample (mono)</li>
 *     <li>2 channel, 8 bits per sample (stereo)</li>
 *     <li>2 channel, 16 bits per sample (stereo)</li>
 * </ul>
 *
 * This function should be used for music and other long-duration sound effects
 * as loading their entire contents into memory can have negative effects on
 * program performance.
 *
 * To load a file entirely into memory rather than streaming it dynamically on a
 * second thread, see {@link S_sound_load_wav(const Schar *)}.
 *
 * @param[in] filename The file from which to load the WAV file.
 * @return A newly allocated sound stream for a given WAV file.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid string is provided to
 * the function.
 * @exception S_INVALID_FORMAT If the requested audio file does not contain 8 or
 * 16 bits per sample and is not in mono or stereo format.
 * @since 1.0.0
 */
STICKY_API Ssound  *S_sound_stream_wav(const Schar *);

/**
 * @brief Free audio from memory.
 *
 * Free the block of memory that an audio clip resides in, or the buffer in
 * which it is streamed into.
 *
 * Once this function is called for a given sound, that sound becomes invalid
 * and may not be used again in any other audio function. It must be loaded
 * again from memory.
 *
 * @warning Any sound speaker that makes reference to a given sound must be
 * stopped (and another sound loaded before playing again) before calling this
 * function, or else this function as well as the output by the speaker is
 * undefined.
 * @param[in,out] sound The sound to free from memory.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid sound is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void     S_sound_delete(Ssound *);

/**
 * @brief Set the pitch of a sound.
 *
 * The parameter @p pitch must be more than zero. <c>1.0f</c> is the identity
 * pitch. Each doubling of pitch is an increase by 12 semitones, and conversely
 * each halving is a decrease by 12 semitones.
 *
 * @param[in,out] sound The sound.
 * @param[in] pitch The pitch to set for the sound.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid sound is provided to
 * the function or @p pitch is less than or equal to zero.
 * @since 1.0.0
 */
STICKY_API void     S_sound_set_pitch(Ssound *, Sfloat);

/**
 * @brief Set the gain of a sound.
 *
 * The parameter @p gain must be between <c>0.0f</c> and <c>1.0f</c>.
 *
 * @param[in,out] sound The sound.
 * @param[in] gain The gain to set for the sound.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid sound is provided to
 * the function or @p gain is not between zero and one.
 * @since 1.0.0
 */
STICKY_API void     S_sound_set_gain(Ssound *, Sfloat);

/**
 * @brief Get the pitch of a sound.
 *
 * @param[in] sound The sound.
 * @return The pitch of the sound.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid sound is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sfloat   S_sound_get_pitch(const Ssound *);

/**
 * @brief Get the gain of a sound.
 *
 * @param[in] sound The sound.
 * @return The gain of the sound.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid sound is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sfloat   S_sound_get_gain(const Ssound *);

void    _S_sound_init(void);
void    _S_sound_free(void);

void    _S_sound_stream_reset_wav(Ssound *);
Suint64 _S_sound_stream_buffer_wav(Ssound *, Sint16 *, Ssize_t, int);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_SOUND_H */

