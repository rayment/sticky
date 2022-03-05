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

#define S_SPEAKER_STATIC   1
#define S_SPEAKER_STREAMER 2

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

Sspeaker *S_speaker_new(Senum);
void      S_speaker_delete(Sspeaker *);

void      S_speaker_play(Sspeaker *, Ssound *);
void      S_speaker_pause(Sspeaker *);
void      S_speaker_resume(Sspeaker *);
void      S_speaker_stop(Sspeaker *);

void      S_speaker_set_pos(Sspeaker *, const Svec3 *);
void      S_speaker_set_vel(Sspeaker *, const Svec3 *);
void      S_speaker_add_pos(Sspeaker *, const Svec3 *);
void      S_speaker_add_vel(Sspeaker *, const Svec3 *);

void      S_speaker_get_pos(const Sspeaker *, Svec3 *);
void      S_speaker_get_vel(const Sspeaker *, Svec3 *);
Sbool     S_speaker_get_playing(const Sspeaker *);
Sbool     S_speaker_get_paused(const Sspeaker *);
Sbool     S_speaker_get_stopped(const Sspeaker *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_SPEAKER_H */

