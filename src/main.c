#include <locale.h>

#include "audio/client.h"
#include "audio/server.h"
#include "audio/source.h"
#include "io/gamepad.h"
#include "io/keyboard.h"
#include "io/mouse.h"
#include "log/log.h"
#include "lua/lua.h"
#include "video/window.h"

int
main(void)
{
    setlocale(LC_ALL, "C"); // disable system locale

    st_log_init();
    st_log_info("%s", "Initialised logging system.");

    st_window_init();

    st_keyboard_init();
    st_mouse_init();
    st_gamepad_init();

    st_audio_server_init();

    st_audio_client_t audio_cli;
    st_audio_client_create(&audio_cli);

    st_lua_t L;
    st_lua_create(&L);

    st_window_show();

    st_lua_exec_file(L, ST_STRING("mod/core/init.lua"));
    st_lua_exec_file(L, ST_STRING("mod/core/runtime.lua"));

    st_lua_call_function(L, ST_STRING("on_start"), 0, nullptr);

    st_window_set_fixed_step_rate(3);
    st_window_reset_timing();

    while (!st_window_is_close_requested() &&
           !st_keyboard_is_key_down(SDL_SCANCODE_ESCAPE))
    {
        st_window_update_delta();
        st_window_poll_events();

        while (st_window_pause_for_fixed_step())
            st_lua_call_function(L, ST_STRING("on_fixed_step"), 0, nullptr);

        st_float64 delta_time = st_window_delta_time();
        st_lua_call_function(L, ST_STRING("on_step"), 1, &(st_lua_arg_t) {
            .type = ST_LUA_ARG_NUMBER,
            .value.number = delta_time
        });

        st_window_new_frame();
        st_window_render_frame();
        st_window_swap_frame();
    }

    st_lua_call_function(L, ST_STRING("on_quit"), 0, nullptr);

    st_lua_destroy(L);

    st_audio_client_destroy(audio_cli);

    st_audio_server_free();

    st_gamepad_free();
    st_mouse_free();
    st_keyboard_free();

    st_window_free();

    st_log_info("%s", "Goodbye.");
    st_log_free();

    return 0;
}
