/*
 * client.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include "st_includes.h"
#include "st_macros.h"
#include "audio/client.h"

st_bool
st_audio_client_create(st_audio_client_t *cli)
{
    assert(cli);
    cli->position = ST_VEC3_FLOAT64_ZERO;
    cli->velocity = ST_VEC3_FLOAT64_ZERO;
    return true;
}

st_bool
st_audio_client_destroy(ST_UNUSED st_audio_client_t cli)
{
    return true;
}

st_bool
st_audio_client_select(st_audio_client_t cli)
{
    alListener3f(AL_POSITION, ST_CAST(st_float32, cli.position.x), ST_CAST(st_float32, cli.position.y), ST_CAST(st_float32, cli.position.z));
    alListener3f(AL_VELOCITY, ST_CAST(st_float32, cli.velocity.x), ST_CAST(st_float32, cli.velocity.y), ST_CAST(st_float32, cli.velocity.z));
    return true;
}

st_bool
st_audio_client_set_position(st_audio_client_t *cli,
                             st_vec3_float64_t position)
{
    cli->position = position;
    return true;
}

st_bool
st_audio_client_set_velocity(st_audio_client_t *cli,
                             st_vec3_float64_t velocity)
{
    cli->velocity = velocity;
    return true;
}
