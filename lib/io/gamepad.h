/*
 * gamepad.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#ifndef STICKY_GAMEPAD_H
#define STICKY_GAMEPAD_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <memory>
#include <unordered_map>

#include "st_macros.h"
#include "st_types.h"

namespace st::io
{

    class gamepad
    {
    public:
        enum axis
        {
            LeftX, LeftY,
            RightX, RightY,
            TriggerLeft, TriggerRight
        };

        enum button
        {
            North, East, South, West,
            Back, Guide, Start,
            LeftStick, RightStick, LeftShoulder, RightShoulder,
            DPadUp, DPadDown, DPadLeft, DPadRight,
            LeftPaddle1, LeftPaddle2, RightPaddle1, RightPaddle2,
            Touchpad,
            Misc1, Misc2, Misc3, Misc4, Misc5, Misc6
        };

        static std::unordered_map<st::size, std::shared_ptr<gamepad>> gamepads;

        static std::shared_ptr<gamepad> connect       (st::size id) noexcept;
        static void                     disconnect    (st::size id) noexcept;
        static void                     disconnect_all()            noexcept;

        ~gamepad() noexcept;

        ST_DELETE_COPY_CTORS(gamepad);
        ST_DELETE_MOVE_CTORS(gamepad);

        [[nodiscard]] st::size   id         ()         const noexcept;
        [[nodiscard]] st::int16  axis_value (axis a)   const noexcept;
        [[nodiscard]] st::bool_t button_down(button b) const noexcept;
        [[nodiscard]] st::bool_t button_up  (button b) const noexcept;

    private:
        gamepad(st::size id) noexcept;

        struct gamepad_impl;
        std::unique_ptr<gamepad_impl> m_impl;
    };

}

#endif // STICKY_GAMEPAD_H
