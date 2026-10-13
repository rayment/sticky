/*
 * source.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include <AL/al.h>
#include <AL/alext.h>

#include "audio/source.h"

st::audio::source::source() noexcept
{
    static_cast<void>(alGetError());
    alGenSources(1, &m_source);
    m_valid = alGetError() == AL_NO_ERROR;
    if (!m_valid)
    {
        m_source = 0;
        return;
    }

    alSourcei(m_source, AL_SOURCE_RELATIVE, AL_FALSE);
    alSourcef(m_source, AL_REFERENCE_DISTANCE, 1.0f);
    alSourcef(m_source, AL_MAX_DISTANCE, 1000.0f);
    alSourcef(m_source, AL_ROLLOFF_FACTOR, 1.0f);
    alSourcei(m_source, AL_SOURCE_SPATIALIZE_SOFT, AL_TRUE);
}

st::audio::source::~source() noexcept
{
    if (!m_valid)
        return;
    // The buffer is detached before the sound it belongs to can be released.
    alSourceStop(m_source);
    alSourcei(m_source, AL_BUFFER, 0);
    alDeleteSources(1, &m_source);
}

st::bool_t
st::audio::source::set_sound(const std::shared_ptr<st::audio::sound> &sound) noexcept
{
    if (!m_valid)
        return false;
    alSourceStop(m_source);
    alSourcei(m_source, AL_BUFFER, sound != nullptr ? static_cast<ALint>(sound->m_buffer) : 0);
    m_sound = sound;
    return true;
}

st::bool_t
st::audio::source::set_sound(const st::audio::sound_handle &sound) noexcept
{
    return set_sound(sound.shared());
}

st::bool_t
st::audio::source::play() noexcept
{
    if (!m_valid)
        return false;
    alSourceRewind(m_source);
    alSourcePlay(m_source);
    return true;
}

st::bool_t
st::audio::source::pause() noexcept
{
    if (!m_valid)
        return false;
    alSourcePause(m_source);
    return true;
}

st::bool_t
st::audio::source::resume() noexcept
{
    if (!m_valid)
        return false;
    alSourcePlay(m_source);
    return true;
}

st::bool_t
st::audio::source::stop() noexcept
{
    if (!m_valid)
        return false;
    alSourceStop(m_source);
    return true;
}

st::bool_t
st::audio::source::is_playing() const noexcept
{
    ALint state = AL_INITIAL;
    if (m_valid)
        alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

st::bool_t
st::audio::source::is_paused() const noexcept
{
    ALint state = AL_INITIAL;
    if (m_valid)
        alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_PAUSED;
}

st::bool_t
st::audio::source::is_stopped() const noexcept
{
    ALint state = AL_INITIAL;
    if (m_valid)
        alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    return state == AL_STOPPED;
}

st::bool_t
st::audio::source::set_position(glm::f64vec3 position) noexcept
{
    if (!m_valid)
        return false;
    alSource3f(m_source, AL_POSITION, static_cast<ALfloat>(position.x), static_cast<ALfloat>(position.y),
               static_cast<ALfloat>(position.z));
    return true;
}

st::bool_t
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
st::audio::source::set_position(st::float64 x,
                                st::float64 y,
                                st::float64 z) noexcept
{
    return set_position(glm::f64vec3(x, y, z));
}

st::bool_t
st::audio::source::set_velocity(glm::f64vec3 velocity) noexcept
{
    if (!m_valid)
        return false;
    alSource3f(m_source, AL_VELOCITY, static_cast<ALfloat>(velocity.x), static_cast<ALfloat>(velocity.y),
               static_cast<ALfloat>(velocity.z));
    return true;
}

st::bool_t
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
st::audio::source::set_velocity(st::float64 x,
                                st::float64 y,
                                st::float64 z) noexcept
{
    return set_velocity(glm::f64vec3(x, y, z));
}

st::bool_t
st::audio::source::set_pitch(st::float32 pitch) noexcept
{
    if (!m_valid)
        return false;
    alSourcef(m_source, AL_PITCH, pitch);
    return true;
}

st::bool_t
st::audio::source::set_gain(st::float32 gain) noexcept
{
    if (!m_valid)
        return false;
    alSourcef(m_source, AL_GAIN, gain);
    return true;
}

st::bool_t
st::audio::source::set_loop(st::bool_t loop) noexcept
{
    if (!m_valid)
        return false;
    alSourcei(m_source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    return true;
}

st::bool_t
st::audio::source::set_spatial(st::bool_t spatial) noexcept
{
    if (!m_valid)
        return false;
    alSourcei(m_source, AL_SOURCE_SPATIALIZE_SOFT, spatial ? AL_TRUE : AL_FALSE);
    return true;
}
