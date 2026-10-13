/*
 * log.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <array>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <system_error>
#include <vector>

#include <fmt/format.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "io/filesystem.h"
#include "io/log.h"
#include "st_macros.h"

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
    constexpr st::bool_t is_debug_level(spdlog::level::level_enum lvl) noexcept
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

    constexpr std::chrono::seconds overlay_lifetime{10};

    class overlay_sink final : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        overlay_sink() noexcept { set_level(spdlog::level::warn); }

        std::vector<st::io::log::overlay_line> lines() noexcept
        {
            const auto                             now = std::chrono::steady_clock::now();
            std::vector<st::io::log::overlay_line> result(m_slots.size());
            std::lock_guard<std::mutex>            lock(mutex_);
            for (std::size_t i = 0; i < m_slots.size(); ++i)
            {
                if (now - m_slots[i].time < overlay_lifetime)
                    result[i] = m_slots[i].line;
            }
            return result;
        }

    protected:
        void sink_it_(const spdlog::details::log_msg &msg) override
        {
            slot &s = m_slots[m_next];
            s.line.is_error = msg.level >= spdlog::level::err;
            s.line.text.assign(msg.payload.data(), msg.payload.size());
            s.time = std::chrono::steady_clock::now();
            m_next = (m_next + 1) % m_slots.size();
        }

        void flush_() override {}

    private:
        struct slot
        {
            st::io::log::overlay_line             line;
            std::chrono::steady_clock::time_point time;
        };

        std::array<slot, st::io::log::overlay_capacity> m_slots;
        std::size_t                                     m_next = 0;
    };

    const std::shared_ptr<overlay_sink> &overlay() noexcept
    {
        static const auto instance = std::make_shared<overlay_sink>();
        return instance;
    }

    template <spdlog::level::level_enum Level>
    void log_to(spdlog::logger  *logger,
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
    void log_default(std::string_view message) noexcept
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
} // namespace

namespace
{
    // The file is opened up front, because spdlog aborts when it cannot open it and exceptions are disabled.
    [[nodiscard]] spdlog::sink_ptr make_file_sink() noexcept
    {
        const std::filesystem::path dir = st::io::filesystem::executable_dir() / "log";
        std::error_code             ec;
        std::filesystem::create_directories(dir, ec);

        std::array<char, 32> buffer{};
        const std::time_t    now = std::time(nullptr);
        const std::tm       *local = std::localtime(&now);
        if (local == nullptr || std::strftime(buffer.data(), buffer.size(), "%Y-%m-%d_%H-%M-%S", local) == 0)
            return nullptr;

        const std::filesystem::path path = dir / (std::string(buffer.data()) + ".log");
        if (!std::ofstream(path, std::ios::app).is_open())
        {
            std::fprintf(stderr, "Cannot open log file '%s'. Logging to the console only.\n", path.u8string().c_str());
            return nullptr;
        }
        return std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.u8string(), false);
    }

    // Created on first use rather than during static initialisation of the library.
    const std::vector<spdlog::sink_ptr> &sinks() noexcept
    {
        static const std::vector<spdlog::sink_ptr> instance = []
        {
            std::vector<spdlog::sink_ptr> result{std::make_shared<spdlog::sinks::stdout_color_sink_mt>(), overlay()};
            if (auto file = make_file_sink())
                result.push_back(std::move(file));
            return result;
        }();
        return instance;
    }
} // namespace

struct st::io::log::logger::logger_impl
{
    std::shared_ptr<spdlog::logger> logger;
    st::bool_t                      registered = false;
};

std::vector<st::io::log::overlay_line>
st::io::log::overlay_lines() noexcept
{
    return overlay()->lines();
}

st::io::log::logger::logger(std::string_view name) noexcept : m_impl{std::make_unique<logger_impl>()}
{
    const auto &all_sinks = sinks();
    m_impl->logger = std::make_shared<spdlog::logger>(std::string(name), all_sinks.begin(), all_sinks.end());
    m_impl->logger->set_level(default_log_level());

    // spdlog refuses a second logger of the same name, which mods must not be able to cause.
    if (spdlog::get(m_impl->logger->name()) == nullptr)
    {
        spdlog::register_logger(m_impl->logger);
        m_impl->registered = true;
    }
}

st::io::log::logger::~logger() noexcept
{
    // The default logger stays registered, so that late messages still have somewhere to go.
    if (m_impl->registered && spdlog::default_logger_raw() != m_impl->logger.get())
        spdlog::drop(m_impl->logger->name());
}

void
st::io::log::logger::set_default() noexcept
{
    spdlog::set_default_logger(m_impl->logger);
}

void
st::io::log::trace(std::string_view m) noexcept
{
    log_default<spdlog::level::trace>(m);
}
void
st::io::log::debug(std::string_view m) noexcept
{
    log_default<spdlog::level::debug>(m);
}
void
st::io::log::info(std::string_view m) noexcept
{
    log_default<spdlog::level::info>(m);
}
void
st::io::log::warn(std::string_view m) noexcept
{
    log_default<spdlog::level::warn>(m);
}
void
st::io::log::error(std::string_view m) noexcept
{
    log_default<spdlog::level::err>(m);
}

void
st::io::log::logger::trace(std::string_view m) noexcept
{
    log_to<spdlog::level::trace>(m_impl->logger.get(), m);
}
void
st::io::log::logger::debug(std::string_view m) noexcept
{
    log_to<spdlog::level::debug>(m_impl->logger.get(), m);
}
void
st::io::log::logger::info(std::string_view m) noexcept
{
    log_to<spdlog::level::info>(m_impl->logger.get(), m);
}
void
st::io::log::logger::warn(std::string_view m) noexcept
{
    log_to<spdlog::level::warn>(m_impl->logger.get(), m);
}
void
st::io::log::logger::error(std::string_view m) noexcept
{
    log_to<spdlog::level::err>(m_impl->logger.get(), m);
}
