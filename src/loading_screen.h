/*
 * loading_screen.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief The progress display shown while mods start.
 */

#ifndef STICKY_LOADING_SCREEN_H
#define STICKY_LOADING_SCREEN_H

#include <memory>
#include <string_view>

#include "st_macros.h"
#include "st_types.h"

namespace st::gui
{
    class core; // forward-declare
}

namespace st::video
{
    class window; // forward-declare
}

/** @addtogroup app
 *  @{
 */

namespace st
{

    /** @brief Draws a status line and a progress bar on the window while the engine boots. */
    class loading_screen
    {
    public:
        /**
         * @brief Prepares the display. Nothing is drawn until update().
         * @param window The window, which must outlive the screen.
         * @param gui The GUI of the window, which must outlive the screen.
         */
        loading_screen(st::video::window &window,
                       st::gui::core     &gui) noexcept;
        ~loading_screen() noexcept;

        ST_DELETE_COPY_CTORS(loading_screen);
        ST_DELETE_MOVE_CTORS(loading_screen);

        /**
         * @brief Renders and presents one frame.
         * @param status Line of text describing the current step.
         * @param fraction Fraction of the boot completed, clamped to the range 0 to 1.
         */
        void update(std::string_view status,
                    st::float32      fraction) noexcept;

    private:
        struct loading_screen_impl;
        std::unique_ptr<loading_screen_impl> m_impl;
    };

} // namespace st

/** @} */

#endif // STICKY_LOADING_SCREEN_H
