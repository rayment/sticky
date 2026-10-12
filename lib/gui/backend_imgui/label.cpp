/*
 * label.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <fmt/format.h>
#include <imgui.h>

#include "st_types.h"

#include "gui/label.h"

struct st::gui::label::label_impl
{
    std::string m_text;
    st::float32 m_width = 0.0f;
    st::float32 m_height = 0.0f;
};

st::gui::label::label() noexcept : m_impl{std::make_unique<label_impl>()} {}

st::gui::label::~label() noexcept {}

void
st::gui::label::set_text(const std::string &text) noexcept
{
    m_impl->m_text = text;
}

void
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
st::gui::label::set_size(st::float32 width,
                         st::float32 height) noexcept
{
    m_impl->m_width = width;
    m_impl->m_height = height;
}

void
st::gui::label::render() noexcept
{
    if (m_impl->m_width <= 0.0f || m_impl->m_height <= 0.0f)
    {
        ImGui::Text("%s", m_impl->m_text.c_str());
        return;
    }

    const ImVec2 text_size = ImGui::CalcTextSize(m_impl->m_text.c_str());
    const ImVec2 origin = ImGui::GetCursorScreenPos();
    ImGui::Dummy({m_impl->m_width, m_impl->m_height});
    const ImVec2 position{origin.x + ((m_impl->m_width - text_size.x) * 0.5f),
                          origin.y + ((m_impl->m_height - text_size.y) * 0.5f)};
    ImGui::GetWindowDrawList()->AddText(position, ImGui::GetColorU32(ImGuiCol_Text), m_impl->m_text.c_str());
}
