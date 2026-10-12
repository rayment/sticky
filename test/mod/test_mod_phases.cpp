/*
 * test_mod_phases.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <filesystem>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>

#include <doctest/doctest.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/spdlog.h>

#include "headless_session.h"
#include "io/log.h"
#include "mod.h"
#include "video/window.h"

namespace
{
    namespace fs = std::filesystem;

    const fs::path res_dir = fs::path(ST_TEST_RES_DIR);

    st::mod make_mod(const char *folder)
    {
        return st::mod{folder, folder, "0.0.0", "test", "test", res_dir / folder / "lua"};
    }

    // Textures need a current OpenGL context, which the window provides.
    // The window is a fixture member so it outlives the mod's textures.
    struct phase_fixture
    {
        st::test::headless_session     &session = st::test::headless_session::instance();
        st::video::window               window;
        std::ostringstream              output;
        std::shared_ptr<spdlog::logger> previous_default = spdlog::default_logger();
        st::io::log::logger             default_logger{"test_mod_phases"};

        phase_fixture()
        {
            default_logger.set_default();
            spdlog::get("test_mod_phases")->sinks().push_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(output));
        }

        ~phase_fixture() { spdlog::set_default_logger(previous_default); }

        ST_DELETE_COPY_CTORS(phase_fixture);
        ST_DELETE_MOVE_CTORS(phase_fixture);

        [[nodiscard]] st::bool_t contains(std::string_view text) const
        {
            return output.str().find(text) != std::string::npos;
        }

        // Runs a function that must raise `message`, which separates a rejected call from a missing function.
        void check_function_fails(st::mod          &mod,
                                  const st::char_t *name,
                                  std::string_view  message)
        {
            INFO(name);
            output.str("");

            CHECK_FALSE(mod.execute_function(name));
            CHECK(contains(message));
        }
    };
} // namespace

TEST_CASE_FIXTURE(phase_fixture,
                  "execute_init loads declared textures for the runtime phase")
{
    st::mod mod = make_mod("0013-mod-textures");

    CHECK(mod.execute_init());
    CHECK(mod.execute_runtime());
    CHECK(mod.execute_function("check_textures"));
}

TEST_CASE_FIXTURE(phase_fixture,
                  "st.init is locked outside of the init phase")
{
    st::mod mod = make_mod("0013-mod-textures");

    constexpr std::string_view message = "only available during the init phase";

    CHECK(mod.execute_runtime());
    check_function_fails(mod, "read_init_textures", message);
    check_function_fails(mod, "write_init_textures", message);
    CHECK(mod.execute_init());
    check_function_fails(mod, "read_init_textures", message);
    check_function_fails(mod, "write_init_textures", message);
}

TEST_CASE_FIXTURE(phase_fixture,
                  "st.io.log is available in the init and runtime phases")
{
    st::mod mod = make_mod("0013-mod-textures");
    spdlog::get("0013-mod-textures")->sinks().push_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(output));

    CHECK(mod.execute_init());
    CHECK(contains("[info] log from init phase"));

    CHECK(mod.execute_runtime());
    CHECK(mod.execute_function("log_from_runtime"));
    CHECK(contains("[info] log from runtime phase"));
}

TEST_CASE_FIXTURE(phase_fixture,
                  "execute_init fails when init.lua is broken")
{
    st::mod mod = make_mod("0014-mod-bad-init");

    CHECK_FALSE(mod.execute_init());
}

TEST_CASE_FIXTURE(phase_fixture,
                  "execute_init fails when init.lua is missing")
{
    st::mod mod = make_mod("0015-mod-no-init");

    CHECK_FALSE(mod.execute_init());
    CHECK(mod.execute_runtime());
}

TEST_CASE_FIXTURE(phase_fixture,
                  "register_config adds declared settings without overwriting existing ones")
{
    st::mod          mod = make_mod("0017-mod-config");
    st::util::config config;
    config.set("0017-mod-config.seed", 99.0);

    CHECK(mod.execute_init());
    mod.register_config(config);

    CHECK(std::get<st::bool_t>(*config.get("0017-mod-config.fullscreen")) == false);
    CHECK(std::get<std::string>(*config.get("0017-mod-config.player")) == "John Smith");
    CHECK(std::get<st::float64>(*config.get("0017-mod-config.seed")) == 99.0);
    CHECK_FALSE(config.get("0017-mod-config.wrong").has_value());
    CHECK_FALSE(config.get("0017-mod-config.unknown").has_value());
}

TEST_CASE_FIXTURE(phase_fixture,
                  "register_config does not double the fullstop of a mod name that ends with one")
{
    st::mod          mod{"core.", "core", "0.0.0", "test", "test", res_dir / "0017-mod-config" / "lua"};
    st::util::config config;

    CHECK(mod.execute_init());
    mod.register_config(config);

    CHECK(config.get("core.fullscreen").has_value());
    CHECK_FALSE(config.get("core..fullscreen").has_value());
}

TEST_CASE_FIXTURE(phase_fixture,
                  "register_config adds nothing when init fails")
{
    st::mod          mod = make_mod("0014-mod-bad-init");
    st::util::config config;

    CHECK_FALSE(mod.execute_init());
    mod.register_config(config);

    CHECK_FALSE(config.get("0014-mod-bad-init.anything").has_value());
}

#if ST_MODULE_AUDIO
TEST_CASE_FIXTURE(phase_fixture,
                  "execute_init loads declared sounds for the runtime phase")
{
    st::mod mod = make_mod("0016-mod-sounds");

    CHECK(mod.execute_init());
    CHECK(mod.execute_runtime());
    CHECK(mod.execute_function("check_sounds"));
}

TEST_CASE_FIXTURE(phase_fixture,
                  "audio sources can be created and driven from Lua")
{
    st::mod mod = make_mod("0016-mod-sounds");

    CHECK(mod.execute_init());
    CHECK(mod.execute_runtime());
    CHECK(mod.execute_function("check_source"));
}

TEST_CASE_FIXTURE(phase_fixture,
                  "st.init.sounds is locked outside of the init phase")
{
    st::mod mod = make_mod("0016-mod-sounds");

    constexpr std::string_view message = "only available during the init phase";

    CHECK(mod.execute_runtime());
    check_function_fails(mod, "read_init_sounds", message);
    check_function_fails(mod, "write_init_sounds", message);
    CHECK(mod.execute_init());
    check_function_fails(mod, "read_init_sounds", message);
    check_function_fails(mod, "write_init_sounds", message);
}

TEST_CASE_FIXTURE(phase_fixture,
                  "audio sources reject invalid arguments from Lua")
{
    st::mod mod = make_mod("0016-mod-sounds");

    CHECK(mod.execute_init());
    CHECK(mod.execute_runtime());
    check_function_fails(mod, "select_nil_sound", "select");
    check_function_fails(mod, "set_position_without_components", "set_position");
}
#endif // ST_MODULE_AUDIO
