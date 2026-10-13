/*
 * keyboard.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

/**
 * @file
 * @brief Polled keyboard state.
 */

#ifndef STICKY_IO_KEYBOARD_H
#define STICKY_IO_KEYBOARD_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include "st_types.h"

/** @addtogroup io
 *  @{
 */

/** @brief The state of the keyboard, updated by st::video::window::poll(). */
namespace st::io::keyboard
{
    /** @brief A physical key position, independent of the keyboard layout. Values are `SDL_Scancode`. */
    typedef st::int32 scancode;

    /** @brief Whether the key is held down. Unknown scancodes are never down. @param code The key. */
    [[nodiscard]] st::bool_t key_down(scancode code) noexcept;
    /** @brief Whether the key is not held down. Unknown scancodes are always up. @param code The key. */
    [[nodiscard]] st::bool_t key_up(scancode code) noexcept;
} // namespace st::io::keyboard

/** @} */

#endif // STICKY_IO_KEYBOARD_H
