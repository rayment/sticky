/*
 * test_mod_phases.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <filesystem>

#include <doctest/doctest.h>

#include "headless_session.h"
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
        st::test::headless_session &session = st::test::headless_session::instance();
        st::video::window           window;
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

    CHECK(mod.execute_runtime());
    CHECK(mod.execute_function("check_init_locked"));
    CHECK(mod.execute_init());
    CHECK(mod.execute_function("check_init_locked"));
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
                  "st.init.sounds is locked outside of the init phase")
{
    st::mod mod = make_mod("0016-mod-sounds");

    CHECK(mod.execute_runtime());
    CHECK(mod.execute_function("check_init_locked"));
    CHECK(mod.execute_init());
    CHECK(mod.execute_function("check_init_locked"));
}
#endif // ST_MODULE_AUDIO
