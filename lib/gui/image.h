/*
 * image.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#ifndef STICKY_GUI_IMAGE_H
#define STICKY_GUI_IMAGE_H

#include <filesystem>
#include <memory>

#include "gui/component.h"
#include "video/texture.h"
#include "video/texture_handle.h"

namespace st::gui
{
    class image : public component
    {
    public:
        image() noexcept;
        ~image() noexcept;

        void render() noexcept override;

        void set_texture(const std::shared_ptr<st::video::texture> &texture) noexcept;
        void set_texture(const st::video::texture_handle &texture) noexcept;

        st::bool_t load_file(const std::filesystem::path &path) noexcept;

        // A dimension of zero uses the texture's natural size for that axis.
        void set_size(st::float32 width,
                      st::float32 height) noexcept;

    private:
        struct image_impl;
        std::unique_ptr<image_impl> m_impl;
    };
} // namespace st::gui

#endif // STICKY_GUI_IMAGE_H
