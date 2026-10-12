/*
 * mouse.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#ifndef STICKY_IO_MOUSE_H
#define STICKY_IO_MOUSE_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <glm/fwd.hpp>

#include "st_types.h"

namespace st::io::mouse
{
    enum button : st::uint8
    {
        Left, Right, Middle, X1, X2,
    };

    [[nodiscard]] glm::f32vec2 delta_pos    () noexcept;
    [[nodiscard]] glm::f32vec2 global_pos   () noexcept;
    [[nodiscard]] glm::f32vec2 window_pos   () noexcept;

    [[nodiscard]] st::bool_t button_down(button b) noexcept;
    [[nodiscard]] st::bool_t button_up  (button b) noexcept;

}

#endif // STICKY_IO_MOUSE_H
