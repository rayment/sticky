/*
 * keyboard.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include "st_includes.h"
#include "io/keyboard.h"

st_bool
st_keyboard_init(void)
{
    return true;
}

st_bool
st_keyboard_free(void)
{
    return true;
}

st_bool
st_keyboard_is_key_down(st_keycode code)
{
    const st_bool *states = SDL_GetKeyboardState(nullptr);
    return states[code];
}

st_bool
st_keyboard_is_key_up(st_keycode code)
{
    return !st_keyboard_is_key_down(code);
}
