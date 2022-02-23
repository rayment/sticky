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

/**
 * @brief Denotes the left mouse button.
 * @hideinitializer
 * @since 1.0.0
 */
#define S_MOUSE_LEFT   1

/**
 * @brief Denotes the middle mouse button.
 * @hideinitializer
 * @since 1.0.0
 */
#define S_MOUSE_MIDDLE 2

/**
 * @brief Denotes the right mouse button.
 * @hideinitializer
 * @since 1.0.0
 */
#define S_MOUSE_RIGHT  3

/**
 * @brief Denotes the primary extra mouse button. Non-standard.
 * @hideinitializer
 * @since 1.0.0
 */
#define S_MOUSE_EXTRA1 4

/**
 * @brief Denotes the secondary extra mouse button. Non-standard.
 * @hideinitializer
 * @since 1.0.0
 */
#define S_MOUSE_EXTRA2 5

/**
 * @brief Denotes any possible mouse button.
 * @hideinitializer
 * @since 1.0.0
 */
#define S_MOUSE_ANY    6

/**
 * @brief Get the current motion of the mouse within the window.
 *
 * The current motion, or the change in position of the mouse pointer is updated
 * each time {@link S_window_poll(Swindow *)} is called, which should be once
 * per frame in the game loop.
 *
 * @param[out] x The change in x position of the mouse pointer in the window.
 * @param[out] y The change in y position of the mouse pointer in the window.
 * @since 1.0.0
 */
void   S_input_get_mouse_motion(Sint32 *, Sint32 *);

/**
 * @brief Get the current motion of the mouse wheel.
 *
 * The current motion of the mouse wheel is updated each time
 * {@link S_window_poll(Swindow *)} is called, which should be once per frame
 * in the game loop.
 *
 * @param[out] x The change in x position of the mouse wheel in the window.
 * @param[out] y The change in y position of the mouse wheel in the window.
 * @since 1.0.0
 */
void   S_input_get_mouse_wheel_motion(Sint32 *, Sint32 *);

/**
 * @brief Get the position of the mouse in the window.
 *
 * Takes the current position of the mosue pointer within the {@link Swindow}
 * instance and places it within two pointers @p x and @p y.
 *
 * @param[out] x The x position of the mouse pointer in the window.
 * @param[out] y The y position of the mouse pointer in the window.
 * @since 1.0.0
 */
void   S_input_get_mouse_pos(Sint32 *, Sint32 *);

/**
 * @brief Check if a given mouse button is currently being pressed down.
 *
 * Each time {@link S_window_poll(Swindow *)} is called, input is polled for.
 * This function is typically called once per frame. During the period of one
 * frame, if a button is detected is being depressed, then this function will
 * return {@link S_TRUE} for that given button. When the polling function is
 * called a second time, typically on the next frame, this function will return
 * {@link S_FALSE} if the button is released or kept down. The only way for it
 * to return {@link S_TRUE} again is to release the button and depress it again.
 *
 * @param[in] button The button to check for depression.
 * @return {@link S_TRUE} if the given button is being pressed down on the
 * current poll period. If {@link S_MOUSE_ANY} is specified and the condition
 * holds true for <i>any</i> button, then {@link S_TRUE} will also be returned.
 * Otherwise, {@link S_FALSE}.
 * @exception S_INVALID_ENUM If an invalid value is given instead of a mouse
 * button.
 * @since 1.0.0
 */
Sbool  S_input_is_button_down(Senum);

/**
 * @brief Check if a given button is currently being held down.
 *
 * This function will return {@link S_TRUE} so long as a button is currently
 * down, whether it be the frame upon whicih it was depressed, or any following
 * frame.
 *
 * @param[in] button The button to check.
 * @return {@link S_TRUE} if the given button is being held down.
 * If {@link S_MOUSE_ANY} is specified and <i>any</i> button is down, then
 * {@link S_TRUE} will also be returned. Otherwise, {@link S_FALSE}.
 * @exception S_INVALID_ENUM If an invalid value is given instead of a mouse
 * button.
 * @since 1.0.0
 */
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

