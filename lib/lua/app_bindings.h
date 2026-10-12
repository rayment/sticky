/*
 * app_bindings.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#ifndef STICKY_LUA_APP_BINDINGS_H
#define STICKY_LUA_APP_BINDINGS_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include "st_types.h"

namespace st::lua::app
{

    // Asks the engine to shut down gracefully, returning `code` to the operating system.
    void quit(st::int32 code) noexcept;

    // Whether a script has asked the engine to shut down.
    [[nodiscard]] st::bool_t quit_requested() noexcept;
    // The exit code of the first quit request, or zero when none was made.
    [[nodiscard]] st::int32 exit_code() noexcept;

} // namespace st::lua::app

#endif // STICKY_LUA_APP_BINDINGS_H
