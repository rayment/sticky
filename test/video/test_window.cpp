/*
 * test_window.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <cstring>

#include <GL/glew.h>
#include <SDL3/SDL.h>
#include <doctest/doctest.h>
#include <glm/vec3.hpp>

#include "st_init.h"
#include "st_macros.h"
#include "video/window.h"

namespace
{
    // Windows are created hidden, so tests render without showing anything.
    // OpenAL's null backend avoids needing an audio device.
    // The library can only be initialized once per process.
    struct headless_session
    {
        static headless_session &instance()
        {
            static headless_session session;
            return session;
        }

        headless_session()
        {
            SDL_setenv_unsafe("ALSOFT_DRIVERS", "null", 1);
            CHECK(st::lib_init(false));
        }

        ~headless_session() { static_cast<void>(st::lib_free()); }

        ST_DELETE_COPY_CTORS(headless_session);
        ST_DELETE_MOVE_CTORS(headless_session);
    };

    struct window_fixture
    {
        headless_session &session = headless_session::instance();
        st::video::window window;
    };
} // namespace

TEST_SUITE("video::window")
{
    TEST_CASE_FIXTURE(window_fixture, "create provides valid SDL and GL handles")
    {
        CHECK_NE(nullptr, window.sdl_ptr());
        CHECK_NE(nullptr, window.gl_ptr());
    }

    TEST_CASE_FIXTURE(window_fixture, "create initializes default state")
    {
        CHECK_FALSE(window.visible());
        CHECK_FALSE(window.close_requested());
        CHECK_EQ(st::video::window::Windowed, window.fullscreen());
        CHECK_EQ(st::video::window::Disabled, window.vsync());
    }

    TEST_CASE_FIXTURE(window_fixture, "show and hide toggle visibility")
    {
        window.show();
        CHECK(window.visible());
        window.hide();
        CHECK_FALSE(window.visible());
    }

    TEST_CASE_FIXTURE(window_fixture, "set_title updates the window title")
    {
        window.set_title("headless test");
        CHECK_EQ("headless test", window.title());
        CHECK_EQ(0, std::strcmp("headless test", SDL_GetWindowTitle(static_cast<SDL_Window *>(window.sdl_ptr()))));
    }

    TEST_CASE_FIXTURE(window_fixture, "close requests closing")
    {
        window.close();
        CHECK(window.close_requested());
    }

    TEST_CASE_FIXTURE(window_fixture, "clear renders the clear color")
    {
        window.set_clear_color({1.0F, 0.0F, 0.0F});
        window.clear();

        GLubyte pixel[4] = {0, 0, 0, 0};
        glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
        CHECK_EQ(GL_NO_ERROR, glGetError());
        CHECK_EQ(255, pixel[0]);
        CHECK_EQ(0, pixel[1]);
        CHECK_EQ(0, pixel[2]);
    }

    TEST_CASE_FIXTURE(window_fixture, "poll and swap run a frame without error")
    {
        window.poll();
        window.clear();
        window.swap();
        CHECK_FALSE(window.close_requested());
    }
}
