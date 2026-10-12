/*
 * log.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#ifndef STICKY_IO_LOG_H
#define STICKY_IO_LOG_H

#include "st_macros.h"
#include "st_types.h"

#ifdef __cplusplus
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace st::io::log
{
    void trace(std::string_view message) noexcept;
    void debug(std::string_view message) noexcept;
    void info(std::string_view message) noexcept;
    void warn(std::string_view message) noexcept;
    void error(std::string_view message) noexcept;

    constexpr std::size_t overlay_capacity = 10;

    struct overlay_line
    {
        st_bool     is_error = false;
        std::string text;
    };

    // Returns overlay_capacity slots of recent warnings and errors. Unused and
    // expired slots have empty text.
    std::vector<overlay_line> overlay_lines() noexcept;

    class logger
    {
    public:
        explicit logger(std::string_view name) noexcept;
        ~logger() noexcept;

        ST_DELETE_COPY_CTORS(logger);
        ST_DELETE_MOVE_CTORS(logger);

        void trace(std::string_view message) noexcept;
        void debug(std::string_view message) noexcept;
        void info(std::string_view message) noexcept;
        void warn(std::string_view message) noexcept;
        void error(std::string_view message) noexcept;

        void set_default() noexcept;

    private:
        struct logger_impl;
        std::unique_ptr<logger_impl> m_impl;
    };
} // namespace st::io::log

#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG
#define st_io_log_trace(message)
#define st_io_log_debug(message)
#else // NDEBUG
void st_io_log_trace(const st_char *message);
void st_io_log_debug(const st_char *message);
#endif // NDEBUG
void st_io_log_info(const st_char *message);
void st_io_log_warn(const st_char *message);
void st_io_log_error(const st_char *message);

#ifdef __cplusplus
}
#endif

#endif // STICKY_IO_LOG_H
