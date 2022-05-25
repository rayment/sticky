/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * gamepad.h
 * Gamepad input handler header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 05/09/2021
 */

#ifndef FR_RAYMENT_STICKY_GAMEPAD_H
#define FR_RAYMENT_STICKY_GAMEPAD_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @addtogroup gamepad
 * @{
 */

#include "sticky/common/includes.h"
#include "sticky/common/types.h"

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_AXIS_LEFT_HORIZONTAL  1

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_AXIS_LEFT_VERTICAL    2

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_AXIS_LEFT_TRIGGER     3

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_AXIS_RIGHT_HORIZONTAL 4

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_AXIS_RIGHT_VERTICAL   5

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_AXIS_RIGHT_TRIGGER    6

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_A              7

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_B              8

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_X              9

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_Y              10

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_BACK           11

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_GUIDE          12

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_START          13

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_LEFT_STICK     14

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_RIGHT_STICK    15

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_LEFT_TRIGGER   16

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_RIGHT_TRIGGER  17

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_DPAD_UP        18

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_DPAD_DOWN      19

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_DPAD_LEFT      20

/**
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_BUTTON_DPAD_RIGHT     21

/**
 * @brief Denotes any possible button or gamepad.
 * @hideinitializer
 * @since 1.0.0
 */
#define S_GAMEPAD_ANY    255

/**
 * @brief The maximum number of concurrently registered controllers allowed.
 * @since 1.0.0
 */
#define S_GAMEPAD_MAX_PADS              8

/**
 * @brief Get the current number of connected gamepads.
 *
 * Returns the current number of gamepads that are both connected to the
 * computer and recognised by the running program. This number will never exceed
 * {@link S_GAMEPAD_MAX_PADS}.
 *
 * @since 1.0.0
 */
Suint8 S_input_get_num_gamepads(void);

/**
 * @brief Get the current offset of one of the gamepad axis.
 *
 * The current position of the axis of the gamepad (typically horizontal and
 * vertical axis exist for both the left and right joysticks) may be obtained by
 * calling this function. Values range from {@link S_INT16_MIN} to
 * {@link S_INT16_MAX} where the larger the value in either direction
 * corresponds to a greater shift upon the axis.
 *
 * This value will be updated each time {@link S_window_poll(Swindow *)} is
 * called, which should be once per frame in the game loop.
 *
 * @warning This function does not support {@link S_GAMEPAD_ANY} and will throw
 * an error.
 * @param[in] gamepad The gamepad index (from 0 to {@link S_GAMEPAD_MAX_PADS} of
 * the gamepad to check.
 * @param[in] axis The axis to get the value from.
 * @return The current axis offset for a given axis on a given gamepad.
 * @exception S_INVALID_INDEX If an invalid gamepad index is provided.
 * @exception S_INVALID_ENUM If an invalid axis is provided.
 * @since 1.0.0
 */
Sint16 S_input_get_gamepad_axis(Suint8, Senum);

/**
 * @brief Check if a given gamepad button is currently being pressed down.
 *
 * Each time {@link S_window_poll(Swindow *)} is called, input is polled for.
 * This function is typically called once per frame. During the period of one
 * frame, if a gamepad button is detected is being depressed, then this function
 * will return {@link S_TRUE} for that given button for the gamepad that caused
 * the event. When the polling function is called a second time, typically on
 * the next frame, this function will return {@link S_FALSE} if the button is
 * released or kept down. The only way for it to return {@link S_TRUE} again is
 * to release the button and depress it again.
 *
 * @param[in] button The gamepad button to check for depression.
 * @param[in] gamepad The gamepad index (from 0 to {@link S_GAMEPAD_MAX_PADS} of
 * the gamepad to check, or {@link S_GAMEPAD_ANY}.
 * @return {@link S_TRUE} if the given button is being pressed down on the
 * current poll period. If {@link S_GAMEPAD_ANY} is specified and the condition
 * holds true for <i>any</i> button, then {@link S_TRUE} will also be returned.
 * Additionally, if {@link S_GAMEPAD_ANY} is used as the gamepad index,
 * {@link S_TRUE} will return if the case holds for any gamepad.
 * Otherwise, {@link S_FALSE}.
 * @exception S_INVALID_INDEX If an invalid gamepad index is provided.
 * @exception S_INVALID_ENUM If an invalid button is provided.
 * @since 1.0.0
 */
Sbool  S_input_is_gamepad_button_down(Suint8, Senum);

/**
 * @brief Check if a given gamepad button is currently being held down.
 *
 * This function will return {@link S_TRUE} so long as a button is currently
 * down, whether it be the frame upon whicih it was depressed, or any following
 * frame.
 *
 * @param[in] button The gamepad button to check.
 * @param[in] gamepad The gamepad index (from 0 to {@link S_GAMEPAD_MAX_PADS} of
 * the gamepad to check, or {@link S_GAMEPAD_ANY}.
 * @return {@link S_TRUE} if the given button is being held down.
 * If {@link S_GAMEPAD_ANY} is specified and the condition holds true for
 * <i>any</i> button, then {@link S_TRUE} will also be returned. Additionally,
 * if {@link S_GAMEPAD_ANY} is used as the gamepad index, {@link S_TRUE} will
 * return if the case holds for any gamepad. Otherwise, {@link S_FALSE}.
 * @exception S_INVALID_INDEX If an invalid gamepad index is provided.
 * @exception S_INVALID_ENUM If an invalid button is provided.
 * @since 1.0.0
 */
Sbool  S_input_is_gamepad_button_pressed(Suint8, Senum);

void  _S_input_gamepad_init(void);
void  _S_input_gamepad_reset(void);
void  _S_input_gamepad_event(SDL_Event);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_GAMEPAD_H */

