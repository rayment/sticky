/*
 * keyboard.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <SDL3/SDL.h>

#include "st_macros.h"
#include "io/keyboard.h"

st_bool
st::io::keyboard::key_down(st_keycode code) noexcept
{
    st_int32 len;
    const st_bool *states = SDL_GetKeyboardState(&len);
    ST_ASSERT(code < len);
    return states[code];
}

st_bool
st::io::keyboard::key_up(st_keycode code) noexcept
{
    return !key_down(code);
}
