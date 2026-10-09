/*
 * source.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include "audio/source.h"

#include "AL/alext.h"

st::audio::source::source() noexcept
    : m_position(0, 0, 0),
      m_velocity(0, 0, 0),
      m_pitch(1.0f),
      m_gain(1.0f),
      m_loop(false),
      m_spatial(true)
{
    alGenSources(1, &m_source);
    alSourcei(m_source, AL_SOURCE_RELATIVE, AL_FALSE);
    // TODO: Allow configuration of distance and rolloff.
    alSourcef(m_source, AL_REFERENCE_DISTANCE, 1.0f);
    alSourcef(m_source, AL_MAX_DISTANCE, 1000.0f);
    alSourcef(m_source, AL_ROLLOFF_FACTOR, 1.0f);
}

st::audio::source::~source() noexcept
{
    alDeleteSources(1, &m_source);
}

st::bool_t
st::audio::source::set_position(glm::f64vec3 position) noexcept
{
    m_position = position;
    alSource3f(
        m_source,
        AL_POSITION,
        static_cast<ALfloat>(m_position.x),
        static_cast<ALfloat>(m_position.y),
        static_cast<ALfloat>(m_position.z)
    );
    return true;
}

st::bool_t
st::audio::source::set_velocity(glm::f64vec3 velocity) noexcept
{

    m_velocity = velocity;
    alSource3f(
        m_source,
        AL_VELOCITY,
        static_cast<ALfloat>(m_velocity.x),
        static_cast<ALfloat>(m_velocity.y),
        static_cast<ALfloat>(m_velocity.z)
    );
    return true;
}

st::bool_t
st::audio::source::set_pitch(st::float32 pitch) noexcept
{
    m_pitch = pitch;
    alSourcef(m_source, AL_PITCH, m_pitch);
    return true;
}

st::bool_t
st::audio::source::set_gain(st::float32 gain) noexcept
{
    m_gain = gain;
    alSourcef(m_source, AL_GAIN, m_gain);
    return true;
}

st::bool_t
st::audio::source::set_loop(st::bool_t loop) noexcept
{
    m_loop = loop;
    alSourcei(m_source, AL_LOOPING, m_loop ? AL_TRUE : AL_FALSE);
    return true;
}

st::bool_t
st::audio::source::set_spatial(st::bool_t spatial) noexcept
{
    // TODO: This forces stereo sounds to work. OpenAL does not spatialise stereo by default.
    // Need to instead allow both spatialised and non-spatialised stereo (by loading as mono instead, not using this line)
    //   and also spatialised mono. This line eventually thus needs to be removed.
    m_spatial = spatial;
    alSourcei(m_source, AL_SOURCE_SPATIALIZE_SOFT, spatial ? AL_TRUE : AL_FALSE);
    return true;
}

st::bool_t
st::audio::source::select(const st::audio::sound &sound) noexcept
{
    alSourcei(m_source, AL_BUFFER, sound.m_buffer);
    return true;
}

st::bool_t
st::audio::source::play() noexcept
{
    alSourceRewind(m_source);
    alSourcePlay(m_source);
    return true;
}

st::bool_t
st::audio::source::pause() noexcept
{
    alSourcePause(m_source);
    return true;
}

st::bool_t
st::audio::source::resume() noexcept
{
    alSourcePlay(m_source);
    return true;
}

st::bool_t
st::audio::source::stop() noexcept
{
    alSourceStop(m_source);
    return true;
}

st::bool_t
st::audio::source::is_playing() const noexcept
{
    ALint state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

st::bool_t
st::audio::source::is_paused() const noexcept
{
    ALint state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
}

st::bool_t
st::audio::source::is_stopped() const noexcept
{
    ALint state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}
