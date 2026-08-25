/*
 * window.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/08/2026
 */

#include "gui/backend/gui_backend.h"
#include "io/gamepad.h"
#include "log/log.h"
#include "os/popup.h"
#include "video/window.h"

#define ST_SET_GL_ATTRIB(attr, value) \
    { \
        if (ST_UNLIKELY(!SDL_GL_SetAttribute(attr, value))) \
        { \
            handle_sdl_error("Error: SDL_GL_SetAttribute()"); \
            return false; \
        } \
    }

#define ST_SAFE_CALL(call, compare, error_title) \
    { \
        if (ST_UNLIKELY((call) == compare)) \
        { \
            handle_sdl_error("Error: " error_title); \
            return false; \
        } \
    }

static void
handle_sdl_error(const st_char *title)
{
    st_size title_len = strlen(title);
    const st_char *sdl_error = SDL_GetError();
    st_size sdl_error_len = strlen(sdl_error);
    st_popup_error(ST_STRING(title, title_len), ST_STRING(sdl_error, sdl_error_len));
}

typedef struct st_window
{
    SDL_Window *sdl_win;
    SDL_GLContext sdl_gl;
    st_bool want_close;
    st_vec2_int32_t size;
    st_vec3_float32_t clear_color;
    st_uint64 perf_counter;
    st_float64 delta_time;
    st_float64 fixed_step_accumulator;
    st_uint32 fixed_step_rate;
} st_window_t;

static st_window_t win;

st_bool
st_window_init(void)
{
    ST_SAFE_CALL(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD), false, "SDL_Init()");

    ST_SET_GL_ATTRIB(SDL_GL_CONTEXT_FLAGS, 0);
    ST_SET_GL_ATTRIB(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    ST_SET_GL_ATTRIB(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    ST_SET_GL_ATTRIB(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    ST_SET_GL_ATTRIB(SDL_GL_DOUBLEBUFFER, 1);
    ST_SET_GL_ATTRIB(SDL_GL_DEPTH_SIZE, 24);
    ST_SET_GL_ATTRIB(SDL_GL_STENCIL_SIZE, 8);
#ifdef __APPLE__
    ST_SET_GL_ATTRIB(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
#endif

    st_gui_backend_init();

    st_float32 scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    win.size.x= ST_CAST(st_int32, 1280 * scale);
    win.size.y= ST_CAST(st_int32, 720 * scale);
    win.sdl_win = SDL_CreateWindow(PROGRAM_TITLE, win.size.x, win.size.y, window_flags);
    ST_SAFE_CALL(win.sdl_win, nullptr, "SDL_CreateWindow()");
    win.sdl_gl = SDL_GL_CreateContext(win.sdl_win);
    ST_SAFE_CALL(win.sdl_gl, nullptr, "SDL_GL_CreateContext()");

    ST_SAFE_CALL(SDL_GL_MakeCurrent(win.sdl_win, win.sdl_gl), false, "SDL_GL_MakeCurrent()");
    ST_SAFE_CALL(SDL_GL_SetSwapInterval(1), false, "SDL_GL_SetSwapInterval()");

    ST_SAFE_CALL(st_gui_backend_create(win.sdl_win, win.sdl_gl), false, "st_gui_backend_create()");

    win.want_close = false;

    st_window_set_clear_color((st_vec3_float32_t){0.1f, 0.1f, 0.25f});
    st_window_center();

    win.perf_counter = SDL_GetPerformanceCounter();
    win.delta_time = 0.0l;
    win.fixed_step_accumulator = 0.0l;
    win.fixed_step_rate = 20;

    return true;
}

st_bool
st_window_free(void)
{
    ST_SAFE_CALL(st_gui_backend_destroy(), false, "st_gui_backend_destroy()");
    ST_SAFE_CALL(SDL_GL_DestroyContext(win.sdl_gl), false, "SDL_GL_DestroyContext()");
    SDL_DestroyWindow(win.sdl_win);
    ST_SAFE_CALL(st_gui_backend_free(), false, "st_gui_backend_free()");
    SDL_Quit();
    return true;
}

st_bool
st_window_center(void)
{
    ST_SAFE_CALL(SDL_SetWindowPosition(win.sdl_win, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED), false, "SDL_SetWindowPosition()");
    return true;
}

st_bool
st_window_close(void)
{
    win.want_close = true;
    return true;
}

st_bool
st_window_show(void)
{
    ST_SAFE_CALL(SDL_ShowWindow(win.sdl_win), false, "SDL_ShowWindow()");
    return true;
}

st_bool
st_window_hide(void)
{
    ST_SAFE_CALL(SDL_HideWindow(win.sdl_win), false, "SDL_HideWindow()");
    return true;
}

st_bool
st_window_set_clear_color(st_vec3_float32_t color)
{
    win.clear_color = color;
    return true;
}

st_float64
st_window_delta_time(void)
{
    return win.delta_time;
}

st_bool
st_window_reset_timing(void)
{
    win.fixed_step_accumulator = 0.0;
    win.delta_time = 0;
    win.perf_counter = SDL_GetPerformanceCounter();
    return true;
}

st_bool
st_window_pause_for_fixed_step(void)
{
    st_float64 fixed_dt = 1.0 / ST_CAST(st_float64, win.fixed_step_rate);
    st_bool result = win.fixed_step_accumulator >= fixed_dt;
    if (result)
        win.fixed_step_accumulator -= fixed_dt;
    return result;
}

st_bool
st_window_update_delta(void)
{
    st_uint64 now = SDL_GetPerformanceCounter();
    if (now > win.perf_counter)
        win.delta_time = ST_CAST(st_float64, now - win.perf_counter) / ST_CAST(st_float64, SDL_GetPerformanceFrequency());
    win.perf_counter = now;
    win.fixed_step_accumulator += win.delta_time;
    return true;
}

st_bool
st_window_new_frame(void)
{
    glViewport(0, 0, win.size.x, win.size.y);
    glClearColor(win.clear_color.x, win.clear_color.y, win.clear_color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    st_gui_backend_new_frame();
    return true;
}

st_bool
st_window_poll_events(void)
{
    // TODO: Move all of this into the event handler.
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        st_gui_backend_process_event(e);
        if (e.type == SDL_EVENT_QUIT)
        {
            st_window_close();
        }
        else if (e.type == SDL_EVENT_GAMEPAD_ADDED)
        {
            st_index next = st_priv_gamepad_next_disconnected_index();
            if (next != ST_INDEX_NONE)
            {
                SDL_Gamepad *gamepad = SDL_OpenGamepad(e.gdevice.which);
                st_priv_gamepad_connect(next, gamepad, e.gdevice.which);
                st_log_debug("Gamepad %d connected.", next);
            }
            else
            {
                st_log_debug("No more available gamepad slots. Ignoring connect event.");
            }
        }
        else if (e.type == SDL_EVENT_GAMEPAD_REMOVED)
        {
            st_index idx = st_priv_gamepad_by_id(e.gdevice.which);
            if (idx != ST_INDEX_NONE)
            {
                st_priv_gamepad_disconnect(idx);
                st_log_debug("Gamepad %d disconnected.", idx);
            }
            else
            {
                st_log_debug("Gamepad disconnected but was not active. Ignoring disconnect event.");
            }
        }
    }
    return true;
}

st_bool
st_window_render_frame(void)
{
    st_gui_backend_render_frame();
    return true;
}

st_bool
st_window_swap_frame(void)
{
    st_gui_backend_swap_frame();
    ST_SAFE_CALL(SDL_GL_SwapWindow(win.sdl_win), false, "SDL_GL_SwapWindow()");
    return true;
}

st_bool
st_window_set_title(st_string_t title)
{
    ST_STRING_SAFE_CSTR(title, safe_title);
    ST_SAFE_CALL(SDL_SetWindowTitle(win.sdl_win, safe_title), false, "SDL_SetWindowTitle()");
    return true;
}

st_bool
st_window_set_fixed_step_rate(st_uint32 rate)
{
    win.fixed_step_rate = rate;
    return true;
}

st_bool
st_window_is_close_requested(void)
{
    return win.want_close;
}
