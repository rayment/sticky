/*
 * client.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#ifndef STICKY_AUDIO_CLIENT_H
#define STICKY_AUDIO_CLIENT_H

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "st_macros.h"

namespace st::audio
{
    class client
    {
    public:
         client() noexcept;
        ~client() noexcept;

        ST_DELETE_COPY_CTORS(client);
        ST_DELETE_MOVE_CTORS(client);

        void select() noexcept;

        void set_position(const glm::f64vec3 &position) noexcept;
        void set_velocity(const glm::f64vec3 &velocity) noexcept;

        [[nodiscard]] glm::f64vec3 position() const noexcept;
        [[nodiscard]] glm::f64vec3 velocity() const noexcept;

    private:
        glm::f64vec3 m_position;
        glm::f64vec3 m_velocity;
    };
}

#endif // STICKY_AUDIO_CLIENT_H
