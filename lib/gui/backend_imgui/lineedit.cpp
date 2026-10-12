/*
 * lineedit.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#include <cfloat>

#include <fmt/format.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include "st_types.h"

#include "gui/lineedit.h"

struct st::gui::lineedit::lineedit_impl
{
    explicit lineedit_impl(st::uint64 id) noexcept :
        m_label(fmt::format("##{}",
                            id))
    {
    }

    st::bool_t  m_enabled = true;
    st::bool_t  m_read_only = false;
    st::bool_t  m_masked = false;
    std::string m_label;
    std::string m_text;
    std::string m_hint;
};

st::gui::lineedit::lineedit() noexcept : m_impl{std::make_unique<lineedit_impl>(id())} {}

st::gui::lineedit::~lineedit() noexcept {}

const std::string &
st::gui::lineedit::text() const noexcept
{
    return m_impl->m_text;
}

st::bool_t
st::gui::lineedit::is_enabled() const noexcept
{
    return m_impl->m_enabled;
}

st::bool_t
st::gui::lineedit::is_read_only() const noexcept
{
    return m_impl->m_read_only;
}

st::bool_t
st::gui::lineedit::is_masked() const noexcept
{
    return m_impl->m_masked;
}

void
st::gui::lineedit::set_text(const std::string &text) noexcept
{
    m_impl->m_text = text;
}

void
st::gui::lineedit::set_hint(const std::string &hint) noexcept
{
    m_impl->m_hint = hint;
}

void
st::gui::lineedit::set_enabled(st::bool_t enabled) noexcept
{
    m_impl->m_enabled = enabled;
}

void
st::gui::lineedit::set_read_only(st::bool_t read_only) noexcept
{
    m_impl->m_read_only = read_only;
}

void
st::gui::lineedit::set_masked(st::bool_t masked) noexcept
{
    m_impl->m_masked = masked;
}

void
st::gui::lineedit::render() noexcept
{
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
    if (m_impl->m_read_only)
        flags |= ImGuiInputTextFlags_ReadOnly;
    if (m_impl->m_masked)
        flags |= ImGuiInputTextFlags_Password;

    ImGui::BeginDisabled(!m_impl->m_enabled);
    ImGui::SetNextItemWidth(-FLT_MIN);
    ImGui::InputTextWithHint(m_impl->m_label.c_str(), m_impl->m_hint.c_str(), &m_impl->m_text, flags);
    ImGui::EndDisabled();
}
