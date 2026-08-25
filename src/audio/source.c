/*
 * source.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include "st_includes.h"
#include "st_macros.h"
#include "audio/source.h"

#include "alext.h"

st_bool
st_audio_source_create(st_audio_source_t *src)
{
    assert(src != nullptr);
    alGenSources(ST_CAST(ALuint, 1), &src->source);
    st_audio_source_set_position(src, ST_VEC3_FLOAT64_ZERO);
    st_audio_source_set_velocity(src, ST_VEC3_FLOAT64_ZERO);
    st_audio_source_set_pitch(src, 1.0f);
    st_audio_source_set_gain(src, 1.0f);
    st_audio_source_set_loop(src, false);
    alSourcei(src->source, AL_SOURCE_RELATIVE, AL_FALSE);
    // TODO: Allow configuration of distance and rolloff.
    alSourcef(src->source, AL_REFERENCE_DISTANCE, 1.0f);
    alSourcef(src->source, AL_MAX_DISTANCE, 1000.0f);
    alSourcef(src->source, AL_ROLLOFF_FACTOR, 1.0f);
    // TODO: This forces stereo sounds to work. OpenAL does not spatialise stereo by default.
    // Need to instead allow both spatialised and non-spatialised stereo (by loading as mono instead, not using this line)
    //   and also spatialised mono. This line eventually thus needs to be removed.
    alSourcei(src->source, AL_SOURCE_SPATIALIZE_SOFT, AL_TRUE);
    return true;
}

st_bool
st_audio_source_destroy(st_audio_source_t src)
{
    alDeleteSources(1, &src.source);
    return true;
}

st_bool
st_audio_source_set_position(st_audio_source_t *src,
                             st_vec3_float64_t position)
{
    src->position = position;
    alSource3f(src->source, AL_POSITION, ST_CAST(st_float32, src->position.x), ST_CAST(st_float32, src->position.y), ST_CAST(st_float32, src->position.z));
    return true;
}

st_bool
st_audio_source_set_velocity(st_audio_source_t *src,
                             st_vec3_float64_t velocity)
{
    src->velocity = velocity;
    alSource3f(src->source, AL_VELOCITY, ST_CAST(st_float32, src->velocity.x), ST_CAST(st_float32, src->velocity.y), ST_CAST(st_float32, src->velocity.z));
    return true;
}

st_bool
st_audio_source_set_pitch(st_audio_source_t *src,
                          st_float32 pitch)
{
    src->pitch = pitch;
    alSourcef(src->source, AL_PITCH, src->pitch);
    return true;
}

st_bool
st_audio_source_set_gain(st_audio_source_t *src,
                         st_float32 gain)
{
    src->gain = gain;
    alSourcef(src->source, AL_GAIN, src->gain);
    return true;
}

st_bool
st_audio_source_set_loop(st_audio_source_t *src,
                         st_bool loop)
{
    src->loop = loop;
    alSourcei(src->source, AL_LOOPING, src->loop ? AL_TRUE : AL_FALSE);
    return true;
}

st_bool
st_audio_source_select(st_audio_source_t src,
                       st_audio_sound_t sound)
{
    if (!sound.valid)
        return false;
    // TODO: Is the ST_CAST safe?
    alSourcei(src.source, AL_BUFFER, ST_CAST(ALint, sound.buffer));
    return true;
}

st_bool
st_audio_source_play(st_audio_source_t src)
{
    alSourceRewind(src.source);
    alSourcePlay(src.source);
    return true;
}

st_bool
st_audio_source_pause(st_audio_source_t src)
{
    alSourcePause(src.source);
    return true;
}

st_bool
st_audio_source_resume(st_audio_source_t src)
{
    alSourcePlay(src.source);
    return true;
}

st_bool
st_audio_source_stop(st_audio_source_t src)
{
    alSourceStop(src.source);
    return true;
}

st_bool
st_audio_source_is_playing(st_audio_source_t src)
{
    ALint state;
    alGetSourcei(src.source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

st_bool
st_audio_source_is_paused(st_audio_source_t src)
{
    ALint state;
    alGetSourcei(src.source, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
}

st_bool
st_audio_source_is_stopped(st_audio_source_t src)
{
    ALint state;
    alGetSourcei(src.source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}
