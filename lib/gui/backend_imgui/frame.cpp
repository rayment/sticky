/*
 * frame.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#include <cfloat>
#include <utility>

#include <fmt/format.h>
#include <glm/common.hpp>
#include <imgui.h>

#include "gui/frame.h"

#include "imgui_internal.h"

#define SET_FLAG(value, flag)                                                                                          \
    m_impl->m_flags =                                                                                                  \
        (value) ? (m_impl->m_flags | ImGuiWindowFlags_##flag) : (m_impl->m_flags & ~ImGuiWindowFlags_##flag)

namespace
{
    constexpr st::float32 fade_seconds = 0.1f;
}

struct st::gui::frame::frame_impl
{
    frame_impl(st::uint64 id) noexcept :
        m_title({}),
        m_frame_title(fmt::format("##{}",
                                  id)),
        m_flags(ImGuiWindowFlags_None),
        m_position_set(false),
        m_size_set(false),
        m_min_size(0.0f,
                   0.0f),
        m_closable(true),
        m_window_open(true),
        m_fading_out(false),
        m_alpha(0.0f)
    {
    }
    ~frame_impl() noexcept = default;

    std::string                    m_title;
    std::string                    m_frame_title;
    ImGuiWindowFlags               m_flags;
    st::bool_t                     m_position_set, m_size_set;
    glm::f32vec2                   m_position, m_size, m_min_size;
    st::bool_t                     m_closable, m_window_open, m_fading_out;
    st::float32                    m_alpha;
    std::vector<on_close_callback> m_callback_on_close;
};

st::gui::frame::frame() noexcept : m_impl{std::make_unique<frame_impl>(id())} {}

st::gui::frame::~frame() noexcept {}

void
st::gui::frame::set_auto_resize(st::bool_t auto_resize) noexcept
{
    SET_FLAG(auto_resize, AlwaysAutoResize);
}

void
st::gui::frame::set_closable(st::bool_t closable) noexcept
{
    m_impl->m_closable = closable;
}

void
st::gui::frame::set_collapsible(st::bool_t collapsible) noexcept
{
    SET_FLAG(!collapsible, NoCollapse);
}

void
st::gui::frame::set_min_size(st::float32 w,
                             st::float32 h) noexcept
{
    set_min_size({w, h});
}

void
st::gui::frame::set_min_size(glm::f32vec2 size) noexcept
{
    m_impl->m_min_size = glm::max(size, glm::f32vec2{0.0f, 0.0f});
}

void
st::gui::frame::set_movable(st::bool_t movable) noexcept
{
    SET_FLAG(!movable, NoMove);
}

void
st::gui::frame::set_position(st::float32 x,
                             st::float32 y) noexcept
{
    set_position({x, y});
}

void
st::gui::frame::set_position(glm::f32vec2 position) noexcept
{
    m_impl->m_position = position;
    m_impl->m_position_set = true;
}

void
st::gui::frame::set_resizable(st::bool_t resizable) noexcept
{
    SET_FLAG(!resizable, NoResize);
}

void
st::gui::frame::set_show_titlebar(st::bool_t show_titlebar) noexcept
{
    SET_FLAG(!show_titlebar, NoTitleBar);
}

void
st::gui::frame::set_size(st::float32 w,
                         st::float32 h) noexcept
{
    set_size({w, h});
}

void
st::gui::frame::set_size(glm::f32vec2 size) noexcept
{
    m_impl->m_size = size;
    m_impl->m_size_set = true;
}

void
st::gui::frame::set_title(std::string_view title) noexcept
{
    m_impl->m_title = title;
    m_impl->m_frame_title = fmt::format("{}##{}", m_impl->m_title, id());
}

void
st::gui::frame::handle_close() noexcept
{
    m_impl->m_window_open = false;

    // Callbacks may close this frame again or release the last reference to it, so they are taken out first.
    const auto callbacks = std::move(m_impl->m_callback_on_close);
    m_impl->m_callback_on_close.clear();
    for (const auto &callback : callbacks)
        callback();

    nestable_component::handle_close();
}

void
st::gui::frame::register_on_close(const on_close_callback &callback) noexcept
{
    if (callback)
        m_impl->m_callback_on_close.push_back(callback);
}

void
st::gui::frame::clear_on_close() noexcept
{
    m_impl->m_callback_on_close.clear();
}

void
st::gui::frame::fade_out() noexcept
{
    m_impl->m_fading_out = true;
}

void
st::gui::frame::render() noexcept
{
    if (is_closed())
        return;

    ImGui::SetNextWindowSizeConstraints({m_impl->m_min_size.x, m_impl->m_min_size.y}, {FLT_MAX, FLT_MAX});
    if (m_impl->m_size_set)
    {
        ImGui::SetNextWindowSize({m_impl->m_size.x, m_impl->m_size.y}, ImGuiCond_Always);
        m_impl->m_size_set = false;
    }
    if (m_impl->m_position_set)
    {
        ImGui::SetNextWindowPos({m_impl->m_position.x, m_impl->m_position.y}, ImGuiCond_Always, {0, 0});
        m_impl->m_position_set = false;
    }
    ImGuiWindowFlags flags = m_impl->m_flags;
    for (const component *child : children())
    {
        if (child->is_menubar())
            flags |= ImGuiWindowFlags_MenuBar;
    }
    if (m_impl->m_fading_out)
        flags |= ImGuiWindowFlags_NoInputs;

    const st::float32 step = ImGui::GetIO().DeltaTime / fade_seconds;
    m_impl->m_alpha = glm::clamp(m_impl->m_alpha + (m_impl->m_fading_out ? -step : step), 0.0f, 1.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, m_impl->m_alpha);
    if (ImGui::Begin(m_impl->m_frame_title.c_str(), m_impl->m_closable ? &m_impl->m_window_open : nullptr, flags))
    {
        render_children();
    }
    ImGui::End();
    ImGui::PopStyleVar();

    if (!m_impl->m_window_open && !m_impl->m_fading_out)
    {
        m_impl->m_fading_out = true;
        m_impl->m_window_open = true;
    }
    if (m_impl->m_fading_out && m_impl->m_alpha <= 0.0f)
        close();
}
