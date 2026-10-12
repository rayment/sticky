/*
 * rect.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <utility>
#include <vector>

#include <imgui.h>

#include "st_types.h"

#include "gui/rect.h"

struct st::gui::rect::rect_impl
{
    std::shared_ptr<st::video::texture> m_texture;
    ImVec4                              m_colour{1.0f, 1.0f, 1.0f, 1.0f};
    ImVec4                              m_background{0.0f, 0.0f, 0.0f, 0.0f};
    st::float32                         m_width = 0.0f;
    st::float32                         m_height = 0.0f;
    std::vector<on_click_callback>      m_callback_on_click;
};

st::gui::rect::rect() noexcept : m_impl{std::make_unique<rect_impl>()} {}

st::gui::rect::~rect() noexcept {}

void
st::gui::rect::set_texture(const std::shared_ptr<st::video::texture> &texture) noexcept
{
    m_impl->m_texture = texture;
}

void
st::gui::rect::set_texture(const st::video::texture_handle &texture) noexcept
{
    m_impl->m_texture = texture.shared();
}

st::bool_t
st::gui::rect::load_file(const std::filesystem::path &path) noexcept
{
    auto texture = st::video::texture::load_file(path);
    if (!texture)
        return false;
    m_impl->m_texture = std::move(texture);
    return true;
}

void
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
st::gui::rect::set_colour(st::float32 r,
                          st::float32 g,
                          st::float32 b,
                          st::float32 a) noexcept
{
    m_impl->m_colour = ImVec4{r, g, b, a};
}

void
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
st::gui::rect::set_background(st::float32 r,
                              st::float32 g,
                              st::float32 b,
                              st::float32 a) noexcept
{
    m_impl->m_background = ImVec4{r, g, b, a};
}

void
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
st::gui::rect::set_size(st::float32 width,
                        st::float32 height) noexcept
{
    m_impl->m_width = width;
    m_impl->m_height = height;
}

void
st::gui::rect::register_on_click(const on_click_callback &callback) noexcept
{
    if (callback)
        m_impl->m_callback_on_click.push_back(callback);
}

void
st::gui::rect::clear_on_click() noexcept
{
    m_impl->m_callback_on_click.clear();
}

void
st::gui::rect::handle_close() noexcept
{
    clear_on_click();
}

void
st::gui::rect::render() noexcept
{
    const auto &texture = m_impl->m_texture;
    if (texture)
    {
        const ImVec2 size{m_impl->m_width > 0.0f ? m_impl->m_width : static_cast<st::float32>(texture->width()),
                          m_impl->m_height > 0.0f ? m_impl->m_height : static_cast<st::float32>(texture->height())};
        ImGui::ImageWithBg(ImTextureRef(static_cast<ImTextureID>(texture->imgui_id())), size, {0.0f, 0.0f},
                           {1.0f, 1.0f}, m_impl->m_background, m_impl->m_colour);
    }
    else
    {
        if (m_impl->m_width <= 0.0f || m_impl->m_height <= 0.0f)
            return;
        ImGui::Dummy({m_impl->m_width, m_impl->m_height});
        ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(),
                                                  ImGui::ColorConvertFloat4ToU32(m_impl->m_colour));
    }

    for (const auto b :
         {st::io::mouse::Left, st::io::mouse::Right, st::io::mouse::Middle, st::io::mouse::X1, st::io::mouse::X2})
    {
        if (!ImGui::IsItemClicked(static_cast<ImGuiMouseButton>(b)))
            continue;

        // A callback may close the rect, which clears the callbacks being run.
        const auto callbacks = m_impl->m_callback_on_click;
        for (const auto &callback : callbacks)
            callback(b);
    }
}
