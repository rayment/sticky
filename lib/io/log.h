/*
 * log.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#ifndef STICKY_IO_LOG_H
#define STICKY_IO_LOG_H
#include "st_types.h"

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
void st_io_log_info (const st_char *message);
void st_io_log_warn (const st_char *message);
void st_io_log_error(const st_char *message);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <string_view>

namespace st::io::log
{
    void trace(std::string_view message) noexcept;
    void debug(std::string_view message) noexcept;
    void info (std::string_view message) noexcept;
    void warn (std::string_view message) noexcept;
    void error(std::string_view message) noexcept;
}

#endif // __cplusplus

#endif // STICKY_IO_LOG_H
