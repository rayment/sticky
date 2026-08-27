/*
 * gamepad.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <SDL3/SDL.h>

#include "io/gamepad.h"

#include <limits>

struct st::io::gamepad::gamepad_impl
{
    gamepad_impl (st::size id) noexcept
        : pad_id(id)
    {}
    ~gamepad_impl() noexcept = default;

    SDL_Gamepad *handle    = nullptr;
    st::size     pad_id    = std::numeric_limits<st::size>::max();
    st::bool_t   connected = false;
};

std::unordered_map<st::size, std::shared_ptr<st::io::gamepad>> st::io::gamepad::gamepads;

st::io::gamepad::gamepad(st::size id) noexcept
    : m_impl(std::make_unique<gamepad_impl>(id))
{
    SDL_Gamepad *pad= SDL_OpenGamepad(static_cast<SDL_JoystickID>(id));
    ST_ASSERT(pad);
    m_impl->handle = pad;
    m_impl->pad_id = id;
    m_impl->connected = true;
}

st::io::gamepad::~gamepad() noexcept {}

std::shared_ptr<st::io::gamepad>
st::io::gamepad::connect(st::size id) noexcept
{
    if (ST_UNLIKELY(gamepads.find(id) != gamepads.end()))
    {
        std::shared_ptr<gamepad> pad = gamepads[id];
        pad->m_impl->connected = true;
        return pad;
    }
    else
    {
        std::shared_ptr<gamepad> pad = std::shared_ptr<gamepad>(new gamepad(id));
        pad->m_impl->connected = true;
        gamepads[id] = pad;
        return pad;
    }
}

void
st::io::gamepad::disconnect(st::size id) noexcept
{
    auto iter = gamepads.find(id);
    ST_ASSERT(iter != gamepads.end());
    iter->second->m_impl->handle = nullptr;
    iter->second->m_impl->connected = false;
    gamepads.erase(iter);
}

st::size
st::io::gamepad::id() const noexcept
{
    return m_impl->pad_id;
}

st_int16
st::io::gamepad::axis_value(axis a) const noexcept
{
    if (!m_impl->connected)
        return 0;
    st_uint8 mask;
    switch (a)
    {
    case LeftX:        mask = SDL_BUTTON_LEFT;  break;
    case LeftY:        mask = SDL_BUTTON_LEFT;  break;
    case RightX:       mask = SDL_BUTTON_RIGHT; break;
    case RightY:       mask = SDL_BUTTON_RIGHT; break;
    case TriggerLeft:  mask = SDL_BUTTON_X1;    break;
    case TriggerRight: mask = SDL_BUTTON_X2;    break;
    default: ST_ASSERT(false);                  break;
    }
    return SDL_GetGamepadAxis(m_impl->handle, static_cast<SDL_GamepadAxis>(mask));
}

st::bool_t
st::io::gamepad::button_down(button b) const noexcept
{
    if (!m_impl->connected)
        return false;
    st_uint8 mask;
    switch (b)
    {
    case North:         mask = SDL_GAMEPAD_BUTTON_NORTH;          break;
    case East:          mask = SDL_GAMEPAD_BUTTON_EAST;           break;
    case South:         mask = SDL_GAMEPAD_BUTTON_SOUTH;          break;
    case West:          mask = SDL_GAMEPAD_BUTTON_WEST;           break;
    case Back:          mask = SDL_GAMEPAD_BUTTON_BACK;           break;
    case Guide:         mask = SDL_GAMEPAD_BUTTON_GUIDE;          break;
    case Start:         mask = SDL_GAMEPAD_BUTTON_START;          break;
    case LeftStick:     mask = SDL_GAMEPAD_BUTTON_LEFT_STICK;     break;
    case RightStick:    mask = SDL_GAMEPAD_BUTTON_RIGHT_STICK;    break;
    case LeftShoulder:  mask = SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;  break;
    case RightShoulder: mask = SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER; break;
    case DPadUp:        mask = SDL_GAMEPAD_BUTTON_DPAD_UP;        break;
    case DPadDown:      mask = SDL_GAMEPAD_BUTTON_DPAD_DOWN;      break;
    case DPadLeft:      mask = SDL_GAMEPAD_BUTTON_DPAD_LEFT;      break;
    case DPadRight:     mask = SDL_GAMEPAD_BUTTON_DPAD_RIGHT;     break;
    case LeftPaddle1:   mask = SDL_GAMEPAD_BUTTON_LEFT_PADDLE1;   break;
    case LeftPaddle2:   mask = SDL_GAMEPAD_BUTTON_LEFT_PADDLE2;   break;
    case RightPaddle1:  mask = SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1;  break;
    case RightPaddle2:  mask = SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2;  break;
    case Touchpad:      mask = SDL_GAMEPAD_BUTTON_TOUCHPAD;       break;
    case Misc1:         mask = SDL_GAMEPAD_BUTTON_MISC1;          break;
    case Misc2:         mask = SDL_GAMEPAD_BUTTON_MISC2;          break;
    case Misc3:         mask = SDL_GAMEPAD_BUTTON_MISC3;          break;
    case Misc4:         mask = SDL_GAMEPAD_BUTTON_MISC4;          break;
    case Misc5:         mask = SDL_GAMEPAD_BUTTON_MISC5;          break;
    case Misc6:         mask = SDL_GAMEPAD_BUTTON_MISC6;          break;
    default: ST_ASSERT(false);                   break;
    }
    return SDL_GetGamepadButton(m_impl->handle, static_cast<SDL_GamepadButton>(mask));
}

st::bool_t
st::io::gamepad::button_up(button b) const noexcept
{
    if (!m_impl->connected)
        return false;
    return !button_down(b);
}
