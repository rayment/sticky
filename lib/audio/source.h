/*
 * source.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#ifndef STICKY_AUDIO_SOURCE_H
#define STICKY_AUDIO_SOURCE_H

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "audio/sound.h"
#include "audio/sound_handle.h"
#include "st_macros.h"

namespace st::audio
{
    class source
    {
    public:
        source() noexcept;
        ~source() noexcept;

        ST_DELETE_COPY_CTORS(source);
        ST_DELETE_MOVE_CTORS(source);

        st::bool_t select(const st::audio::sound &sound) noexcept;
        st::bool_t select(const st::audio::sound_handle &sound) noexcept;
        st::bool_t play() noexcept;
        st::bool_t pause() noexcept;
        st::bool_t resume() noexcept;
        st::bool_t stop() noexcept;

        [[nodiscard]] st::bool_t is_playing() const noexcept;
        [[nodiscard]] st::bool_t is_paused() const noexcept;
        [[nodiscard]] st::bool_t is_stopped() const noexcept;

        st::bool_t set_position(glm::f64vec3 position) noexcept;
        st::bool_t set_position(st::float64 x,
                                st::float64 y,
                                st::float64 z) noexcept;
        st::bool_t set_velocity(glm::f64vec3 velocity) noexcept;
        st::bool_t set_velocity(st::float64 x,
                                st::float64 y,
                                st::float64 z) noexcept;
        st::bool_t set_pitch(st::float32 pitch) noexcept;
        st::bool_t set_gain(st::float32 gain) noexcept;
        st::bool_t set_loop(st::bool_t loop) noexcept;
        st::bool_t set_spatial(st::bool_t spatial) noexcept;

    private:
        glm::f64vec3 m_position;
        glm::f64vec3 m_velocity;
        st::float32  m_pitch;
        st::float32  m_gain;
        st::bool_t   m_loop;
        st::bool_t   m_spatial;
        st::uint32   m_source;
    };
} // namespace st::audio

#endif // STICKY_AUDIO_SOURCE_H
