/*
 * window.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <cstdlib>

#include <fmt/format.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/vector_common.hpp>
#include <GL/glew.h>
#include <SDL3/SDL.h>

#include "gui/core.h"
#include "io/gamepad.h"
#include "io/log.h"
#include "video/window.h"

struct st::video::window::window_impl
{
    window_impl () noexcept = default;
    ~window_impl() noexcept = default;

    ST_DELETE_COPY_CTORS(window_impl);
    ST_DELETE_MOVE_CTORS(window_impl);

    SDL_Window      *window         = nullptr;
    SDL_GLContext    gl_context     = {};
    std::string      title          = "libsticky " VERSION_STRING;
    st::bool_t       visible        = false;
    st::bool_t       want_close     = false;
    vsync_mode       vsync          = Disabled;
    fullscreen_mode  fullscreen     = Windowed;
    glm::u16vec2     size           = { 800, 600 };
    glm::f32vec3     clear_color    = { 0.0f, 0.0f, 0.0f };
    st::float64      delta_time     = 0.0;
    st::float64      fixed_timestep = 0.02;
    st::uint64       perf_counter   = 0;
    st::float64      accumulator    = 0;
    st::gui::core   *gui            = nullptr; // TODO: Remove.
};

#define ST_SDL_SAFE_CALL(name, expr) \
    if (ST_UNLIKELY(!(expr))) \
    { \
        const st_char *err = SDL_GetError(); \
        st::os::popup::error((name), err); \
        abort(); \
    }

st::video::window::window() noexcept
    : m_impl{std::make_unique<window_impl>()}
{
    SDL_InitFlags flags = SDL_WasInit(SDL_INIT_VIDEO);
    ST_ASSERT(flags != 0);

    st_float32 scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    m_impl->size.x = static_cast<st_uint16>(1280 * scale);
    m_impl->size.y = static_cast<st_uint16>(720 * scale);
    m_impl->window = SDL_CreateWindow(m_impl->title.c_str(), m_impl->size.x, m_impl->size.y, window_flags);
    ST_SDL_SAFE_CALL("SDL_CreateWindow", m_impl->window != nullptr);
    m_impl->gl_context = SDL_GL_CreateContext(m_impl->window);
    ST_SDL_SAFE_CALL("SDL_GL_CreateContext", m_impl->gl_context != nullptr);
    ST_SDL_SAFE_CALL("SDL_GL_MakeCurrent", SDL_GL_MakeCurrent(m_impl->window, m_impl->gl_context));
    ST_SDL_SAFE_CALL("SDL_SetWindowPosition", SDL_SetWindowPosition(m_impl->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED));

    set_clear_color(m_impl->clear_color);
    set_fullscreen(m_impl->fullscreen);
    set_title(m_impl->title);
    set_vsync(m_impl->vsync);
}

st::video::window::~window() noexcept
{
    // TODO: Move gamepad free to event handler.
    st::io::gamepad::disconnect_all(); // free up gamepads allocated from SDL connect events
    ST_SDL_SAFE_CALL("SDL_GL_DestroyContext", SDL_GL_DestroyContext(m_impl->gl_context));
    SDL_DestroyWindow(m_impl->window);
}

void
st::video::window::show() noexcept
{
    m_impl->visible = true;
    ST_SDL_SAFE_CALL("SDL_ShowWindow", SDL_ShowWindow(m_impl->window));
}

void
st::video::window::hide() noexcept
{
    m_impl->visible = false;
    ST_SDL_SAFE_CALL("SDL_HideWindow", SDL_HideWindow(m_impl->window));
}

void
st::video::window::close() noexcept
{
    m_impl->want_close = true;
}

void
st::video::window::poll() noexcept
{
    // TODO: Move all of this into the event handler.
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        // st_gui_backend_process_event(e);
        if (e.type == SDL_EVENT_QUIT)
        {
            close();
        }
        else if (e.type == SDL_EVENT_GAMEPAD_ADDED)
        {
            st::io::gamepad::connect(e.gdevice.which);
        }
        else if (e.type == SDL_EVENT_GAMEPAD_REMOVED)
        {
            st::io::gamepad::disconnect(e.gdevice.which);
        }
        else
        {
            // TODO: Replace with event bus.
            if (m_impl->gui)
                m_impl->gui->process_event(e);
        }
    }
}

void
st::video::window::clear() noexcept
{
    glViewport(0, 0, m_impl->size.x, m_impl->size.y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
st::video::window::swap() noexcept
{
    ST_SDL_SAFE_CALL("SDL_GL_SwapWindow", SDL_GL_SwapWindow(m_impl->window));
}

void
st::video::window::accumulate() noexcept
{
    st::uint64 current_perf_counter = SDL_GetPerformanceCounter();
    st::uint64 freq = SDL_GetPerformanceFrequency();
    st::float64 frame_delta = static_cast<st::float64>(current_perf_counter - m_impl->perf_counter) / static_cast<st::float64>(freq);
    m_impl->perf_counter = current_perf_counter;
    m_impl->delta_time = glm::clamp<st::float64>(frame_delta, 0.0, m_impl->fixed_timestep);
    m_impl->accumulator += frame_delta;
}

void
st::video::window::reset_accumulator() noexcept
{
    m_impl->accumulator = 0.0;
    m_impl->perf_counter = SDL_GetPerformanceCounter();
    m_impl->delta_time = 0.0;
}

void
st::video::window::simulate(std::function<void(st::float64)> callback) noexcept
{
    while (m_impl->accumulator >= m_impl->fixed_timestep)
    {
        callback(m_impl->fixed_timestep);
        m_impl->accumulator -= m_impl->fixed_timestep;
    }
}

st_bool
st::video::window::close_requested() const noexcept
{
    return m_impl->want_close;
}

st::float64
st::video::window::delta_time() const noexcept
{
    return m_impl->delta_time;
}

st::float64
st::video::window::fixed_timestep() const noexcept
{
    return m_impl->fixed_timestep;
}

st::video::window::fullscreen_mode
st::video::window::fullscreen() const noexcept
{
    return m_impl->fullscreen;
}

std::string
st::video::window::title() const noexcept
{
    return m_impl->title;
}

st_bool
st::video::window::visible() const noexcept
{
    return m_impl->visible;
}

st::video::window::vsync_mode
st::video::window::vsync() const noexcept
{
    return m_impl->vsync;
}

void
st::video::window::set_clear_color(glm::f32vec3 color) noexcept
{
    m_impl->clear_color = color;
    glClearColor(m_impl->clear_color.x, m_impl->clear_color.y, m_impl->clear_color.z, 1.0f);
}

void
st::video::window::set_fixed_timestep(st::float64 timestep) noexcept
{
    m_impl->fixed_timestep = glm::clamp<st::float64>(timestep, 0.0, 1.0);
}

void
st::video::window::set_fullscreen(fullscreen_mode mode) noexcept
{
    m_impl->fullscreen = mode;
    switch (m_impl->fullscreen)
    {
    case Windowed:
        ST_SDL_SAFE_CALL("SDL_SetWindowFullscreen",
            SDL_SetWindowFullscreen(m_impl->window, false));
        break;
    case Fullscreen:
        ST_SDL_SAFE_CALL("SDL_SetWindowFullscreen",
            SDL_SetWindowFullscreen(m_impl->window, true));
        break;
    }
}

void
st::video::window::set_title(std::string_view title) noexcept
{
    m_impl->title = title;
    ST_SDL_SAFE_CALL("SDL_SetWindowTitle",
        SDL_SetWindowTitle(m_impl->window, m_impl->title.c_str()));
}

void
st::video::window::set_vsync(vsync_mode mode) noexcept
{
    m_impl->vsync = mode;
    switch (m_impl->vsync)
    {
    case Adaptive:
        ST_SDL_SAFE_CALL("SDL_GL_SetSwapInterval",
            SDL_GL_SetSwapInterval(SDL_RENDERER_VSYNC_ADAPTIVE));
        break;
    case Disabled:
        ST_SDL_SAFE_CALL("SDL_GL_SetSwapInterval",
            SDL_GL_SetSwapInterval(SDL_RENDERER_VSYNC_DISABLED));
        break;
    }
}

void *
st::video::window::sdl_ptr() const noexcept
{
    return m_impl->window;
}

void *
st::video::window::gl_ptr() const noexcept
{
    return m_impl->gl_context;
}

void
st::video::window::attach_gui(st::gui::core &gui) noexcept
{
    // TODO: Remove.
    m_impl->gui = &gui;
}
