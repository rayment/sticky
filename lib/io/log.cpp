/*
 * log.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <spdlog/spdlog.h>

#include "io/log.h"

#ifndef NDEBUG
void
st_io_log_trace(const st_char *message)
{
    st::io::log::trace(message);
}

void
st_io_log_debug(const st_char *message)
{
    st::io::log::debug(message);
}
#endif // NDEBUG

void
st_io_log_info(const st_char *message)
{
    st::io::log::info(message);
}

void
st_io_log_warn(const st_char *message)
{
    st::io::log::warn(message);
}

void
st_io_log_error(const st_char *message)
{
    st::io::log::error(message);
}

#ifndef NDEBUG
void
st::io::log::trace(std::string_view message) noexcept
{
    spdlog::trace(message);
}

void
st::io::log::debug(std::string_view message) noexcept
{
    spdlog::debug(message);
}
#endif // NDEBUG

void
st::io::log::info(std::string_view message) noexcept
{
    spdlog::info(message);
}

void
st::io::log::warn(std::string_view message) noexcept
{
    spdlog::warn(message);
}

void
st::io::log::error(std::string_view message) noexcept
{
    spdlog::error(message);
}
