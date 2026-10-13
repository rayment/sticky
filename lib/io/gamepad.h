/*
 * gamepad.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

/**
 * @file
 * @brief Connected gamepads and their polled state.
 */

#ifndef STICKY_IO_GAMEPAD_H
#define STICKY_IO_GAMEPAD_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <memory>
#include <unordered_map>

#include "st_macros.h"
#include "st_types.h"

/** @addtogroup io
 *  @{
 */

namespace st::io
{

    /**
     * @brief A gamepad, connected and disconnected by st::video::window::poll() as devices come and go.
     *
     * A gamepad that was disconnected stays valid for as long as it is referenced, but reports every axis at zero and
     * every button as neither down nor up.
     */
    class gamepad
    {
    public:
        /**
         * @brief An analogue axis.
         *
         * Stick axes range from -32768 to 32767, with negative values to the left and up. Triggers range from 0 when
         * released to 32767.
         */
        enum axis : st::uint8
        {
            LeftX, ///< Horizontal position of the left stick.
            LeftY, ///< Vertical position of the left stick.
            RightX, ///< Horizontal position of the right stick.
            RightY, ///< Vertical position of the right stick.
            TriggerLeft, ///< The left trigger.
            TriggerRight, ///< The right trigger.
        };

        /**
         * @brief A button, named by position so that the layout of the controller does not matter.
         *
         * The face buttons are North, East, South and West; South is "A" on an Xbox controller.
         */
        enum button : st::uint8
        {
            North, ///< The top face button.
            East, ///< The right face button.
            South, ///< The bottom face button.
            West, ///< The left face button.
            Back, ///< The left centre button, such as "Back" or "Select".
            Guide, ///< The middle centre button, such as the logo button.
            Start, ///< The right centre button, such as "Start" or "Menu".
            LeftStick, ///< Pressing the left stick.
            RightStick, ///< Pressing the right stick.
            LeftShoulder, ///< The left bumper.
            RightShoulder, ///< The right bumper.
            DPadUp, ///< Up on the directional pad.
            DPadDown, ///< Down on the directional pad.
            DPadLeft, ///< Left on the directional pad.
            DPadRight, ///< Right on the directional pad.
            LeftPaddle1, ///< The upper left rear paddle.
            LeftPaddle2, ///< The lower left rear paddle.
            RightPaddle1, ///< The upper right rear paddle.
            RightPaddle2, ///< The lower right rear paddle.
            Touchpad, ///< Pressing the touchpad.
            Misc1, ///< A controller-specific extra button, such as "Share".
            Misc2, ///< A controller-specific extra button.
            Misc3, ///< A controller-specific extra button.
            Misc4, ///< A controller-specific extra button.
            Misc5, ///< A controller-specific extra button.
            Misc6, ///< A controller-specific extra button.
        };

        /**
         * @brief Opens a gamepad, or returns it if it is already connected.
         * @param id The `SDL_JoystickID` of the device.
         * @return The gamepad, or null if the device could not be opened. The reason is logged.
         */
        static std::shared_ptr<gamepad> connect(st::size id) noexcept;
        /** @brief Closes a connected gamepad. Unknown ids are ignored. @param id The id given to connect(). */
        static void disconnect(st::size id) noexcept;
        /** @brief Closes every connected gamepad. */
        static void disconnect_all() noexcept;
        /** @brief The connected gamepad with an id, or null. @param id The id given to connect(). */
        [[nodiscard]] static std::shared_ptr<gamepad> find(st::size id) noexcept;
        /** @brief The number of connected gamepads. */
        [[nodiscard]] static st::size count() noexcept;

        ~gamepad() noexcept;

        ST_DELETE_COPY_CTORS(gamepad);
        ST_DELETE_MOVE_CTORS(gamepad);

        /** @brief The id the gamepad was connected with. */
        [[nodiscard]] st::size id() const noexcept;
        /** @brief The position of an axis, from -32768 to 32767. @param a The axis. */
        [[nodiscard]] st::int16 axis_value(axis a) const noexcept;
        /** @brief Whether a button is held down. @param b The button. */
        [[nodiscard]] st::bool_t button_down(button b) const noexcept;
        /** @brief Whether a button is not held down. False once disconnected. @param b The button. */
        [[nodiscard]] st::bool_t button_up(button b) const noexcept;

    private:
        explicit gamepad(st::size id) noexcept;

        static std::unordered_map<st::size, std::shared_ptr<gamepad>> gamepads;

        struct gamepad_impl;
        std::unique_ptr<gamepad_impl> m_impl;
    };

} // namespace st::io

/** @} */

#endif // STICKY_IO_GAMEPAD_H
