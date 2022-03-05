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

#include <dr_libs/dr_wav.h>

#include "sticky/common/includes.h"
#include "sticky/common/types.h"

#define S_SOUND_STREAM_BUFFERS 4
#define S_SOUND_STREAM_SIZE    65536 /* 64kb mono, 32kb stereo */

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

	drwav wav;
} Ssound;

void    _S_sound_init(void);
void    _S_sound_free(void);

Ssound  *S_sound_load_wav(const Schar *);
Ssound  *S_sound_stream_wav(const Schar *);
void     S_sound_delete(Ssound *);

void     S_sound_set_pitch(Ssound *, Sfloat);
void     S_sound_set_gain(Ssound *, Sfloat);

Sfloat   S_sound_get_pitch(const Ssound *);
Sfloat   S_sound_get_gain(const Ssound *);

void    _S_sound_stream_reset_wav(Ssound *);
Suint64 _S_sound_stream_buffer_wav(Ssound *, Sint16 *, Ssize_t, int);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_SOUND_H */

