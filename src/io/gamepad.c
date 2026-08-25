/*
 * gamepad.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include "st_macros.h"
#include "io/gamepad.h"


typedef struct st_gamepad
{
    SDL_Gamepad *handle;
    SDL_JoystickID id;
    st_bool connected;
} st_gamepad_t;

static st_gamepad_t gamepads[ST_MAX_GAMEPADS];

st_bool
st_gamepad_init(void)
{
    for (st_index i = 0; i < ST_MAX_GAMEPADS; ++i)
        gamepads[i].connected = false;
    return true;
}

st_bool
st_gamepad_free(void)
{
    return true;
}

st_bool
st_priv_gamepad_connect(st_index index,
                        SDL_Gamepad *handle,
                        SDL_JoystickID id)
{
    ST_ASSERT(index < ST_MAX_GAMEPADS && index != ST_INDEX_NONE);
    gamepads[index].handle = handle;
    gamepads[index].id = id;
    gamepads[index].connected = true;
    return true;
}

st_bool
st_priv_gamepad_disconnect(st_index index)
{
    ST_ASSERT(index < ST_MAX_GAMEPADS && index != ST_INDEX_NONE);
    gamepads[index].connected = false;
    return true;
}

st_index
st_priv_gamepad_by_id(SDL_JoystickID id)
{
    if (id == 0)
        return ST_INDEX_NONE;
    for (st_index i = 0; i < ST_MAX_GAMEPADS; ++i)
    {
        if (gamepads[i].connected && gamepads[i].id == id)
            return i;
    }
    return ST_INDEX_NONE;
}

st_index
st_priv_gamepad_next_disconnected_index(void)
{
    for (st_index i = 0; i < ST_MAX_GAMEPADS; ++i)
    {
        if (!gamepads[i].connected)
            return i;
    }
    return ST_INDEX_NONE;
}

st_int16
st_gamepad_axis(st_index gamepad,
                st_enum axis)
{
    if (gamepad == ST_INDEX_NONE || gamepad >= ST_MAX_GAMEPADS || !gamepads[gamepad].connected)
        return 0;
    return SDL_GetGamepadAxis(gamepads[gamepad].handle, ST_CAST(SDL_GamepadAxis, axis));
}

st_bool
st_gamepad_is_button_down(st_index gamepad,
                          st_enum button)
{
    if (gamepad == ST_INDEX_NONE || gamepad >= ST_MAX_GAMEPADS || !gamepads[gamepad].connected)
        return false;
    return SDL_GetGamepadButton(gamepads[gamepad].handle, ST_CAST(SDL_GamepadButton, button));
}

st_bool
st_gamepad_is_button_up(st_index gamepad,
                        st_enum button)
{
    if (gamepad == ST_INDEX_NONE || gamepad >= ST_MAX_GAMEPADS || !gamepads[gamepad].connected)
        return false;
    return !st_gamepad_is_button_down(gamepad, button);
}
