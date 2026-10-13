/*
 * core.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

/**
 * @file
 * @brief The GUI backend bound to a window.
 */

#ifndef STICKY_GUI_CORE_H
#define STICKY_GUI_CORE_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <memory>

#include <SDL3/SDL.h>

#include "video/window.h"

/** @addtogroup gui
 *  @{
 */

namespace st::gui
{
    /**
     * @brief Draws the shown components and the log overlay into a window.
     *
     * Only one core may exist at a time. Each frame, call clear(), then render(), then swap(), before
     * st::video::window::swap().
     */
    class core
    {
    public:
        /** @brief Binds the GUI to a window, which must be valid and outlive the core. @param window The window. */
        explicit core(const st::video::window &window) noexcept;
        ~core() noexcept;

        ST_DELETE_COPY_CTORS(core);
        ST_DELETE_MOVE_CTORS(core);

        /** @brief Starts a new GUI frame. */
        void clear() noexcept;
        /** @brief Renders every shown component and the overlay of recent warnings and errors. */
        void render() noexcept;
        /** @brief Draws the rendered frame with OpenGL. */
        void swap() noexcept;

        /**
         * @brief Passes an input or window event to the GUI.
         * @param e The event.
         * @todo Replace `SDL_Event` with an engine event type.
         */
        void process_event(const SDL_Event &e) noexcept;

    private:
        struct core_impl;
        std::unique_ptr<core_impl> m_impl;
    };
} // namespace st::gui

/** @} */

#endif // STICKY_GUI_CORE_H
