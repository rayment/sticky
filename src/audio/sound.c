/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * sound.c
 * AL sound source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 30/04/2021
 */

#include <string.h>

#include "sticky/audio/sound.h"
#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/math.h"
#include "sticky/memory/allocator.h"

#define DR_WAV_IMPLEMENTATION
#include "sticky/dr_libs/dr_wav.h"

static ALCdevice *dev;
static ALCcontext *context;

void
_S_sound_init(void)
{
	_S_ALC(dev = alcOpenDevice(NULL), dev);
	_S_ALC(context = alcCreateContext(dev, NULL), dev);
	_S_ALC(alcMakeContextCurrent(context), dev);
}

void
_S_sound_free(void)
{
	_S_ALC(alcMakeContextCurrent(NULL), dev);
	_S_ALC(alcDestroyContext(context), dev);
	alcCloseDevice(dev);
}

static
Sbool
_S_sound_set_format(Ssound *sound)
{
	if (!sound)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_sound_set_format");
		return S_FALSE;
	}
	if (sound->channels == 1 && sound->bits_per_sample == 8)
		sound->format = AL_FORMAT_MONO8;
	else if (sound->channels == 1 && sound->bits_per_sample == 16)
		sound->format = AL_FORMAT_MONO16;
	else if (sound->channels == 2 && sound->bits_per_sample == 8)
		sound->format = AL_FORMAT_STEREO8;
	else if (sound->channels == 2 && sound->bits_per_sample == 16)
		sound->format = AL_FORMAT_STEREO16;
	else
		return S_FALSE;
	return S_TRUE;
}

void
_S_sound_stream_reset_wav(Ssound *sound)
{
	if (!sound)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_sound_stream_reset_wav");
	}
	else
	{
		drwav_seek_to_pcm_frame(&sound->wav, 0);
	}
}

/*
 * Function takes *data and data_size because it is used in such a way that the
 * same block of memory is set multiple times by a for-loop for buffer queueing
 * so it's better to alloc/free once outside this function rather than every
 * single buffer.
 */
Suint64
_S_sound_stream_buffer_wav(Ssound *sound,
                           Sint16 *data,
                           Ssize_t data_size,
                           int buf)
{
	Ssize_t tmp;
	Suint64 num_frames, rd;

	if (!sound || !data || data_size == 0 || buf == 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_sound_stream_buffer_wav");
		return 0;
	}
	/* bytes = frame count * channels * sizeof(Sint16)
	   therefore:
	   max frame count = buffer size / (channels * sizeof(Sint16)) */
	num_frames = S_SOUND_STREAM_SIZE / (sound->channels * sizeof(Sint16));
	rd = drwav_read_pcm_frames_s16(&sound->wav, num_frames, data);
	if (rd > 0)
	{
		tmp = rd * sizeof(Sint16) * sound->channels;
		if (tmp < data_size)
			data_size = tmp;
		_S_AL(alBufferData(buf, sound->format, data,
		                   data_size, (ALsizei) sound->sample_rate));
	}
	return rd * sound->channels * sizeof(Sint16);
}

static
Ssound *
_S_sound_load_wav(const Schar *filename)
{
	Ssound *sound;
	Sbool b;

	if (!filename)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_sound_load_wav");
		return NULL;
	}
	sound = (Ssound *) S_memory_new(sizeof(Ssound));

	if (!drwav_init_file(&sound->wav, filename, NULL))
		_S_error_dr("drwav_init_file", "Failed to load WAV");

	if (sound->wav.bitsPerSample != 8 && sound->wav.bitsPerSample != 16)
	{
		_S_SET_ERROR(S_INVALID_FORMAT, "_S_sound_load_wav");
		return NULL;
	}

	sound->channels = sound->wav.channels;
	sound->sample_rate = sound->wav.sampleRate;
	sound->pitch = 1.0f;
	sound->gain = 1.0f;
	/* converted to signed 16 */
	sound->bits_per_sample = 16;/*sound->wav.bitsPerSample;*/
	_S_CALL("_S_sound_set_format", b = _S_sound_set_format(sound));
	if (!b)
	{
		S_memory_delete(sound);
		_S_SET_ERROR(S_INVALID_FORMAT, "_S_sound_load_wav");
		S_warning("Tried to load unsupported WAV: (%d channels, %d bits)\n",
		          sound->channels, sound->bits_per_sample);
		return NULL;
	}
	sound->size = ((Ssize_t) sound->wav.totalPCMFrameCount) *
	              sound->channels * sizeof(Sint16);

	return sound;
}

Ssound *
S_sound_load_wav(const Schar *filename)
{
	Ssound *sound;
	Sint16 *data;

	if (!filename)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_sound_load_wav");
		return NULL;
	}
	_S_CALL("_S_sound_load_wav", sound = _S_sound_load_wav(filename));
	sound->stream = S_FALSE;
	sound->queue = 0;
	_S_AL(alGenBuffers(1, sound->buffer));
	data = (Sint16 *) S_memory_new(sound->size);
	drwav_read_pcm_frames_s16(&sound->wav, sound->wav.totalPCMFrameCount, data);
	_S_AL(alBufferData(*(sound->buffer), sound->format, data,
	                   sound->size, (ALsizei) sound->sample_rate));
	S_memory_delete(data);
	drwav_uninit(&sound->wav);

	return sound;
}

Ssound *
S_sound_stream_wav(const Schar *filename)
{
	Ssound *sound;

	if (!filename)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_sound_stream_wav");
		return NULL;
	}
	_S_CALL("_S_sound_load_wav", sound = _S_sound_load_wav(filename));
	sound->stream = S_TRUE;
	sound->queue = 0;
	_S_AL(alGenBuffers(S_SOUND_STREAM_BUFFERS, sound->buffer));

	return sound;
}

void
S_sound_delete(Ssound *sound)
{
	if (!sound)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_sound_destroy");
		return;
	}
	if (sound->stream)
	{
		drwav_uninit(&sound->wav);
		_S_AL(alDeleteBuffers(S_SOUND_STREAM_BUFFERS, sound->buffer));
	}
	else
	{
		_S_AL(alDeleteBuffers(1, sound->buffer));
	}
	S_memory_delete(sound);
}

void
S_sound_set_pitch(Ssound *sound,
                  Sfloat pitch)
{
	if (!sound || pitch <= 0.0f)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_sound_set_pitch");
	}
	else
	{
		sound->pitch = pitch;
	}
}

void
S_sound_set_gain(Ssound *sound,
                 Sfloat gain)
{
	if (!sound || gain < 0.0f || gain > 1.0f)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_sound_set_gain");
	}
	else
	{
		sound->gain = gain;
	}
}

Sfloat
S_sound_get_pitch(const Ssound *sound)
{
	if (!sound)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_sound_get_pitch");
		return 0.0f;
	}
	else
	{
		return sound->pitch;
	}
}

Sfloat
S_sound_get_gain(const Ssound *sound)
{
	if (!sound)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_sound_get_gain");
		return 0.0f;
	}
	else
	{
		return sound->gain;
	}
}

