/*
 * client.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include <AL/al.h>

#include "audio/client.h"

st::audio::client::client() noexcept
    : m_position(0, 0, 0), m_velocity(0, 0, 0)
{}

st::audio::client::~client() noexcept
{}

void
st::audio::client::select() noexcept
{
    alListener3f(
        AL_POSITION,
        static_cast<ALfloat>(m_position.x),
        static_cast<ALfloat>(m_position.y),
        static_cast<ALfloat>(m_position.z));
    alListener3f(
        AL_VELOCITY,
        static_cast<ALfloat>(m_velocity.x),
        static_cast<ALfloat>(m_velocity.y),
        static_cast<ALfloat>(m_velocity.z));
}

void
st::audio::client::set_position(const glm::f64vec3 &position) noexcept
{
    m_position = position;
}

void
st::audio::client::set_velocity(const glm::f64vec3 &velocity) noexcept
{
    m_velocity = velocity;
}

glm::f64vec3
st::audio::client::position() const noexcept
{
    return m_position;
}

glm::f64vec3
st::audio::client::velocity() const noexcept
{
    return m_velocity;
}
