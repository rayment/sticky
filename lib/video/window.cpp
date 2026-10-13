/*
 * window.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <fmt/format.h>
#include <glm/ext/vector_common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "gui/core.h"
#include "io/gamepad.h"
#include "io/log.h"
#include "os/popup.h"
#include "video/window.h"

namespace
{
    constexpr st::float64 min_timestep = 0.001;
    constexpr st::float64 max_timestep = 1.0;
    constexpr st::float64 max_frame_delta = 0.25;

    // Logs the SDL error of a failed call and returns whether it succeeded.
    st::bool_t sdl_check(st::bool_t  ok,
                         const char *call) noexcept
    {
        if (!ok)
            st::io::log::error(fmt::format("{} failed: {}", call, SDL_GetError()));
        return ok;
    }
} // namespace

struct st::video::window::window_impl
{
    window_impl() noexcept = default;
    ~window_impl() noexcept = default;

    ST_DELETE_COPY_CTORS(window_impl);
    ST_DELETE_MOVE_CTORS(window_impl);

    SDL_Window     *window = nullptr;
    SDL_GLContext   gl_context = {};
    std::string     title = "libsticky " VERSION_STRING;
    st::bool_t      visible = false;
    st::bool_t      want_close = false;
    vsync_mode      vsync = Disabled;
    fullscreen_mode fullscreen = Windowed;
    glm::f32vec3    clear_colour = {0.0f, 0.0f, 0.0f};
    st::float64     delta_time = 0.0;
    st::float64     fixed_timestep = 0.02;
    st::uint64      perf_counter = 0;
    st::float64     accumulator = 0;
    st::gui::core  *gui = nullptr;
};

st::video::window::window() noexcept : m_impl{std::make_unique<window_impl>()}
{
    ST_ASSERT(SDL_WasInit(SDL_INIT_VIDEO) != 0);

    const st::float32     scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    const SDL_WindowFlags flags =
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    const auto width = static_cast<st::int32>(1280.0f * (scale > 0.0f ? scale : 1.0f));
    const auto height = static_cast<st::int32>(720.0f * (scale > 0.0f ? scale : 1.0f));

    m_impl->window = SDL_CreateWindow(m_impl->title.c_str(), width, height, flags);
    if (!sdl_check(m_impl->window != nullptr, "SDL_CreateWindow"))
    {
        st::os::popup::error("Could not open a window", SDL_GetError());
        return;
    }

    m_impl->gl_context = SDL_GL_CreateContext(m_impl->window);
    if (!sdl_check(m_impl->gl_context != nullptr, "SDL_GL_CreateContext") ||
        !sdl_check(SDL_GL_MakeCurrent(m_impl->window, m_impl->gl_context), "SDL_GL_MakeCurrent"))
    {
        st::os::popup::error("Could not open a window",
                             fmt::format("OpenGL 3.0 is required but could not be started: {}", SDL_GetError()));
        if (m_impl->gl_context != nullptr)
            SDL_GL_DestroyContext(m_impl->gl_context);
        SDL_DestroyWindow(m_impl->window);
        m_impl->gl_context = nullptr;
        m_impl->window = nullptr;
        return;
    }

    glewExperimental = GL_TRUE;
    const GLenum glew_status = glewInit();
    // GLEW cannot query the display server without GLX on Wayland, which is harmless with an SDL context.
    if (glew_status != GLEW_OK && glew_status != GLEW_ERROR_NO_GLX_DISPLAY)
        st::io::log::warn(
            fmt::format("glewInit failed: {}", reinterpret_cast<const char *>(glewGetErrorString(glew_status))));
    static_cast<void>(glGetError());

    static_cast<void>(sdl_check(SDL_SetWindowPosition(m_impl->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED),
                                "SDL_SetWindowPosition"));
    set_clear_colour(m_impl->clear_colour);
    set_title(m_impl->title);
    set_vsync(m_impl->vsync);
}

st::video::window::~window() noexcept
{
    // TODO: Move gamepad free to the event handler.
    st::io::gamepad::disconnect_all();
    if (m_impl->gl_context != nullptr)
        static_cast<void>(sdl_check(SDL_GL_DestroyContext(m_impl->gl_context), "SDL_GL_DestroyContext"));
    if (m_impl->window != nullptr)
        SDL_DestroyWindow(m_impl->window);
}

st::bool_t
st::video::window::is_valid() const noexcept
{
    return m_impl->window != nullptr;
}

void
st::video::window::show() noexcept
{
    if (m_impl->window != nullptr && sdl_check(SDL_ShowWindow(m_impl->window), "SDL_ShowWindow"))
        m_impl->visible = true;
}

void
st::video::window::hide() noexcept
{
    if (m_impl->window != nullptr && sdl_check(SDL_HideWindow(m_impl->window), "SDL_HideWindow"))
        m_impl->visible = false;
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
        if (e.type == SDL_EVENT_QUIT)
            close();
        else if (e.type == SDL_EVENT_GAMEPAD_ADDED)
            st::io::gamepad::connect(e.gdevice.which);
        else if (e.type == SDL_EVENT_GAMEPAD_REMOVED)
            st::io::gamepad::disconnect(e.gdevice.which);
        else if (m_impl->gui != nullptr)
            m_impl->gui->process_event(e);
    }
}

void
st::video::window::clear() noexcept
{
    if (m_impl->window == nullptr)
        return;

    st::int32 width = 0;
    st::int32 height = 0;
    SDL_GetWindowSizeInPixels(m_impl->window, &width, &height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
st::video::window::swap() noexcept
{
    if (m_impl->window != nullptr)
        static_cast<void>(sdl_check(SDL_GL_SwapWindow(m_impl->window), "SDL_GL_SwapWindow"));
}

void
st::video::window::accumulate() noexcept
{
    const st::uint64  current = SDL_GetPerformanceCounter();
    const st::float64 elapsed = static_cast<st::float64>(current - m_impl->perf_counter) /
        static_cast<st::float64>(SDL_GetPerformanceFrequency());
    const st::float64 frame_delta = glm::clamp(elapsed, 0.0, max_frame_delta);
    m_impl->perf_counter = current;
    m_impl->delta_time = glm::min(frame_delta, m_impl->fixed_timestep);
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
st::video::window::simulate(const std::function<void(st::float64)> &callback) noexcept
{
    while (m_impl->accumulator >= m_impl->fixed_timestep)
    {
        callback(m_impl->fixed_timestep);
        m_impl->accumulator -= m_impl->fixed_timestep;
    }
}

st::bool_t
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

glm::f32vec2
st::video::window::size() const noexcept
{
    st::int32 w = 0;
    st::int32 h = 0;
    if (m_impl->window != nullptr)
        SDL_GetWindowSize(m_impl->window, &w, &h);
    return {static_cast<st::float32>(w), static_cast<st::float32>(h)};
}

std::string
st::video::window::title() const noexcept
{
    return m_impl->title;
}

st::bool_t
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
st::video::window::set_clear_colour(glm::f32vec3 colour) noexcept
{
    m_impl->clear_colour = colour;
    if (m_impl->window != nullptr)
        glClearColor(colour.x, colour.y, colour.z, 1.0f);
}

void
st::video::window::set_fixed_timestep(st::float64 timestep) noexcept
{
    m_impl->fixed_timestep = glm::clamp(timestep, min_timestep, max_timestep);
}

void
st::video::window::set_fullscreen(fullscreen_mode mode) noexcept
{
    if (m_impl->window == nullptr)
        return;
    if (sdl_check(SDL_SetWindowFullscreen(m_impl->window, mode == Fullscreen), "SDL_SetWindowFullscreen"))
        m_impl->fullscreen = mode;
}

void
st::video::window::set_title(std::string_view title) noexcept
{
    m_impl->title = title;
    if (m_impl->window != nullptr)
        static_cast<void>(sdl_check(SDL_SetWindowTitle(m_impl->window, m_impl->title.c_str()), "SDL_SetWindowTitle"));
}

void
st::video::window::set_vsync(vsync_mode mode) noexcept
{
    if (m_impl->window == nullptr)
        return;

    if (mode == Adaptive)
    {
        if (SDL_GL_SetSwapInterval(-1))
        {
            m_impl->vsync = Adaptive;
            return;
        }
        st::io::log::info("Adaptive vsync is not supported. Using vsync instead.");
        mode = Enabled;
    }

    if (sdl_check(SDL_GL_SetSwapInterval(mode == Enabled ? 1 : 0), "SDL_GL_SetSwapInterval"))
        m_impl->vsync = mode;
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
    m_impl->gui = &gui;
}
