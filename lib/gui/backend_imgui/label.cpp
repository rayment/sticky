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
};

st::gui::label::label() noexcept
    : m_impl{std::make_unique<label_impl>()}
{}

st::gui::label::~label() noexcept
{}

void
st::gui::label::set_text(const std::string &text) noexcept
{
    m_impl->m_text = text;
}

void
st::gui::label::render() noexcept
{
    ImGui::Text("%s", m_impl->m_text.c_str());
}
