/*
 * menubar.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#include <vector>

#include <fmt/format.h>
#include <imgui.h>

#include "st_types.h"

#include "gui/menubar.h"

struct st::gui::menu::menu_impl
{
    menu_impl(st::uint64 id) noexcept :
        m_enabled(true),
        m_text(fmt::format("##{}",
                           id))
    {
    }
    ~menu_impl() noexcept = default;

    st::bool_t  m_enabled;
    std::string m_text;
};

struct st::gui::menu_button::menu_button_impl
{
    menu_button_impl(st::uint64 id) noexcept :
        m_enabled(true),
        m_text(fmt::format("##{}",
                           id))
    {
    }
    ~menu_button_impl() noexcept = default;

    st::bool_t                     m_enabled;
    std::string                    m_text;
    std::vector<on_click_callback> m_callback_on_click;
};

struct st::gui::menu_checkbox::menu_checkbox_impl
{
    menu_checkbox_impl(st::uint64 id) noexcept :
        m_enabled(true),
        m_checked(false),
        m_text(fmt::format("##{}",
                           id))
    {
    }
    ~menu_checkbox_impl() noexcept = default;

    st::bool_t                      m_enabled;
    st::bool_t                      m_checked;
    std::string                     m_text;
    std::vector<on_toggle_callback> m_callback_on_toggle;
};

st::gui::menubar::menubar() noexcept : nestable_component(nullptr) {}

st::gui::menubar::~menubar() noexcept {}

st::bool_t
st::gui::menubar::is_menubar() const noexcept
{
    return true;
}

void
st::gui::menubar::render() noexcept
{
    if (is_closed())
        return;

    if (!ImGui::BeginMenuBar())
        return;
    render_children();
    ImGui::EndMenuBar();
}

st::gui::menu::menu() noexcept :
    nestable_component(nullptr),
    m_impl{std::make_unique<menu_impl>(id())}
{
}

st::gui::menu::~menu() noexcept {}

void
st::gui::menu::set_enabled(st::bool_t enabled) noexcept
{
    m_impl->m_enabled = enabled;
}

void
st::gui::menu::set_text(const std::string &text) noexcept
{
    m_impl->m_text = fmt::format("{}##{}", text, id());
}

void
st::gui::menu::render() noexcept
{
    if (is_closed())
        return;

    if (!ImGui::BeginMenu(m_impl->m_text.c_str(), m_impl->m_enabled))
        return;
    render_children();
    ImGui::EndMenu();
}

st::gui::menu_button::menu_button() noexcept : m_impl{std::make_unique<menu_button_impl>(id())} {}

st::gui::menu_button::~menu_button() noexcept {}

void
st::gui::menu_button::set_enabled(st::bool_t enabled) noexcept
{
    m_impl->m_enabled = enabled;
}

void
st::gui::menu_button::set_text(const std::string &text) noexcept
{
    m_impl->m_text = fmt::format("{}##{}", text, id());
}

void
st::gui::menu_button::register_on_click(const on_click_callback &callback) noexcept
{
    if (callback)
        m_impl->m_callback_on_click.push_back(callback);
}

void
st::gui::menu_button::clear_on_click() noexcept
{
    m_impl->m_callback_on_click.clear();
}

void
st::gui::menu_button::handle_close() noexcept
{
    clear_on_click();
}

void
st::gui::menu_button::render() noexcept
{
    if (is_closed())
        return;
    if (!ImGui::MenuItem(m_impl->m_text.c_str(), nullptr, false, m_impl->m_enabled))
        return;

    // A callback may close the button, which clears the callbacks being run.
    const auto callbacks = m_impl->m_callback_on_click;
    for (const auto &callback : callbacks)
        callback();
}

st::gui::menu_checkbox::menu_checkbox() noexcept : m_impl{std::make_unique<menu_checkbox_impl>(id())} {}

st::gui::menu_checkbox::~menu_checkbox() noexcept {}

void
st::gui::menu_checkbox::set_checked(st::bool_t checked) noexcept
{
    m_impl->m_checked = checked;
}

void
st::gui::menu_checkbox::set_enabled(st::bool_t enabled) noexcept
{
    m_impl->m_enabled = enabled;
}

void
st::gui::menu_checkbox::set_text(const std::string &text) noexcept
{
    m_impl->m_text = fmt::format("{}##{}", text, id());
}

st::bool_t
st::gui::menu_checkbox::is_checked() const noexcept
{
    return m_impl->m_checked;
}

void
st::gui::menu_checkbox::register_on_toggle(const on_toggle_callback &callback) noexcept
{
    if (callback)
        m_impl->m_callback_on_toggle.push_back(callback);
}

void
st::gui::menu_checkbox::clear_on_toggle() noexcept
{
    m_impl->m_callback_on_toggle.clear();
}

void
st::gui::menu_checkbox::handle_close() noexcept
{
    clear_on_toggle();
}

void
st::gui::menu_checkbox::render() noexcept
{
    if (is_closed())
        return;
    if (!ImGui::MenuItem(m_impl->m_text.c_str(), nullptr, &m_impl->m_checked, m_impl->m_enabled))
        return;

    // A callback may close the checkbox, which clears the callbacks being run.
    const auto       callbacks = m_impl->m_callback_on_toggle;
    const st::bool_t checked = m_impl->m_checked;
    for (const auto &callback : callbacks)
        callback(checked);
}

st::gui::menu_separator::menu_separator() noexcept = default;

st::gui::menu_separator::~menu_separator() noexcept {}

void
st::gui::menu_separator::render() noexcept
{
    if (is_closed())
        return;
    ImGui::Separator();
}
