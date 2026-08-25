/*
 * client.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#ifndef STICKY_AUDIO_CLIENT_H
#define STICKY_AUDIO_CLIENT_H

#include "st_primitives.h"
#include "math/vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct st_audio_client
{
    st_vec3_float64_t position;
    st_vec3_float64_t velocity;
} st_audio_client_t;

st_bool st_audio_client_create      (st_audio_client_t *cli);
st_bool st_audio_client_destroy     (st_audio_client_t  cli);
st_bool st_audio_client_select      (st_audio_client_t  cli);
st_bool st_audio_client_set_position(st_audio_client_t *cli, st_vec3_float64_t position);
st_bool st_audio_client_set_velocity(st_audio_client_t *cli, st_vec3_float64_t velocity);

#ifdef __cplusplus
}
#endif

#endif // STICKY_AUDIO_CLIENT_H
