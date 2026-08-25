/*
 * source.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#ifndef STICKY_AUDIO_SOURCE_H
#define STICKY_AUDIO_SOURCE_H

#include "st_includes.h"
#include "audio/sound.h"
#include "math/vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct st_audio_source
{
    st_vec3_float64_t position;
    st_vec3_float64_t velocity;
    st_float32 pitch;
    st_float32 gain;
    st_bool loop;

    ALuint source;
} st_audio_source_t;

st_bool st_audio_source_create      (st_audio_source_t *src);
st_bool st_audio_source_destroy     (st_audio_source_t  src);
st_bool st_audio_source_select      (st_audio_source_t  src, st_audio_sound_t  sound);
st_bool st_audio_source_set_position(st_audio_source_t *src, st_vec3_float64_t position);
st_bool st_audio_source_set_velocity(st_audio_source_t *src, st_vec3_float64_t velocity);
st_bool st_audio_source_set_pitch   (st_audio_source_t *src, st_float32        pitch);
st_bool st_audio_source_set_gain    (st_audio_source_t *src, st_float32        gain);
st_bool st_audio_source_set_loop    (st_audio_source_t *src, st_bool           loop);
st_bool st_audio_source_play        (st_audio_source_t  src);
st_bool st_audio_source_pause       (st_audio_source_t  src);
st_bool st_audio_source_resume      (st_audio_source_t  src);
st_bool st_audio_source_stop        (st_audio_source_t  src);
st_bool st_audio_source_is_playing  (st_audio_source_t  src);
st_bool st_audio_source_is_paused   (st_audio_source_t  src);
st_bool st_audio_source_is_stopped  (st_audio_source_t  src);

#ifdef __cplusplus
}
#endif

#endif // STICKY_AUDIO_SOURCE_H
