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
    st_float32 x, y;
    SDL_GetRelativeMouseState(&x, &y);
    return {x, y};
}

glm::f32vec2
st::io::mouse::global_pos() noexcept
{
    st_float32 x, y;
    SDL_GetGlobalMouseState(&x, &y);
    return {x, y};
}

glm::f32vec2
st::io::mouse::window_pos() noexcept
{
    st_float32 x, y;
    SDL_GetMouseState(&x, &y);
    return {x, y};
}

namespace
{
    template <st_uint8 Enum>
    st_bool
    button_down() noexcept
    {
        SDL_MouseButtonFlags flags = SDL_GetMouseState(nullptr, nullptr);
        return (flags & SDL_BUTTON_MASK(Enum)) != 0;
    }

    template <st_uint8 Enum>
    st_bool
    button_up() noexcept
    {
        return !button_down<Enum>();
    }
}

st_bool st::io::mouse::left_button_down  () noexcept { return button_down<SDL_BUTTON_LEFT>(); }
st_bool st::io::mouse::right_button_down () noexcept { return button_down<SDL_BUTTON_RIGHT>(); }
st_bool st::io::mouse::middle_button_down() noexcept { return button_down<SDL_BUTTON_MIDDLE>(); }
st_bool st::io::mouse::x1_button_down    () noexcept { return button_down<SDL_BUTTON_X1>(); }
st_bool st::io::mouse::x2_button_down    () noexcept { return button_down<SDL_BUTTON_X2>(); }

st_bool st::io::mouse::left_button_up  () noexcept { return button_up<SDL_BUTTON_LEFT>(); }
st_bool st::io::mouse::right_button_up () noexcept { return button_up<SDL_BUTTON_RIGHT>(); }
st_bool st::io::mouse::middle_button_up() noexcept { return button_up<SDL_BUTTON_MIDDLE>(); }
st_bool st::io::mouse::x1_button_up    () noexcept { return button_up<SDL_BUTTON_X1>(); }
st_bool st::io::mouse::x2_button_up    () noexcept { return button_up<SDL_BUTTON_X2>(); }
