/*
 * log.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <ctime>
#include <filesystem>
#include <vector>

#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "st_macros.h"
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

namespace
{
    constexpr st_bool
    is_debug_level(spdlog::level::level_enum lvl) noexcept
    {
        return lvl == spdlog::level::trace || lvl == spdlog::level::debug;
    }

    constexpr spdlog::level::level_enum default_log_level() noexcept
    {
#ifdef NDEBUG
        return spdlog::level::info;
#else // NDEBUG
        return spdlog::level::trace;
#endif // NDEBUG
    }

    template <spdlog::level::level_enum Level>
    void
    log_to(spdlog::logger *logger,
           std::string_view message) noexcept
    {
#ifdef NDEBUG
        if constexpr (is_debug_level(Level))
        {
            (void) logger;
            (void) message;
        }
        else
#endif // NDEBUG
        {
            logger->log(Level, message);
        }
    }

    template <spdlog::level::level_enum Level>
    void
    log_default(std::string_view message) noexcept
    {
#ifdef NDEBUG
        if constexpr (is_debug_level(Level))
        {
            (void) message;
        }
        else
#endif // NDEBUG
        {
            spdlog::log(Level, message);
        }
    }
}

struct st::io::log::logger::logger_impl
{
    logger_impl () noexcept = default;
    ~logger_impl() noexcept = default;

    std::shared_ptr<spdlog::logger> logger;

    ST_INLINE static std::string sink_file_log_path()
    {
        std::filesystem::create_directory("log");
        std::array<char, 32> buffer{};
        const std::time_t now = std::time(nullptr);
        const std::tm *local = std::localtime(&now);
        std::strftime(
            buffer.data(),
            buffer.size(),
            "%Y-%m-%d_%H-%M-%S",
            local
        );
        return (std::filesystem::path("log") / (std::string{buffer.data()} + ".log")).string();
    }

    static std::vector<spdlog::sink_ptr> sinks;
};

std::vector<spdlog::sink_ptr> st::io::log::logger::logger_impl::sinks = {
    std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
    std::make_shared<spdlog::sinks::basic_file_sink_mt>(sink_file_log_path(), false)
};

st::io::log::logger::logger(std::string_view name) noexcept
    : m_impl{std::make_unique<logger_impl>()}
{
    m_impl->logger = std::make_shared<spdlog::logger>(
        std::string(name),
        logger_impl::sinks.begin(),
        logger_impl::sinks.end()
    );
    m_impl->logger->set_level(default_log_level());
    spdlog::register_logger(m_impl->logger);
}

st::io::log::logger::~logger() noexcept
{
    spdlog::drop(m_impl->logger->name());
}

void
st::io::log::logger::set_default() noexcept
{
    spdlog::set_default_logger(m_impl->logger);
}

void st::io::log::trace(std::string_view m) noexcept { log_default<spdlog::level::trace>(m); }
void st::io::log::debug(std::string_view m) noexcept { log_default<spdlog::level::debug>(m); }
void st::io::log::info (std::string_view m) noexcept { log_default<spdlog::level::info >(m); }
void st::io::log::warn (std::string_view m) noexcept { log_default<spdlog::level::warn >(m); }
void st::io::log::error(std::string_view m) noexcept { log_default<spdlog::level::err  >(m); }

void st::io::log::logger::trace(std::string_view m) noexcept { log_to<spdlog::level::trace>(m_impl->logger.get(), m); }
void st::io::log::logger::debug(std::string_view m) noexcept { log_to<spdlog::level::debug>(m_impl->logger.get(), m); }
void st::io::log::logger::info (std::string_view m) noexcept { log_to<spdlog::level::info >(m_impl->logger.get(), m); }
void st::io::log::logger::warn (std::string_view m) noexcept { log_to<spdlog::level::warn >(m_impl->logger.get(), m); }
void st::io::log::logger::error(std::string_view m) noexcept { log_to<spdlog::level::err  >(m_impl->logger.get(), m); }
