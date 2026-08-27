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

namespace st::io::log
{
    class logger; // forward declaration
}

namespace st::lua
{
    namespace log
    {
        void trace(std::string_view message) noexcept;
        void debug(std::string_view message) noexcept;
        void info (std::string_view message) noexcept;
        void warn (std::string_view message) noexcept;
        void error(std::string_view message) noexcept;
    }

    [[nodiscard]] st::io::log::logger *logger() noexcept;

    void set_logger(st::io::log::logger *logger) noexcept;
}

#endif // STICKY_LUA_LOG_BINDINGS_H
