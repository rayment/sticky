/*
 * window.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#ifndef STICKY_WINDOW_H
#define STICKY_WINDOW_H

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

namespace st::video
{
    class window
    {
    public:
        enum fullscreen_mode : st::uint8 { Windowed, Fullscreen };
        enum vsync_mode : st::uint8 { Adaptive, Disabled };

        window () noexcept;
        ~window() noexcept;

        ST_DELETE_COPY_CTORS(window);
        ST_DELETE_MOVE_CTORS(window);

        void show             ()                                          noexcept;
        void hide             ()                                          noexcept;
        void close            ()                                          noexcept;
        void poll             ()                                          noexcept;
        void clear            ()                                          noexcept;
        void swap             ()                                          noexcept;
        void accumulate       ()                                          noexcept;
        void reset_accumulator()                                          noexcept;
        void simulate         (const std::function<void(st::float64)> &callback) noexcept;
        void attach_gui       (st::gui::core &gui)                        noexcept; // TODO: Remove.

        [[nodiscard]] st::bool_t      close_requested() const noexcept;
        [[nodiscard]] st::float64     delta_time     () const noexcept;
        [[nodiscard]] st::float64     fixed_timestep () const noexcept;
        [[nodiscard]] fullscreen_mode fullscreen     () const noexcept;
        [[nodiscard]] glm::f32vec2    size           () const noexcept;
        [[nodiscard]] std::string     title          () const noexcept;
        [[nodiscard]] st::bool_t      visible        () const noexcept;
        [[nodiscard]] vsync_mode      vsync          () const noexcept;

        void set_clear_color   (glm::f32vec3 color)     noexcept;
        void set_fixed_timestep(st::float64 timestep)   noexcept;
        void set_fullscreen    (fullscreen_mode mode)   noexcept;
        void set_title         (std::string_view title) noexcept;
        void set_vsync         (vsync_mode mode)        noexcept;

        [[nodiscard]] void *sdl_ptr() const noexcept;
        [[nodiscard]] void *gl_ptr () const noexcept;

    private:
        struct window_impl;
        std::unique_ptr<window_impl> m_impl;
    };
}

#endif // STICKY_WINDOW_H
