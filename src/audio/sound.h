/*
 * sound.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#ifndef STICKY_AUDIO_SOUND_H
#define STICKY_AUDIO_SOUND_H

#include "st_includes.h"
#include "st_primitives.h"
#include "type/string.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct st_audio_sound
{
    ALuint buffer;
    st_bool valid;
} st_audio_sound_t;

st_bool st_audio_sound_load_file(st_audio_sound_t *sound, st_string_t path);
st_bool st_audio_sound_destroy  (st_audio_sound_t  sound);

#ifdef __cplusplus
}
#endif

#endif // STICKY_AUDIO_SOUND_H
