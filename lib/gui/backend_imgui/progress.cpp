/*
 * progress.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <algorithm>

#include <imgui.h>

#include "st_types.h"

#include "gui/progress.h"

struct st::gui::progress::progress_impl
{
    st::float32 m_value = 0.0f;
    std::string m_text;
};

st::gui::progress::progress() noexcept : m_impl{std::make_unique<progress_impl>()} {}

st::gui::progress::~progress() noexcept {}

st::float32
st::gui::progress::value() const noexcept
{
    return m_impl->m_value;
}

void
st::gui::progress::set_value(st::float32 value) noexcept
{
    m_impl->m_value = std::clamp(value, 0.0f, 1.0f);
}

void
st::gui::progress::set_text(const std::string &text) noexcept
{
    m_impl->m_text = text;
}

void
st::gui::progress::render() noexcept
{
    const char *overlay = m_impl->m_text.empty() ? nullptr : m_impl->m_text.c_str();
    ImGui::ProgressBar(m_impl->m_value, {-FLT_MIN, 0.0f}, overlay);
}
