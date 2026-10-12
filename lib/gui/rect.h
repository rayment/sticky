/*
 * rect.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#ifndef STICKY_GUI_RECT_H
#define STICKY_GUI_RECT_H

#include <filesystem>
#include <functional>
#include <memory>

#include "gui/component.h"
#include "io/mouse.h"
#include "video/texture.h"
#include "video/texture_handle.h"

namespace st::gui
{
    class rect : public component
    {
    public:
        rect() noexcept;
        ~rect() noexcept;

        void render() noexcept override;

        void set_texture(const std::shared_ptr<st::video::texture> &texture) noexcept;
        void set_texture(const st::video::texture_handle &texture) noexcept;

        st::bool_t load_file(const std::filesystem::path &path) noexcept;

        // Components are in the range 0 to 1. With a texture the colour tints it, without one it fills the rect.
        void set_colour(st::float32 r,
                        st::float32 g,
                        st::float32 b,
                        st::float32 a) noexcept;

        // Drawn behind a texture, so a transparent texture shows this colour. Transparent by default.
        void set_background(st::float32 r,
                            st::float32 g,
                            st::float32 b,
                            st::float32 a) noexcept;

        // A dimension of zero uses the texture's natural size for that axis, or nothing when there is no texture.
        void set_size(st::float32 width,
                      st::float32 height) noexcept;

        // Called with the mouse button that was used to click the rect.
        using on_click_callback = std::function<void(st::io::mouse::button)>;

        void register_on_click(const on_click_callback &callback) noexcept;

        void clear_on_click() noexcept;

    protected:
        // Drops the click callbacks so that closures referencing the rect are released.
        void handle_close() noexcept override;

    private:
        struct rect_impl;
        std::unique_ptr<rect_impl> m_impl;
    };
} // namespace st::gui

#endif // STICKY_GUI_RECT_H
