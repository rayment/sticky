/*
 * mouse.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <glm/vec2.hpp>
#include <SDL3/SDL.h>

#include "st_macros.h"
#include "io/mouse.h"

glm::f32vec2
st::io::mouse::delta_pos() noexcept
{
    st::float32 x, y;
    SDL_GetRelativeMouseState(&x, &y);
    return {x, y};
}

glm::f32vec2
st::io::mouse::global_pos() noexcept
{
    st::float32 x, y;
    SDL_GetGlobalMouseState(&x, &y);
    return {x, y};
}

glm::f32vec2
st::io::mouse::window_pos() noexcept
{
    st::float32 x, y;
    SDL_GetMouseState(&x, &y);
    return {x, y};
}

st::bool_t
st::io::mouse::button_down(st::io::mouse::button b) noexcept
{
    st::uint8 mask;
    switch (b)
    {
    case Left:   mask = SDL_BUTTON_LEFT;   break;
    case Right:  mask = SDL_BUTTON_RIGHT;  break;
    case Middle: mask = SDL_BUTTON_MIDDLE; break;
    case X1:     mask = SDL_BUTTON_X1;     break;
    case X2:     mask = SDL_BUTTON_X2;     break;
    default: ST_ASSERT(false); ST_UNREACHABLE();
    }
    SDL_MouseButtonFlags flags = SDL_GetMouseState(nullptr, nullptr);
    return (flags & SDL_BUTTON_MASK(mask)) != 0;
}

st::bool_t
st::io::mouse::button_up(st::io::mouse::button b) noexcept
{
    return !button_down(b);
}
