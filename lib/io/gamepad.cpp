/*
 * gamepad.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <limits>

#include <SDL3/SDL.h>
#include <fmt/format.h>

#include "io/gamepad.h"
#include "io/log.h"

struct st::io::gamepad::gamepad_impl
{
    gamepad_impl(st::size id) noexcept : pad_id(id) {}
    ~gamepad_impl() noexcept = default;

    SDL_Gamepad *handle = nullptr;
    st::size     pad_id = std::numeric_limits<st::size>::max();
    st::bool_t   connected = false;
};

std::unordered_map<st::size, std::shared_ptr<st::io::gamepad>> st::io::gamepad::gamepads;

st::io::gamepad::gamepad(st::size id) noexcept : m_impl(std::make_unique<gamepad_impl>(id))
{
    m_impl->handle = SDL_OpenGamepad(static_cast<SDL_JoystickID>(id));
    m_impl->connected = m_impl->handle != nullptr;
}

st::io::gamepad::~gamepad() noexcept = default;

std::shared_ptr<st::io::gamepad>
st::io::gamepad::connect(st::size id) noexcept
{
    const auto existing = gamepads.find(id);
    if (existing != gamepads.end())
        return existing->second;

    std::shared_ptr<gamepad> pad(new gamepad(id));
    if (!pad->m_impl->connected)
    {
        st::io::log::warn(fmt::format("Gamepad {} could not be opened: {}", id, SDL_GetError()));
        return nullptr;
    }
    gamepads.emplace(id, pad);
    st::io::log::debug(fmt::format("Gamepad {} connected.", id));
    return pad;
}

void
st::io::gamepad::disconnect(st::size id) noexcept
{
    auto iter = gamepads.find(id);
    if (iter == gamepads.end())
        return;
    // Closed here rather than in ~gamepad(), which may run much later while the gamepad is still referenced.
    SDL_CloseGamepad(iter->second->m_impl->handle);
    iter->second->m_impl->handle = nullptr;
    iter->second->m_impl->connected = false;
    gamepads.erase(iter);
    st::io::log::debug(fmt::format("Gamepad {} disconnected.", id));
}

void
st::io::gamepad::disconnect_all() noexcept
{
    for (auto &[id, pad] : gamepads)
    {
        SDL_CloseGamepad(pad->m_impl->handle);
        pad->m_impl->handle = nullptr;
        pad->m_impl->connected = false;
        st::io::log::debug(fmt::format("Gamepad {} disconnected.", pad->m_impl->pad_id));
    }
    gamepads.clear();
}

std::shared_ptr<st::io::gamepad>
st::io::gamepad::find(st::size id) noexcept
{
    const auto iter = gamepads.find(id);
    return iter != gamepads.end() ? iter->second : nullptr;
}

st::size
st::io::gamepad::count() noexcept
{
    return gamepads.size();
}

st::size
st::io::gamepad::id() const noexcept
{
    return m_impl->pad_id;
}

st::int16
st::io::gamepad::axis_value(axis a) const noexcept
{
    if (!m_impl->connected)
        return 0;
    SDL_GamepadAxis mask = SDL_GAMEPAD_AXIS_INVALID;
    switch (a)
    {
    case LeftX:
        mask = SDL_GAMEPAD_AXIS_LEFTX;
        break;
    case LeftY:
        mask = SDL_GAMEPAD_AXIS_LEFTY;
        break;
    case RightX:
        mask = SDL_GAMEPAD_AXIS_RIGHTX;
        break;
    case RightY:
        mask = SDL_GAMEPAD_AXIS_RIGHTY;
        break;
    case TriggerLeft:
        mask = SDL_GAMEPAD_AXIS_LEFT_TRIGGER;
        break;
    case TriggerRight:
        mask = SDL_GAMEPAD_AXIS_RIGHT_TRIGGER;
        break;
    default:
        return 0;
    }
    return SDL_GetGamepadAxis(m_impl->handle, mask);
}

st::bool_t
st::io::gamepad::button_down(button b) const noexcept
{
    if (!m_impl->connected)
        return false;
    SDL_GamepadButton mask = SDL_GAMEPAD_BUTTON_INVALID;
    switch (b)
    {
    case North:
        mask = SDL_GAMEPAD_BUTTON_NORTH;
        break;
    case East:
        mask = SDL_GAMEPAD_BUTTON_EAST;
        break;
    case South:
        mask = SDL_GAMEPAD_BUTTON_SOUTH;
        break;
    case West:
        mask = SDL_GAMEPAD_BUTTON_WEST;
        break;
    case Back:
        mask = SDL_GAMEPAD_BUTTON_BACK;
        break;
    case Guide:
        mask = SDL_GAMEPAD_BUTTON_GUIDE;
        break;
    case Start:
        mask = SDL_GAMEPAD_BUTTON_START;
        break;
    case LeftStick:
        mask = SDL_GAMEPAD_BUTTON_LEFT_STICK;
        break;
    case RightStick:
        mask = SDL_GAMEPAD_BUTTON_RIGHT_STICK;
        break;
    case LeftShoulder:
        mask = SDL_GAMEPAD_BUTTON_LEFT_SHOULDER;
        break;
    case RightShoulder:
        mask = SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER;
        break;
    case DPadUp:
        mask = SDL_GAMEPAD_BUTTON_DPAD_UP;
        break;
    case DPadDown:
        mask = SDL_GAMEPAD_BUTTON_DPAD_DOWN;
        break;
    case DPadLeft:
        mask = SDL_GAMEPAD_BUTTON_DPAD_LEFT;
        break;
    case DPadRight:
        mask = SDL_GAMEPAD_BUTTON_DPAD_RIGHT;
        break;
    case LeftPaddle1:
        mask = SDL_GAMEPAD_BUTTON_LEFT_PADDLE1;
        break;
    case LeftPaddle2:
        mask = SDL_GAMEPAD_BUTTON_LEFT_PADDLE2;
        break;
    case RightPaddle1:
        mask = SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1;
        break;
    case RightPaddle2:
        mask = SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2;
        break;
    case Touchpad:
        mask = SDL_GAMEPAD_BUTTON_TOUCHPAD;
        break;
    case Misc1:
        mask = SDL_GAMEPAD_BUTTON_MISC1;
        break;
    case Misc2:
        mask = SDL_GAMEPAD_BUTTON_MISC2;
        break;
    case Misc3:
        mask = SDL_GAMEPAD_BUTTON_MISC3;
        break;
    case Misc4:
        mask = SDL_GAMEPAD_BUTTON_MISC4;
        break;
    case Misc5:
        mask = SDL_GAMEPAD_BUTTON_MISC5;
        break;
    case Misc6:
        mask = SDL_GAMEPAD_BUTTON_MISC6;
        break;
    default:
        return false;
    }
    return SDL_GetGamepadButton(m_impl->handle, mask);
}

st::bool_t
st::io::gamepad::button_up(button b) const noexcept
{
    if (!m_impl->connected)
        return false;
    return !button_down(b);
}
