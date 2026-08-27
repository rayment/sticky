/*
 * keyboard.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <SDL3/SDL.h>

#include "st_macros.h"
#include "io/keyboard.h"

st::bool_t
st::io::keyboard::key_down(st::io::keyboard::keycode code) noexcept
{
    st::int32 len;
    const st::bool_t *states = SDL_GetKeyboardState(&len);
    ST_ASSERT(code < len);
    return states[code];
}

st::bool_t
st::io::keyboard::key_up(st::io::keyboard::keycode code) noexcept
{
    return !key_down(code);
}
