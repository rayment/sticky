/*
 * test_log.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <memory>
#include <optional>
#include <sstream>
#include <string>

#include <doctest/doctest.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

#include "io/log.h"

namespace
{
    constexpr bool debug_levels_enabled =
#ifdef NDEBUG
        false;
#else
        true;
#endif

    std::optional<st::io::log::overlay_line> overlay_find(const std::string &text)
    {
        for (const auto &line : st::io::log::overlay_lines())
        {
            if (line.text == text)
                return line;
        }
        return std::nullopt;
    }

    // Captures everything a logger writes in an in-memory buffer and restores
    // the previous default logger on destruction.
    struct log_fixture
    {
        std::ostringstream              output;
        std::shared_ptr<spdlog::logger> previous_default = spdlog::default_logger();
        st::io::log::logger             logger{"test_log"};

        log_fixture()
        {
            auto sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(output);
            spdlog::get("test_log")->sinks().push_back(sink);
        }

        ~log_fixture() { spdlog::set_default_logger(previous_default); }

        ST_DELETE_COPY_CTORS(log_fixture);
        ST_DELETE_MOVE_CTORS(log_fixture);

        [[nodiscard]] bool contains(const std::string &text) const
        {
            return output.str().find(text) != std::string::npos;
        }
    };
} // namespace

TEST_SUITE("io::log")
{
    TEST_CASE_FIXTURE(log_fixture, "logger writes info warn and error")
    {
        logger.info("info message");
        logger.warn("warn message");
        logger.error("error message");

        CHECK(contains("[info] info message"));
        CHECK(contains("[warning] warn message"));
        CHECK(contains("[error] error message"));
    }

    TEST_CASE_FIXTURE(log_fixture, "logger trace and debug follow build type")
    {
        logger.trace("trace message");
        logger.debug("debug message");

        CHECK_EQ(debug_levels_enabled, contains("[trace] trace message"));
        CHECK_EQ(debug_levels_enabled, contains("[debug] debug message"));
    }

    TEST_CASE_FIXTURE(log_fixture, "logger includes its name")
    {
        logger.info("named");

        CHECK(contains("[test_log]"));
    }

    TEST_CASE_FIXTURE(log_fixture, "free functions use the default logger")
    {
        logger.set_default();

        st::io::log::trace("free trace");
        st::io::log::debug("free debug");
        st::io::log::info("free info");
        st::io::log::warn("free warn");
        st::io::log::error("free error");

        CHECK_EQ(debug_levels_enabled, contains("[trace] free trace"));
        CHECK_EQ(debug_levels_enabled, contains("[debug] free debug"));
        CHECK(contains("[info] free info"));
        CHECK(contains("[warning] free warn"));
        CHECK(contains("[error] free error"));
    }

    TEST_CASE_FIXTURE(log_fixture, "C functions forward to the default logger")
    {
        logger.set_default();

#ifndef NDEBUG
        st_io_log_trace("c trace");
        st_io_log_debug("c debug");
        CHECK(contains("[trace] c trace"));
        CHECK(contains("[debug] c debug"));
#endif
        st_io_log_info("c info");
        st_io_log_warn("c warn");
        st_io_log_error("c error");

        CHECK(contains("[info] c info"));
        CHECK(contains("[warning] c warn"));
        CHECK(contains("[error] c error"));
    }

    TEST_CASE_FIXTURE(log_fixture, "set_default changes the free function target")
    {
        std::ostringstream other_output;
        {
            st::io::log::logger other{"test_log_other"};
            spdlog::get("test_log_other")
                ->sinks()
                .push_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(other_output));

            other.set_default();
            st::io::log::info("to other");
            logger.set_default();
            st::io::log::info("to fixture");
        }

        CHECK_NE(std::string::npos, other_output.str().find("to other"));
        CHECK_EQ(std::string::npos, other_output.str().find("to fixture"));
        CHECK(contains("to fixture"));
        CHECK_FALSE(contains("to other"));
    }

    TEST_CASE_FIXTURE(log_fixture, "overlay captures warnings and errors only")
    {
        logger.info("overlay info");
        logger.warn("overlay warn");
        logger.error("overlay error");

        const auto lines = st::io::log::overlay_lines();
        CHECK_EQ(st::io::log::overlay_capacity, lines.size());
        CHECK_FALSE(overlay_find("overlay info"));
        const auto warn = overlay_find("overlay warn");
        const auto error = overlay_find("overlay error");
        CHECK(warn.has_value());
        CHECK(error.has_value());
        CHECK_FALSE(warn.value_or(st::io::log::overlay_line{true, {}}).is_error);
        CHECK(error.value_or(st::io::log::overlay_line{}).is_error);
    }

    TEST_CASE_FIXTURE(log_fixture, "overlay wraps around after reaching capacity")
    {
        for (std::size_t i = 0; i < st::io::log::overlay_capacity; ++i)
            logger.warn("wrap " + std::to_string(i));

        CHECK(overlay_find("wrap 0"));

        logger.warn("wrap extra");

        CHECK_FALSE(overlay_find("wrap 0"));
        CHECK(overlay_find("wrap 1"));
        CHECK(overlay_find("wrap extra"));
    }

    TEST_CASE("logger destructor unregisters the logger")
    {
        {
            st::io::log::logger scoped{"test_log_scoped"};
            CHECK_NE(nullptr, spdlog::get("test_log_scoped"));
        }

        CHECK_EQ(nullptr, spdlog::get("test_log_scoped"));
    }

    TEST_CASE("loggers may share a name")
    {
        {
            st::io::log::logger first{"test_log_shared"};
            {
                st::io::log::logger second{"test_log_shared"};
                second.info("from the second logger");
            }
            CHECK_NE(nullptr, spdlog::get("test_log_shared"));
            first.info("from the first logger");
        }
        CHECK_EQ(nullptr, spdlog::get("test_log_shared"));
    }
}
