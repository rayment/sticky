/*
 * source.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

/**
 * @file
 * @brief A positioned emitter that plays sounds.
 */

#ifndef STICKY_AUDIO_SOURCE_H
#define STICKY_AUDIO_SOURCE_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <memory>

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "audio/sound.h"
#include "audio/sound_handle.h"
#include "st_macros.h"

/** @addtogroup audio
 *  @{
 */

namespace st::audio
{
    /**
     * @brief Plays one sound at a time from a position in the world.
     *
     * A source keeps the sound it was given alive. Every setter returns false, and does nothing, if the audio device
     * could not provide a source, which happens when no device is open or too many sources exist.
     */
    class source
    {
    public:
        source() noexcept;
        ~source() noexcept;

        ST_DELETE_COPY_CTORS(source);
        ST_DELETE_MOVE_CTORS(source);

        /**
         * @brief Sets the sound that play() starts. Stops the current sound.
         * @param sound Sound to play, or null to remove it.
         */
        st::bool_t set_sound(const std::shared_ptr<st::audio::sound> &sound) noexcept;
        /** @brief Sets the sound of a handle. @copydetails set_sound(const std::shared_ptr<st::audio::sound> &) */
        st::bool_t set_sound(const st::audio::sound_handle &sound) noexcept;

        /** @brief Plays the sound from the start, even when it is already playing. */
        st::bool_t play() noexcept;
        /** @brief Pauses the sound where it is. */
        st::bool_t pause() noexcept;
        /** @brief Continues a paused sound, or starts a stopped one from the start. */
        st::bool_t resume() noexcept;
        /** @brief Stops the sound. The next play() or resume() starts from the beginning. */
        st::bool_t stop() noexcept;

        /** @brief Whether the sound is playing. */
        [[nodiscard]] st::bool_t is_playing() const noexcept;
        /** @brief Whether the sound is paused. */
        [[nodiscard]] st::bool_t is_paused() const noexcept;
        /** @brief Whether the sound was stopped or reached its end. A source that never played is not stopped. */
        [[nodiscard]] st::bool_t is_stopped() const noexcept;

        /** @brief Moves the source. @param position Position in world units. */
        st::bool_t set_position(glm::f64vec3 position) noexcept;
        /** @brief Moves the source. @param x X coordinate. @param y Y coordinate. @param z Z coordinate. */
        st::bool_t set_position(st::float64 x,
                                st::float64 y,
                                st::float64 z) noexcept;
        /** @brief Sets the velocity used for the Doppler effect. @param velocity Velocity in world units per second. */
        st::bool_t set_velocity(glm::f64vec3 velocity) noexcept;
        /** @brief Sets the velocity. @param x X component. @param y Y component. @param z Z component. */
        st::bool_t set_velocity(st::float64 x,
                                st::float64 y,
                                st::float64 z) noexcept;
        /** @brief Sets the pitch multiplier. @param pitch 1 plays at the original pitch, 2 an octave higher. */
        st::bool_t set_pitch(st::float32 pitch) noexcept;
        /** @brief Sets the volume. @param gain 1 plays at the original volume, 0 is silent. */
        st::bool_t set_gain(st::float32 gain) noexcept;
        /** @brief Sets whether the sound restarts when it reaches its end. @param loop Whether to loop. */
        st::bool_t set_loop(st::bool_t loop) noexcept;
        /**
         * @brief Sets whether the sound is positioned in the world or plays at the same volume everywhere.
         *
         * Applies to mono and stereo sounds alike. A disabled source plays as if it were at the listener.
         *
         * @param spatial Whether the position of the source and the listener affect the sound. Enabled by default.
         */
        st::bool_t set_spatial(st::bool_t spatial) noexcept;

    private:
        std::shared_ptr<st::audio::sound> m_sound;
        st::uint32                        m_source = 0;
        st::bool_t                        m_valid = false;
    };
} // namespace st::audio

/** @} */

#endif // STICKY_AUDIO_SOURCE_H
