/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mouse.h
 * Mouse input handler header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 05/09/2021
 */

#ifndef FR_RAYMENT_STICKY_MOUSE_H
#define FR_RAYMENT_STICKY_MOUSE_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @addtogroup mouse
 * @{
 */

#include "sticky/common/includes.h"
#include "sticky/common/types.h"

#define S_MOUSE_LEFT   1
#define S_MOUSE_MIDDLE 2
#define S_MOUSE_RIGHT  3
#define S_MOUSE_EXTRA1 4
#define S_MOUSE_EXTRA2 5
#define S_MOUSE_ANY    6

void   S_input_get_mouse_motion(Sint32 *, Sint32 *);
void   S_input_get_mouse_wheel_motion(Sint32 *, Sint32 *);
void   S_input_get_mouse_pos(Sint32 *, Sint32 *);

Sbool  S_input_is_button_down(Senum);
Sbool  S_input_is_button_pressed(Senum);

void  _S_input_mouse_init(void);
void  _S_input_mouse_reset(void);
void  _S_input_mouse_event(SDL_Event);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MOUSE_H */

