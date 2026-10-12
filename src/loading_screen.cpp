/*
 * loading_screen.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <string>

#include <glm/vec2.hpp>

#include "gui/core.h"
#include "gui/frame.h"
#include "gui/label.h"
#include "gui/progress.h"
#include "video/window.h"

#include "loading_screen.h"

namespace
{
    constexpr st::float32 width_fraction = 0.5f;
    constexpr st::float32 frame_height = 80.0f;
} // namespace

struct st::loading_screen::loading_screen_impl
{
    loading_screen_impl(st::video::window &window,
                        st::gui::core     &gui) noexcept :
        m_window(window),
        m_gui(gui)
    {
        m_frame.set_show_titlebar(false);
        m_frame.set_closable(false);
        m_frame.set_movable(false);
        m_frame.set_resizable(false);
        m_frame.set_collapsable(false);
        m_label.set_parent(&m_frame);
        m_progress.set_parent(&m_frame);
    }
    ~loading_screen_impl() noexcept = default;

    st::video::window &m_window;
    st::gui::core     &m_gui;
    st::gui::frame     m_frame;
    st::gui::label     m_label;
    st::gui::progress  m_progress;
};

st::loading_screen::loading_screen(st::video::window &window,
                                   st::gui::core     &gui) noexcept :
    m_impl{
        std::make_unique<loading_screen_impl>(window,
                                              gui),
    }
{
}

st::loading_screen::~loading_screen() noexcept {}

void
st::loading_screen::update(std::string_view status,
                           st::float32      fraction) noexcept
{
    const glm::f32vec2 window_size = m_impl->m_window.size();
    const glm::f32vec2 size{window_size.x * width_fraction, frame_height};

    m_impl->m_label.set_text(std::string(status));
    m_impl->m_progress.set_value(fraction);
    m_impl->m_frame.set_size(size);
    m_impl->m_frame.set_position((window_size - size) * 0.5f);

    m_impl->m_window.poll();
    m_impl->m_window.clear();
    m_impl->m_gui.clear();
    m_impl->m_frame.render();
    m_impl->m_gui.render();
    m_impl->m_gui.swap();
    m_impl->m_window.swap();
}
