/*
 * layout.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#include <imgui.h>

#include "st_types.h"

#include "gui/layout.h"

struct st::gui::layout::layout_impl
{
    st::float32 m_spacing = -1.0f;
};

st::gui::layout::layout() noexcept :
    nestable_component(nullptr),
    m_impl{std::make_unique<layout_impl>()}
{
}

st::gui::layout::~layout() noexcept {}

void
st::gui::layout::set_spacing(st::float32 spacing) noexcept
{
    m_impl->m_spacing = spacing;
}

void
st::gui::layout::render_group(st::bool_t horizontal) noexcept
{
    const st::bool_t spaced = m_impl->m_spacing >= 0.0f;
    if (spaced)
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {m_impl->m_spacing, m_impl->m_spacing});

    ImGui::BeginGroup();
    if (horizontal)
        render_children([] { ImGui::SameLine(); });
    else
        render_children();
    ImGui::EndGroup();

    if (spaced)
        ImGui::PopStyleVar();
}

st::gui::hlayout::hlayout() noexcept = default;

st::gui::hlayout::~hlayout() noexcept {}

void
st::gui::hlayout::render() noexcept
{
    render_group(true);
}

st::gui::vlayout::vlayout() noexcept = default;

st::gui::vlayout::~vlayout() noexcept {}

void
st::gui::vlayout::render() noexcept
{
    render_group(false);
}
