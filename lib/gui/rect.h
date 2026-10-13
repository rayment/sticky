/*
 * rect.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief A coloured or textured rectangle.
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

/** @addtogroup gui
 *  @{
 */

namespace st::gui
{
    /** @brief A rectangle filled with a colour or a texture, which reports clicks of any mouse button. */
    class rect : public component
    {
    public:
        rect() noexcept;
        ~rect() noexcept;

        void render() noexcept override;

        /** @brief Draws a texture. @param texture The texture, or null for a plain colour. */
        void set_texture(const std::shared_ptr<st::video::texture> &texture) noexcept;
        /** @brief Draws the texture of a handle. @param texture The handle. */
        void set_texture(const st::video::texture_handle &texture) noexcept;

        /**
         * @brief Loads a PNG and draws it.
         * @param path The file.
         * @return False, leaving the texture unchanged, if the file could not be loaded.
         */
        st::bool_t load_file(const std::filesystem::path &path) noexcept;

        /**
         * @brief Sets the colour, which tints the texture or fills the rect when there is none. White by default.
         * @param r Red. @param g Green. @param b Blue. @param a Opacity. Each from 0 to 1.
         */
        void set_colour(st::float32 r,
                        st::float32 g,
                        st::float32 b,
                        st::float32 a) noexcept;

        /**
         * @brief Sets the colour drawn behind a texture, which shows through its transparent parts. Transparent by
         *        default.
         * @param r Red. @param g Green. @param b Blue. @param a Opacity. Each from 0 to 1.
         */
        void set_background(st::float32 r,
                            st::float32 g,
                            st::float32 b,
                            st::float32 a) noexcept;

        /**
         * @brief Sets the size.
         *
         * A dimension of zero uses the natural size of the texture along that axis, or draws nothing without one.
         *
         * @param width Width in pixels.
         * @param height Height in pixels.
         */
        void set_size(st::float32 width,
                      st::float32 height) noexcept;

        /** @brief Called with the mouse button that clicked the rect. */
        using on_click_callback = std::function<void(st::io::mouse::button)>;

        /** @brief Adds a click callback. @param callback The callback. */
        void register_on_click(const on_click_callback &callback) noexcept;

        /** @brief Removes every click callback. */
        void clear_on_click() noexcept;

    protected:
        /** @brief Drops the click callbacks so that closures referencing the rect are released. */
        void handle_close() noexcept override;

    private:
        struct rect_impl;
        std::unique_ptr<rect_impl> m_impl;
    };
} // namespace st::gui

/** @} */

#endif // STICKY_GUI_RECT_H
