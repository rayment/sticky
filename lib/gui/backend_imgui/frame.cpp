/*
 * frame.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#include <fmt/format.h>
#include <imgui.h>

#include "gui/frame.h"

#include "imgui_internal.h"

#define SET_FLAG(value, flag) m_impl->m_flags = (value) ? (m_impl->m_flags | ImGuiWindowFlags_##flag) : (m_impl->m_flags & ~ImGuiWindowFlags_##flag)

struct st::gui::frame::frame_impl
{
    frame_impl(st::uint64 id) noexcept
        : m_title({}),
          m_frame_title(fmt::format("##{}", id)),
          m_flags(ImGuiWindowFlags_None),
          m_position_set(false),
          m_size_set(false),
          m_closable(true),
          m_visible(true),
          m_window_open(true)
    {}
    ~frame_impl() noexcept = default;

    std::string m_title;
    std::string m_frame_title;
    ImGuiWindowFlags m_flags;
    st::bool_t m_position_set, m_size_set;
    glm::f32vec2 m_position, m_size;
    st::bool_t m_closable, m_visible, m_window_open;
    std::vector<on_close_callback> m_callback_on_close;
};

st::gui::frame::frame() noexcept
    : m_impl{std::make_unique<frame_impl>(id())}
{}

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
st::gui::frame::set_collapsable(st::bool_t collapsable) noexcept
{
    SET_FLAG(!collapsable, NoCollapse);
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
st::gui::frame::set_size(st::float32 w, st::float32 h) noexcept
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
st::gui::frame::close() noexcept
{
    m_impl->m_window_open = false;
    m_impl->m_visible = false;
    for (auto &callback : m_impl->m_callback_on_close)
        callback();
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
st::gui::frame::render() noexcept
{
    if (!m_impl->m_visible)
        return;

    if (m_impl->m_size_set)
    {
        ImGui::SetNextWindowSize({m_impl->m_size.x, m_impl->m_size.y}, ImGuiCond_Always);
        m_impl->m_size_set = false;
    }
    if (m_impl->m_position_set)
    {
        ImGui::SetNextWindowPos(
            { m_impl->m_position.x, m_impl->m_position.y },
            ImGuiCond_Always,
            {0, 0}
        );
        m_impl->m_position_set = false;
    }
    if (ImGui::Begin(
        m_impl->m_frame_title.c_str(),
        m_impl->m_closable ? &m_impl->m_window_open : nullptr,
        m_impl->m_flags
    ))
    {
        for (auto &child : children())
            child->render();
    }
    ImGui::End();
    if (m_impl->m_visible != m_impl->m_window_open)
        close();
}
