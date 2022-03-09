/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * speaker.c
 * AL speaker source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 30/04/2021
 */

#include <string.h>

#include "sticky/audio/speaker.h"
#include "sticky/common/error.h"
#include "sticky/concurrency/mutex.h"
#include "sticky/concurrency/thread.h"
#include "sticky/math/math.h"
#include "sticky/memory/allocator.h"

#define TIMEOUTS       5
#define TIMEOUT_MILLIS 10

struct
_Sspeaker_sound_pair
{
	Sspeaker *speaker;
	Ssound *sound;
};

static
void
_S_speaker_update_pos(Sspeaker *speaker)
{
	_S_AL(alSource3f(speaker->source, AL_POSITION,
	                 speaker->pos.x, speaker->pos.y, speaker->pos.z));
}

static
void
_S_speaker_update_vel(Sspeaker *speaker)
{
	_S_AL(alSource3f(speaker->source, AL_VELOCITY,
	                speaker->vel.x, speaker->vel.y, speaker->vel.z));
}

Sspeaker *
S_speaker_new(Senum mode)
{
	Sspeaker *speaker;
	if (mode != S_SPEAKER_STATIC && mode != S_SPEAKER_STREAMER)
	{
		_S_SET_ERROR(S_INVALID_ENUM, "S_speaker_new");
		return NULL;
	}
	speaker = (Sspeaker *) S_memory_new(sizeof(Sspeaker));

	_S_AL(alGenSources(1, &speaker->source));
	_S_CALL("S_vec3_zero", S_vec3_zero(&speaker->pos));
	_S_CALL("S_vec3_zero", S_vec3_zero(&speaker->vel));
	_S_CALL("_S_speaker_update_pos", _S_speaker_update_pos(speaker));
	_S_CALL("_S_speaker_update_vel", _S_speaker_update_vel(speaker));
	speaker->streamer = mode == S_SPEAKER_STREAMER;
	if (speaker->streamer)
	{
		_S_CALL("S_mutex_new", speaker->mutex = S_mutex_new());
		speaker->alive = S_FALSE;
		speaker->thread = NULL;
	}
	return speaker;
}

void
S_speaker_delete(Sspeaker *speaker)
{
	if (!speaker)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_speaker_destroy");
		return;
	}
	/* TODO: Mutex. */
	speaker->alive = S_FALSE;
	if (speaker->streamer)
	{
		if (speaker->thread)
		{
			/* wait on current thread if active */
			_S_CALL("S_thread_join", S_thread_join(speaker->thread));
		}
		_S_CALL("S_mutex_delete", S_mutex_delete(speaker->mutex));
	}
	_S_AL(alSourceStop(speaker->source));
	_S_AL(alDeleteSources(1, &speaker->source));
	S_memory_delete(speaker);
}

static
void
_S_audio_stream_update(Sspeaker *speaker,
                       Ssound *sound)
{
	Sint16 *data;
	Ssize_t data_size;
	ALint buffers_processed = 0;
	ALuint buffer;
	Suint64 len;

	_S_AL(alGetSourcei(speaker->source, AL_BUFFERS_PROCESSED,
	                   &buffers_processed));

	if (buffers_processed <= 0)
		return;
	data_size = sizeof(Sint16) * S_SOUND_STREAM_SIZE;
	data = (Sint16 *) S_memory_new(data_size);
	sound->queue = 0;
	while (buffers_processed--)
	{
		_S_AL(alSourceUnqueueBuffers(speaker->source, 1, &buffer));
		_S_CALL("_S_sound_stream_buffer_wav",
		        len = _S_sound_stream_buffer_wav(sound,
		                                         data,
		                                         data_size,
		                                         buffer));
		if (len < S_SOUND_STREAM_SIZE)
			memset(data+(len / sizeof(Sint16)), 0, S_SOUND_STREAM_SIZE - len);
		_S_AL(alBufferData(buffer, sound->format, data, S_SOUND_STREAM_SIZE,
		                   sound->sample_rate));
		if (len > 0){
			++sound->queue;
			_S_AL(alSourceQueueBuffers(speaker->source, 1, &buffer));
		}
	}
	S_memory_delete(data);
}

static
void *
_S_speaker_stream_thread(void *pair_void)
{
	struct _Sspeaker_sound_pair *pair;
	ALint state, al_state;
	ALuint buffer;
	Sint16 *data;
	Ssize_t data_size;
	Suint8 i;
	Suint64 frames;
	Sbool b;

	pair = (struct _Sspeaker_sound_pair *) pair_void;

	/* lock thread and wait for mutex */
	i = 0;
	_S_CALL("S_mutex_lock", S_mutex_lock(pair->speaker->mutex));

	_S_AL(alSourceStop(pair->speaker->source));
	_S_AL(alGetSourcei(pair->speaker->source, AL_BUFFERS_QUEUED, &al_state));
	for (; al_state > 0; --al_state)
	{
		_S_AL(alSourceUnqueueBuffers(pair->speaker->source, 1, &buffer));
	}
	pair->sound->queue = 0;
	pair->speaker->alive = S_TRUE;
	data_size = sizeof(Sint16) * S_SOUND_STREAM_SIZE;
	data = (Sint16 *) S_memory_new(data_size);
	_S_CALL("_S_sound_stream_reset_wav",
	        _S_sound_stream_reset_wav(pair->sound));
	for (i = 0; i < S_SOUND_STREAM_BUFFERS; ++i)
	{
		_S_CALL("_S_sound_stream_buffer_wav",
		        frames = _S_sound_stream_buffer_wav(pair->sound,
		                                            data, data_size,
			                                        pair->sound->buffer[i]));
		if (frames > 0)
			++pair->sound->queue;
		else
			break;
	}
	S_memory_delete(data);

#ifdef DEBUG
	_S_AL(alGetSourcei(pair->speaker->source, AL_SOURCE_RELATIVE, &al_state));
	fprintf(stdout, "AL_SOURCE_RELATIVE: %d\n", al_state);
	_S_AL(alGetSourcei(pair->speaker->source, AL_BUFFER, &al_state));
	fprintf(stdout, "AL_BUFFER: %d\n", al_state);
	_S_AL(alGetSourcei(pair->speaker->source, AL_SOURCE_STATE, &al_state));
	fprintf(stdout, "AL_SOURCE_STATE: %d\n", al_state);
	_S_AL(alGetSourcei(pair->speaker->source, AL_BUFFERS_QUEUED, &al_state));
	fprintf(stdout, "AL_BUFFERS_QUEUED: %d\n", al_state);
	_S_AL(alGetSourcei(pair->speaker->source, AL_BUFFERS_PROCESSED, &al_state));
	fprintf(stdout, "AL_BUFFERS_PROCESSED: %d\n", al_state);
	fprintf(stdout, "queueing %d buffers to memory location %p\n",
	        S_SOUND_STREAM_BUFFERS, (void *) pair->sound->buffer);
#endif /* DEBUG */

	_S_AL(alSourceQueueBuffers(pair->speaker->source, pair->sound->queue,
	                           pair->sound->buffer));
	_S_AL(alSourcePlay(pair->speaker->source));
	/* thread entry */
	do
	{
		/* return if the thread parent was destroyed */
		if (!pair->speaker->alive)
			break;
		_S_CALL("S_speaker_is_paused",
		        b = S_speaker_is_paused(pair->speaker));
		if (b)
		{
			/* if paused, wait a little while */
			S_thread_msleep(TIMEOUT_MILLIS);
		}
		else
		{
			/* otherwise continue streaming */
			_S_CALL("_S_audio_stream_update",
			        _S_audio_stream_update(pair->speaker, pair->sound));
		}
		_S_AL(alGetSourcei(pair->speaker->source, AL_SOURCE_STATE, &state));
	} while (state == AL_PLAYING || state == AL_PAUSED);

	/* thread exit */
	/* do not call stop on exit as another thread may have began */
	_S_CALL("S_mutex_unlock", S_mutex_unlock(pair->speaker->mutex));
	pair->speaker->alive = S_FALSE;
	S_memory_delete(pair);
	return NULL;
}

void
S_speaker_play(Sspeaker *speaker,
               Ssound *sound)
{
	struct _Sspeaker_sound_pair *pair;
	if (!speaker || !sound)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_speaker_play");
		return;
	}
	if (speaker->streamer != sound->stream)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_speaker_play");
		return;
	}
	_S_AL(alSourcef(speaker->source, AL_PITCH, sound->pitch));
	_S_AL(alSourcef(speaker->source, AL_GAIN, sound->gain));
	if (!speaker->streamer)
	{
		_S_AL(alSourceStop(speaker->source));
		_S_AL(alSourcei(speaker->source, AL_BUFFER, *sound->buffer));
		_S_AL(alSourcePlay(speaker->source));
	}
	else
	{
		pair = (struct _Sspeaker_sound_pair *)
			S_memory_new(sizeof(struct _Sspeaker_sound_pair));
		pair->speaker = speaker;
		pair->sound = sound;
		speaker->alive = S_FALSE;
		if (speaker->thread)
		{
			_S_CALL("S_thread_join", S_thread_join(speaker->thread));
		}
		_S_CALL("S_thread_new",
		        speaker->thread = S_thread_new(_S_speaker_stream_thread,
		                                       (void *) pair));
	}
}

void
S_speaker_pause(Sspeaker *speaker)
{
	Sbool b;
	if (!speaker)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_speaker_pause");
		return;
	}
	_S_CALL("S_speaker_is_playing", b = S_speaker_is_playing(speaker));
	if (b)
	{
		_S_AL(alSourcePause(speaker->source));
	}
}

void
S_speaker_resume(Sspeaker *speaker)
{
	Sbool b;
	if (!speaker)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_speaker_resume");
		return;
	}
	_S_CALL("S_speaker_is_paused", b = S_speaker_is_paused(speaker));
	if (b)
	{
		_S_AL(alSourcePlay(speaker->source));
	}
}

void
S_speaker_stop(Sspeaker *speaker)
{
	if (!speaker)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_speaker_stop");
		return;
	}
	if (!speaker->streamer)
	{
		_S_AL(alSourceStop(speaker->source));
	}
	else
	{
		speaker->alive = S_FALSE;
		if (speaker->thread)
		{
			S_thread_join(speaker->thread);
			speaker->thread = NULL;
		}
		_S_AL(alSourceStop(speaker->source));
	}
}

void
S_speaker_set_pos(Sspeaker *speaker,
                  const Svec3 *pos)
{
	if (!speaker || !pos)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_speaker_set_pos");
	}
	else
	{
		_S_CALL("S_vec3_copy", S_vec3_copy(&speaker->pos, pos));
		_S_CALL("_S_speaker_update_pos", _S_speaker_update_pos(speaker));
	}
}

void
S_speaker_add_pos(Sspeaker *speaker,
                  const Svec3 *pos)
{
	if (!speaker || !pos)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_speaker_add_pos");
	}
	else
	{
		_S_CALL("S_vec3_add", S_vec3_add(&speaker->pos, pos));
		_S_CALL("_S_speaker_update_pos", _S_speaker_update_pos(speaker));
	}
}

void
S_speaker_set_vel(Sspeaker *speaker,
                  const Svec3 *vel)
{
	if (!speaker || !vel)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_speaker_set_vel");
	}
	else
	{
		_S_CALL("S_vec3_copy", S_vec3_copy(&speaker->vel, vel));
		_S_CALL("_S_speaker_update_vel", _S_speaker_update_vel(speaker));
	}
}

void
S_speaker_add_vel(Sspeaker *speaker,
                  const Svec3 *vel)
{
	if (!speaker || !vel)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_speaker_add_vel");
	}
	else
	{
		_S_CALL("S_vec3_add", S_vec3_add(&speaker->vel, vel));
		_S_CALL("_S_speaker_update_vel", _S_speaker_update_vel(speaker));
	}
}

void
S_speaker_get_pos(const Sspeaker *speaker,
                  Svec3 *dest)
{
	if (!speaker || !dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_speaker_get_pos");
	}
	else
	{
		_S_CALL("S_vec3_copy", S_vec3_copy(dest, &speaker->pos));
	}
}

void
S_speaker_get_vel(const Sspeaker *speaker,
                  Svec3 *dest)
{
	if (!speaker || !dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_speaker_get_vel");
	}
	else
	{
		_S_CALL("S_vec3_copy", S_vec3_copy(dest, &speaker->vel));
	}
}

static
ALint
_S_speaker_get_state(const Sspeaker *speaker)
{
	ALint i;
	if (!speaker)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_speaker_get_state");
		return 0;
	}
	_S_AL(alGetSourcei(speaker->source, AL_SOURCE_STATE, &i));
	return i;
}

Sbool
S_speaker_is_playing(const Sspeaker *speaker)
{
	ALint b;
	if (!speaker)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_speaker_is_playing");
		return S_FALSE;
	}
	_S_CALL("_S_speaker_get_state", b = _S_speaker_get_state(speaker));
	return b == AL_PLAYING;
}

Sbool
S_speaker_is_paused(const Sspeaker *speaker)
{
	ALint b;
	if (!speaker)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_speaker_is_paused");
		return S_FALSE;
	}
	_S_CALL("_S_speaker_get_state", b = _S_speaker_get_state(speaker));
	return b == AL_PAUSED;
}

Sbool
S_speaker_is_stopped(const Sspeaker *speaker)
{
	ALint b;
	if (!speaker)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_speaker_is_stopped");
		return S_FALSE;
	}
	_S_CALL("_S_speaker_get_state", b = _S_speaker_get_state(speaker));
	return b == AL_INITIAL || b == AL_STOPPED;
}

