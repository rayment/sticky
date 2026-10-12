/*
 * image.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <utility>

#include <imgui.h>

#include "st_types.h"

#include "gui/image.h"

struct st::gui::image::image_impl
{
    std::shared_ptr<st::video::texture> m_texture;
    st::float32                         m_width = 0.0f;
    st::float32                         m_height = 0.0f;
};

st::gui::image::image() noexcept : m_impl{std::make_unique<image_impl>()} {}

st::gui::image::~image() noexcept {}

void
st::gui::image::set_texture(const std::shared_ptr<st::video::texture> &texture) noexcept
{
    m_impl->m_texture = texture;
}

void
st::gui::image::set_texture(const st::video::texture_handle &texture) noexcept
{
    m_impl->m_texture = texture.shared();
}

st::bool_t
st::gui::image::load_file(const std::filesystem::path &path) noexcept
{
    auto texture = st::video::texture::load_file(path);
    if (!texture)
        return false;
    m_impl->m_texture = std::move(texture);
    return true;
}

void
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
st::gui::image::set_size(st::float32 width,
                         st::float32 height) noexcept
{
    m_impl->m_width = width;
    m_impl->m_height = height;
}

void
st::gui::image::render() noexcept
{
    const auto &texture = m_impl->m_texture;
    if (!texture)
        return;

    const ImVec2 size{m_impl->m_width > 0.0f ? m_impl->m_width : static_cast<st::float32>(texture->width()),
                      m_impl->m_height > 0.0f ? m_impl->m_height : static_cast<st::float32>(texture->height())};
    ImGui::Image(ImTextureRef(static_cast<ImTextureID>(texture->imgui_id())), size);
}
