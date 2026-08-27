/*
 * keyboard.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#ifndef STICKY_IO_KEYBOARD_H
#define STICKY_IO_KEYBOARD_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include "st_types.h"

namespace st::io::keyboard
{
    typedef st::int32 keycode;

    [[nodiscard]] st::bool_t key_down(keycode code) noexcept;
    [[nodiscard]] st::bool_t key_up  (keycode code) noexcept;
}

#endif // STICKY_IO_KEYBOARD_H
