/*
 * keyboard.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#ifndef STICKY_IO_KEYBOARD_H
#define STICKY_IO_KEYBOARD_H

#include "st_primitives.h"
#include "video/window.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef st_int32 st_keycode;

st_bool st_keyboard_init(void);
st_bool st_keyboard_free(void);

st_bool st_keyboard_is_key_down(st_keycode code);
st_bool st_keyboard_is_key_up(st_keycode code);

#ifdef __cplusplus
}
#endif

#endif // STICKY_IO_KEYBOARD_H
