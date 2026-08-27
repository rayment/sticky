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

    [[nodiscard]] glm::f32vec2 delta_pos    () noexcept;
    [[nodiscard]] glm::f32vec2 global_pos   () noexcept;
    [[nodiscard]] glm::f32vec2 window_pos   () noexcept;

    [[nodiscard]] st_bool left_button_down  () noexcept;
    [[nodiscard]] st_bool right_button_down () noexcept;
    [[nodiscard]] st_bool middle_button_down() noexcept;
    [[nodiscard]] st_bool x1_button_down    () noexcept;
    [[nodiscard]] st_bool x2_button_down    () noexcept;

    [[nodiscard]] st_bool left_button_up    () noexcept;
    [[nodiscard]] st_bool right_button_up   () noexcept;
    [[nodiscard]] st_bool middle_button_up  () noexcept;
    [[nodiscard]] st_bool x1_button_up      () noexcept;
    [[nodiscard]] st_bool x2_button_up      () noexcept;

}

#endif // STICKY_IO_MOUSE_H
