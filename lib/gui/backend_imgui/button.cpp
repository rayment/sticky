/*
 * button.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <vector>

#include <fmt/format.h>
#include <imgui.h>

#include "st_types.h"

#include "gui/button.h"

struct st::gui::button::button_impl
{
    button_impl(st::uint64 id) noexcept
        : m_enabled(true),
          m_text(fmt::format("##{}", id))
    {}
    ~button_impl() noexcept = default;

    st::bool_t m_enabled;
    std::string m_text;
    std::vector<on_click_callback> m_callback_on_click;
};

st::gui::button::button() noexcept
    : m_impl{std::make_unique<button_impl>(id())}
{
}

st::gui::button::~button() noexcept
{}

void
st::gui::button::set_enabled(st::bool_t enabled) noexcept
{
    m_impl->m_enabled = enabled;
}

void
st::gui::button::set_text(const std::string &text) noexcept
{
    m_impl->m_text = fmt::format("{}##{}", text, id());
}

void
st::gui::button::register_on_click(const on_click_callback &callback) noexcept
{
    if (callback)
        m_impl->m_callback_on_click.push_back(callback);
}

void
st::gui::button::clear_on_click() noexcept
{
    m_impl->m_callback_on_click.clear();
}

void
st::gui::button::render() noexcept
{
    ImGui::BeginDisabled(!m_impl->m_enabled);
    if (ImGui::Button(m_impl->m_text.c_str()))
    {
        for (const auto &callback : m_impl->m_callback_on_click)
            callback();
    }
    ImGui::EndDisabled();
}
