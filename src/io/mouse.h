/*
 * mouse.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#ifndef STICKY_IO_MOUSE_H
#define STICKY_IO_MOUSE_H

#include "st_includes.h"
#include "st_primitives.h"
#include "math/vec2.h"
#include "video/window.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ST_MOUSE_BUTTON_LEFT   SDL_BUTTON_LEFT
#define ST_MOUSE_BUTTON_RIGHT  SDL_BUTTON_RIGHT
#define ST_MOUSE_BUTTON_MIDDLE SDL_BUTTON_MIDDLE
#define ST_MOUSE_BUTTON_X1     SDL_BUTTON_X1
#define ST_MOUSE_BUTTON_X2     SDL_BUTTON_X2

st_bool st_mouse_init(void);
st_bool st_mouse_free(void);

st_vec2_float32_t st_mouse_get_delta_pos(void);
st_vec2_float32_t st_mouse_get_global_pos(void);
st_vec2_float32_t st_mouse_get_window_pos(void);

st_bool st_mouse_is_button_down(st_uint8 button);
st_bool st_mouse_is_button_up(st_uint8 button);

#ifdef __cplusplus
}
#endif

#endif // STICKY_IO_MOUSE_H
