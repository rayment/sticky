/*
 * mouse.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include "io/mouse.h"

st_bool
st_mouse_init(void)
{
    return true;
}

st_bool
st_mouse_free(void)
{
    return true;
}

st_vec2_float32_t
st_mouse_get_delta_pos(void)
{
    st_float32 x, y;
    ST_UNUSED SDL_MouseButtonFlags flags = SDL_GetRelativeMouseState(&x, &y);
    return (st_vec2_float32_t){x, y};
}

st_vec2_float32_t
st_mouse_get_global_pos(void)
{
    st_float32 x, y;
    ST_UNUSED SDL_MouseButtonFlags flags = SDL_GetGlobalMouseState(&x, &y);
    return (st_vec2_float32_t){x, y};
}

st_vec2_float32_t
st_mouse_get_window_pos(void)
{
    st_float32 x, y;
    ST_UNUSED SDL_MouseButtonFlags flags = SDL_GetMouseState(&x, &y);
    return (st_vec2_float32_t){x, y};
}

st_bool
st_mouse_is_button_down(st_uint8 button)
{
    SDL_MouseButtonFlags flags = SDL_GetMouseState(nullptr, nullptr);
    return (flags & SDL_BUTTON_MASK(button)) != 0;
}

st_bool
st_mouse_is_button_up(st_uint8 button)
{
    return !st_mouse_is_button_down(button);
}
