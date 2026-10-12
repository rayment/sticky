/*
 * popup.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include "gui/popup.h"
#include "gui/button.h"
#include "gui/frame.h"
#include "gui/label.h"

struct st::gui::popup::popup_impl
{
    popup_impl() noexcept
    {
        m_frame.set_auto_resize(true);
        m_frame.set_resizable(false);
        m_label.set_parent(&m_frame);
        m_button.set_parent(&m_frame);
        m_button.set_text("OK");
        m_button.register_on_click([this] { m_frame.close(); });
    }
    ~popup_impl() noexcept = default;

    st::gui::frame  m_frame;
    st::gui::label  m_label;
    st::gui::button m_button;
};

st::gui::popup::popup() noexcept : m_impl{std::make_unique<popup_impl>()} {}

st::gui::popup::~popup() noexcept {}

void
st::gui::popup::close() noexcept
{
    m_impl->m_frame.close();
}

void
st::gui::popup::render() noexcept
{
    m_impl->m_frame.render();
}

void
st::gui::popup::set_title(const std::string &title) noexcept
{
    m_impl->m_frame.set_title(title);
}

void
st::gui::popup::set_text(const std::string &text) noexcept
{
    m_impl->m_label.set_text(text);
}

void
st::gui::popup::set_button_text(const std::string &button_text) noexcept
{
    m_impl->m_button.set_text(button_text);
}

void
st::gui::popup::register_on_close(const on_close_callback &callback) noexcept
{
    m_impl->m_frame.register_on_close(callback);
}

void
st::gui::popup::clear_on_close() noexcept
{
    m_impl->m_frame.clear_on_close();
}
