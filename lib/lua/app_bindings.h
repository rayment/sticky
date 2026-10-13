/*
 * app_bindings.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

/**
 * @file
 * @brief The `st.app` Lua module.
 */

#ifndef STICKY_LUA_APP_BINDINGS_H
#define STICKY_LUA_APP_BINDINGS_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include "st_types.h"

/** @addtogroup lua
 *  @{
 */

/** @brief Control of the application from scripts, exposed to Lua as `st.app`. */
namespace st::lua::app
{

    /**
     * @brief Asks the engine to shut down gracefully once the current frame ends. Later requests are ignored.
     * @param code Exit status returned to the operating system.
     */
    void quit(st::int32 code) noexcept;

    /** @brief Whether a script has asked the engine to shut down. */
    [[nodiscard]] st::bool_t quit_requested() noexcept;
    /** @brief The exit status of the first quit request, or zero when none was made. */
    [[nodiscard]] st::int32 exit_code() noexcept;

} // namespace st::lua::app

/** @} */

#endif // STICKY_LUA_APP_BINDINGS_H
