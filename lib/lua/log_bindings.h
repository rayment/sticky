/*
 * log_bindings.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 28/08/2026
 */

#ifndef STICKY_LUA_LOG_BINDINGS_H
#define STICKY_LUA_LOG_BINDINGS_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <string_view>

struct lua_State;

namespace st::io::log
{
    class logger; // forward declaration
}

namespace st::lua
{
    // Messages are written to the logger of the Lua state of the caller, so that a call made from a callback that the
    // engine fired at any time is attributed to the script that made it.
    namespace log
    {
        void trace(std::string_view message, lua_State *state) noexcept;
        void debug(std::string_view message, lua_State *state) noexcept;
        void info (std::string_view message, lua_State *state) noexcept;
        void warn (std::string_view message, lua_State *state) noexcept;
        void error(std::string_view message, lua_State *state) noexcept;
    }

    // Selects the logger of a state, or none when null. The logger must outlive the state.
    void set_logger(lua_State *L, st::io::log::logger *logger) noexcept;
}

#endif // STICKY_LUA_LOG_BINDINGS_H
