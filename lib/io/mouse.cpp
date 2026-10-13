/*
 * mouse.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <SDL3/SDL.h>
#include <glm/vec2.hpp>

#include "io/mouse.h"

glm::f32vec2
st::io::mouse::delta_pos() noexcept
{
    st::float32 x = 0.0f;
    st::float32 y = 0.0f;
    SDL_GetRelativeMouseState(&x, &y);
    return {x, y};
}

glm::f32vec2
st::io::mouse::global_pos() noexcept
{
    st::float32 x = 0.0f;
    st::float32 y = 0.0f;
    SDL_GetGlobalMouseState(&x, &y);
    return {x, y};
}

glm::f32vec2
st::io::mouse::window_pos() noexcept
{
    st::float32 x = 0.0f;
    st::float32 y = 0.0f;
    SDL_GetMouseState(&x, &y);
    return {x, y};
}

st::bool_t
st::io::mouse::button_down(st::io::mouse::button b) noexcept
{
    st::uint8 index = 0;
    switch (b)
    {
    case Left:
        index = SDL_BUTTON_LEFT;
        break;
    case Right:
        index = SDL_BUTTON_RIGHT;
        break;
    case Middle:
        index = SDL_BUTTON_MIDDLE;
        break;
    case X1:
        index = SDL_BUTTON_X1;
        break;
    case X2:
        index = SDL_BUTTON_X2;
        break;
    default:
        return false;
    }
    const SDL_MouseButtonFlags flags = SDL_GetMouseState(nullptr, nullptr);
    return (flags & SDL_BUTTON_MASK(index)) != 0;
}

st::bool_t
st::io::mouse::button_up(st::io::mouse::button b) noexcept
{
    return !button_down(b);
}
