/*
 * mouse.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

/**
 * @file
 * @brief Polled mouse state.
 */

#ifndef STICKY_IO_MOUSE_H
#define STICKY_IO_MOUSE_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <glm/fwd.hpp>

#include "st_types.h"

/** @addtogroup io
 *  @{
 */

/** @brief The state of the mouse, updated by st::video::window::poll(). */
namespace st::io::mouse
{
    /** @brief A mouse button. Lua sees these as `st.io.mouse.Left` and so on. */
    enum button : st::uint8
    {
        Left, ///< The primary button.
        Right, ///< The secondary button.
        Middle, ///< The wheel button.
        X1, ///< The first side button, usually "back".
        X2, ///< The second side button, usually "forward".
    };

    /** @brief How far the mouse moved since the previous call, in screen coordinates. */
    [[nodiscard]] glm::f32vec2 delta_pos() noexcept;
    /** @brief The position of the mouse on the desktop, in screen coordinates. */
    [[nodiscard]] glm::f32vec2 global_pos() noexcept;
    /** @brief The position of the mouse relative to the window that has focus, in screen coordinates. */
    [[nodiscard]] glm::f32vec2 window_pos() noexcept;

    /** @brief Whether the button is held down. @param b The button. */
    [[nodiscard]] st::bool_t button_down(button b) noexcept;
    /** @brief Whether the button is not held down. @param b The button. */
    [[nodiscard]] st::bool_t button_up(button b) noexcept;

} // namespace st::io::mouse

/** @} */

#endif // STICKY_IO_MOUSE_H
