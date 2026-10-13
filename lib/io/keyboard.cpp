/*
 * keyboard.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <SDL3/SDL.h>

#include "io/keyboard.h"

st::bool_t
st::io::keyboard::key_down(st::io::keyboard::scancode code) noexcept
{
    st::int32         count = 0;
    const st::bool_t *states = SDL_GetKeyboardState(&count);
    if (states == nullptr || code < 0 || code >= count)
        return false;
    return states[code];
}

st::bool_t
st::io::keyboard::key_up(st::io::keyboard::scancode code) noexcept
{
    return !key_down(code);
}
