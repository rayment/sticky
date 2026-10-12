/*
 * test_log.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <memory>
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

    TEST_CASE("logger destructor unregisters the logger")
    {
        {
            st::io::log::logger scoped{"test_log_scoped"};
            CHECK_NE(nullptr, spdlog::get("test_log_scoped"));
        }

        CHECK_EQ(nullptr, spdlog::get("test_log_scoped"));
    }
}
