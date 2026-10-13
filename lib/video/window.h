/*
 * window.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

/**
 * @file
 * @brief The application window, its OpenGL context and the frame timer.
 */

#ifndef STICKY_VIDEO_WINDOW_H
#define STICKY_VIDEO_WINDOW_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include <glm/fwd.hpp>

#include "st_macros.h"
#include "st_types.h"

namespace st::gui
{
    class core; // forward-declare
}

/** @addtogroup video
 *  @{
 */

namespace st::video
{
    /**
     * @brief The window of the application, with an OpenGL 3.0 core context.
     *
     * Requires st::lib_init(). The window is created hidden, centred on the primary display, and its context is made
     * current. A window that could not be created reports the error to the user and is_valid() returns false; every
     * other method is then safe to call but does nothing useful.
     *
     * The window also drives a fixed-timestep frame timer: call accumulate() once per frame and simulate() to run the
     * fixed steps that are due.
     */
    class window
    {
    public:
        /** @brief Whether the window covers the screen. */
        enum fullscreen_mode : st::uint8
        {
            Windowed, ///< A normal, resizable window.
            Fullscreen, ///< Covers the display the window is on.
        };

        /** @brief How buffer swaps are synchronised to the display. */
        enum vsync_mode : st::uint8
        {
            Adaptive, ///< Synchronised, but late frames are shown at once. Falls back to Enabled when unsupported.
            Disabled, ///< Not synchronised.
            Enabled, ///< Synchronised to every refresh.
        };

        window() noexcept;
        ~window() noexcept;

        ST_DELETE_COPY_CTORS(window);
        ST_DELETE_MOVE_CTORS(window);

        /** @brief Whether the window and its OpenGL context were created. */
        [[nodiscard]] st::bool_t is_valid() const noexcept;

        /** @brief Shows the window. */
        void show() noexcept;
        /** @brief Hides the window. */
        void hide() noexcept;
        /** @brief Asks the application to close the window; see close_requested(). */
        void close() noexcept;
        /** @brief Handles all pending window and input events. Call once per frame. */
        void poll() noexcept;
        /** @brief Clears the whole window to the clear colour. */
        void clear() noexcept;
        /** @brief Presents the frame that was drawn. */
        void swap() noexcept;

        /**
         * @brief Measures the time since the last call and adds it to the frame timer.
         *
         * At most a quarter of a second is added per call, so that a stall does not trigger a burst of fixed steps.
         */
        void accumulate() noexcept;
        /** @brief Empties the frame timer and restarts the measurement. Call before the first frame. */
        void reset_accumulator() noexcept;
        /**
         * @brief Runs the fixed steps that the frame timer has accumulated.
         * @param callback Called once per due step with fixed_timestep() in seconds.
         */
        void simulate(const std::function<void(st::float64)> &callback) noexcept;

        /**
         * @brief Forwards the events that poll() does not handle itself to the GUI.
         *
         * A stopgap until the engine has an event bus.
         *
         * @param gui GUI that receives the events. It must outlive the window or the window must stop being polled.
         */
        void attach_gui(st::gui::core &gui) noexcept;

        /** @brief Whether close() was called or the user asked to close the window. */
        [[nodiscard]] st::bool_t close_requested() const noexcept;
        /** @brief Duration of the last frame in seconds, at most one fixed step. */
        [[nodiscard]] st::float64 delta_time() const noexcept;
        /** @brief Duration of a fixed step in seconds. */
        [[nodiscard]] st::float64 fixed_timestep() const noexcept;
        /** @brief The fullscreen mode. */
        [[nodiscard]] fullscreen_mode fullscreen() const noexcept;
        /** @brief Size of the window in screen coordinates, which differ from pixels on high density displays. */
        [[nodiscard]] glm::f32vec2 size() const noexcept;
        /** @brief Title of the window. */
        [[nodiscard]] std::string title() const noexcept;
        /** @brief Whether the window is shown. */
        [[nodiscard]] st::bool_t visible() const noexcept;
        /** @brief The vsync mode in effect, which is Enabled when Adaptive was requested but is unsupported. */
        [[nodiscard]] vsync_mode vsync() const noexcept;

        /** @brief Sets the colour used by clear(). @param colour Red, green and blue in the range 0 to 1. */
        void set_clear_colour(glm::f32vec3 colour) noexcept;
        /** @brief Sets the duration of a fixed step. @param timestep Seconds, clamped to the range 0.001 to 1. */
        void set_fixed_timestep(st::float64 timestep) noexcept;
        /** @brief Sets the fullscreen mode. Failure is logged and leaves the mode unchanged. @param mode New mode. */
        void set_fullscreen(fullscreen_mode mode) noexcept;
        /** @brief Sets the title of the window. @param title Title in UTF-8. */
        void set_title(std::string_view title) noexcept;
        /** @brief Sets the vsync mode. Failure is logged and leaves the mode unchanged. @param mode New mode. */
        void set_vsync(vsync_mode mode) noexcept;

        /** @brief The underlying `SDL_Window`, or null if the window is not valid. */
        [[nodiscard]] void *sdl_ptr() const noexcept;
        /** @brief The underlying `SDL_GLContext`, or null if the window is not valid. */
        [[nodiscard]] void *gl_ptr() const noexcept;

    private:
        struct window_impl;
        std::unique_ptr<window_impl> m_impl;
    };
} // namespace st::video

/** @} */

#endif // STICKY_VIDEO_WINDOW_H
