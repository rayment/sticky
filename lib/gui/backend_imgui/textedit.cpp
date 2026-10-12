/*
 * textedit.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#include <cfloat>

#include <fmt/format.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "st_types.h"

#include "gui/textedit.h"

struct st::gui::textedit::textedit_impl
{
    explicit textedit_impl(st::uint64 id) noexcept :
        m_label(fmt::format("##{}",
                            id))
    {
    }

    st::bool_t  m_enabled = true;
    st::bool_t  m_read_only = false;
    st::bool_t  m_masked = false;
    st::bool_t  m_vertical_scrollbar = true;
    st::bool_t  m_horizontal_scrollbar = false;
    st::float32 m_width = 0.0f;
    st::float32 m_height = 0.0f;
    std::string m_label;
    std::string m_text;
};

st::gui::textedit::textedit() noexcept : m_impl{std::make_unique<textedit_impl>(id())} {}

st::gui::textedit::~textedit() noexcept {}

const std::string &
st::gui::textedit::text() const noexcept
{
    return m_impl->m_text;
}

st::bool_t
st::gui::textedit::is_enabled() const noexcept
{
    return m_impl->m_enabled;
}

st::bool_t
st::gui::textedit::is_read_only() const noexcept
{
    return m_impl->m_read_only;
}

st::bool_t
st::gui::textedit::is_masked() const noexcept
{
    return m_impl->m_masked;
}

st::bool_t
st::gui::textedit::has_vertical_scrollbar() const noexcept
{
    return m_impl->m_vertical_scrollbar;
}

st::bool_t
st::gui::textedit::has_horizontal_scrollbar() const noexcept
{
    return m_impl->m_horizontal_scrollbar;
}

void
st::gui::textedit::set_text(const std::string &text) noexcept
{
    m_impl->m_text = text;
}

void
st::gui::textedit::set_enabled(st::bool_t enabled) noexcept
{
    m_impl->m_enabled = enabled;
}

void
st::gui::textedit::set_read_only(st::bool_t read_only) noexcept
{
    m_impl->m_read_only = read_only;
}

void
st::gui::textedit::set_masked(st::bool_t masked) noexcept
{
    m_impl->m_masked = masked;
}

void
st::gui::textedit::set_vertical_scrollbar(st::bool_t visible) noexcept
{
    m_impl->m_vertical_scrollbar = visible;
}

void
st::gui::textedit::set_horizontal_scrollbar(st::bool_t visible) noexcept
{
    m_impl->m_horizontal_scrollbar = visible;
}

void
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
st::gui::textedit::set_size(st::float32 width,
                            st::float32 height) noexcept
{
    m_impl->m_width = width;
    m_impl->m_height = height;
}

void
st::gui::textedit::render() noexcept
{
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
    if (m_impl->m_read_only)
        flags |= ImGuiInputTextFlags_ReadOnly;
    if (m_impl->m_masked)
        flags |= ImGuiInputTextFlags_Password;
    if (!m_impl->m_horizontal_scrollbar)
        flags |= ImGuiInputTextFlags_WordWrap;

    const ImVec2 size{m_impl->m_width > 0.0f ? m_impl->m_width : -FLT_MIN,
                      m_impl->m_height > 0.0f ? m_impl->m_height : 0.0f};

    ImGui::BeginDisabled(!m_impl->m_enabled);
    if (!m_impl->m_vertical_scrollbar)
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 0.0f);
    ImGui::InputTextMultiline(m_impl->m_label.c_str(), &m_impl->m_text, size, flags);
    if (!m_impl->m_vertical_scrollbar)
        ImGui::PopStyleVar();
    ImGui::EndDisabled();
}
