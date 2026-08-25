/*
 * gamepad.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#ifndef STICKY_IO_GAMEPAD_H
#define STICKY_IO_GAMEPAD_H

#include "st_includes.h"
#include "st_primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ST_MAX_GAMEPADS 4

st_bool st_gamepad_init(void);
st_bool st_gamepad_free(void);

st_int16 st_gamepad_axis          (st_index gamepad, st_enum axis);
st_bool  st_gamepad_is_button_down(st_index gamepad, st_enum button);
st_bool  st_gamepad_is_button_up  (st_index gamepad, st_enum button);

st_bool  st_priv_gamepad_connect                (st_index index, SDL_Gamepad *handle, SDL_JoystickID id);
st_bool  st_priv_gamepad_disconnect             (st_index index);
st_index st_priv_gamepad_by_id                  (SDL_JoystickID id);
st_index st_priv_gamepad_next_disconnected_index(void);

#ifdef __cplusplus
}
#endif

#endif // STICKY_IO_GAMEPAD_H
