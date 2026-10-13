/*
 * log_bindings.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 28/08/2026
 */

/**
 * @file
 * @brief The `st.io.log` Lua module.
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

/** @addtogroup lua
 *  @{
 */

namespace st::lua
{
    /**
     * @brief Logging from scripts, exposed to Lua as `st.io.log`.
     *
     * Messages are written to the logger of the Lua state of the caller, so that a call made from a callback that the
     * engine fired at any time is attributed to the script that made it. Without a logger, messages are dropped.
     */
    namespace log
    {
        /** @brief Logs a trace message. @param message Text. @param state Lua state of the caller. */
        void trace(std::string_view message,
                   lua_State       *state) noexcept;
        /** @brief Logs a debug message. @param message Text. @param state Lua state of the caller. */
        void debug(std::string_view message,
                   lua_State       *state) noexcept;
        /** @brief Logs an information message. @param message Text. @param state Lua state of the caller. */
        void info(std::string_view message,
                  lua_State       *state) noexcept;
        /** @brief Logs a warning. @param message Text. @param state Lua state of the caller. */
        void warn(std::string_view message,
                  lua_State       *state) noexcept;
        /** @brief Logs an error. @param message Text. @param state Lua state of the caller. */
        void error(std::string_view message,
                   lua_State       *state) noexcept;
    } // namespace log

    /**
     * @brief Sets the logger that `st.io.log` of a Lua state writes to.
     * @param L The Lua state.
     * @param logger The logger, which must outlive the state, or null to drop messages.
     */
    void set_logger(lua_State           *L,
                    st::io::log::logger *logger) noexcept;
} // namespace st::lua

/** @} */

#endif // STICKY_LUA_LOG_BINDINGS_H
