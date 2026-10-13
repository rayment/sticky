/*
 * log.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

/**
 * @file
 * @brief Engine logging to the console, a log file and the on-screen overlay.
 */

#ifndef STICKY_IO_LOG_H
#define STICKY_IO_LOG_H

#include "st_macros.h"
#include "st_types.h"

/** @addtogroup io
 *  @{
 */

#ifdef __cplusplus
#include <memory>
#include <string>
#include <string_view>
#include <vector>

/**
 * @brief Logging through the default logger and through named loggers.
 *
 * Every logger writes to the console, to a timestamped file in the `log` folder of the working directory and, for
 * warnings and errors, to the overlay drawn by the GUI. The file is skipped, with a message on the console, if it
 * cannot be created. Trace and debug messages are compiled out of release builds.
 */
namespace st::io::log
{
    /** @brief Logs a trace message to the default logger. @param message Text of the message. */
    void trace(std::string_view message) noexcept;
    /** @brief Logs a debug message to the default logger. @param message Text of the message. */
    void debug(std::string_view message) noexcept;
    /** @brief Logs an information message to the default logger. @param message Text of the message. */
    void info(std::string_view message) noexcept;
    /** @brief Logs a warning to the default logger. @param message Text of the message. */
    void warn(std::string_view message) noexcept;
    /** @brief Logs an error to the default logger. @param message Text of the message. */
    void error(std::string_view message) noexcept;

    /** @brief Number of lines kept by the overlay. */
    constexpr std::size_t overlay_capacity = 10;

    /** @brief One line of the overlay. */
    struct overlay_line
    {
        st::bool_t  is_error = false; ///< Whether the line is an error rather than a warning.
        std::string text; ///< Text of the line, empty when the slot is unused.
    };

    /**
     * @brief Returns the recent warnings and errors for the overlay.
     * @return #overlay_capacity slots, oldest first. Unused slots and lines older than ten seconds have empty text.
     */
    [[nodiscard]] std::vector<overlay_line> overlay_lines() noexcept;

    /**
     * @brief A named logger, such as the one given to each mod.
     *
     * The name prefixes every message. Several loggers may share a name.
     */
    class logger
    {
    public:
        /** @brief Creates a logger. @param name Name shown with every message. */
        explicit logger(std::string_view name) noexcept;
        ~logger() noexcept;

        ST_DELETE_COPY_CTORS(logger);
        ST_DELETE_MOVE_CTORS(logger);

        /** @brief Logs a trace message. @param message Text of the message. */
        void trace(std::string_view message) noexcept;
        /** @brief Logs a debug message. @param message Text of the message. */
        void debug(std::string_view message) noexcept;
        /** @brief Logs an information message. @param message Text of the message. */
        void info(std::string_view message) noexcept;
        /** @brief Logs a warning. @param message Text of the message. */
        void warn(std::string_view message) noexcept;
        /** @brief Logs an error. @param message Text of the message. */
        void error(std::string_view message) noexcept;

        /** @brief Makes this logger the one used by the free functions of st::io::log. */
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

/** @brief C binding of st::io::log::trace(). Does nothing in release builds. */
void st_io_log_trace(const st_char *message);
/** @brief C binding of st::io::log::debug(). Does nothing in release builds. */
void st_io_log_debug(const st_char *message);
/** @brief C binding of st::io::log::info(). */
void st_io_log_info(const st_char *message);
/** @brief C binding of st::io::log::warn(). */
void st_io_log_warn(const st_char *message);
/** @brief C binding of st::io::log::error(). */
void st_io_log_error(const st_char *message);

#ifdef __cplusplus
}
#endif

/** @} */

#endif // STICKY_IO_LOG_H
