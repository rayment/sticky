/*
 * client.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

/**
 * @file
 * @brief The listener that hears every audio source.
 */

#ifndef STICKY_AUDIO_CLIENT_H
#define STICKY_AUDIO_CLIENT_H

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "st_macros.h"

/** @addtogroup audio
 *  @{
 */

namespace st::audio
{
    /**
     * @brief The position and velocity of the listener, usually the camera or the player.
     *
     * The audio device has a single listener. Changes take effect when select() is called.
     */
    class client
    {
    public:
        /** @brief Creates a listener at the origin, at rest. */
        client() noexcept;
        ~client() noexcept;

        ST_DELETE_COPY_CTORS(client);
        ST_DELETE_MOVE_CTORS(client);

        /** @brief Makes this the listener of the audio device, applying its position and velocity. */
        void select() noexcept;

        /** @brief Moves the listener. Call select() to apply it. @param position Position in world units. */
        void set_position(const glm::f64vec3 &position) noexcept;
        /**
         * @brief Sets the velocity used for the Doppler effect. Call select() to apply it.
         * @param velocity Velocity in world units per second.
         */
        void set_velocity(const glm::f64vec3 &velocity) noexcept;

        /** @brief The position of the listener. */
        [[nodiscard]] glm::f64vec3 position() const noexcept;
        /** @brief The velocity of the listener. */
        [[nodiscard]] glm::f64vec3 velocity() const noexcept;

    private:
        glm::f64vec3 m_position;
        glm::f64vec3 m_velocity;
    };
} // namespace st::audio

/** @} */

#endif // STICKY_AUDIO_CLIENT_H
