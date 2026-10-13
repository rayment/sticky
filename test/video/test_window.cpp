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

#include "headless_session.h"
#include "video/window.h"

namespace
{
    // Windows are created hidden, so tests render without showing anything.
    struct window_fixture
    {
        st::test::headless_session &session = st::test::headless_session::instance();
        st::video::window           window;
    };
} // namespace

TEST_SUITE("video::window")
{
    TEST_CASE_FIXTURE(window_fixture, "create provides valid SDL and GL handles")
    {
        CHECK(window.is_valid());
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
        window.set_clear_colour({1.0F, 0.0F, 0.0F});
        window.clear();

        GLubyte pixel[4] = {0, 0, 0, 0};
        glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
        CHECK_EQ(GL_NO_ERROR, glGetError());
        CHECK_EQ(255, pixel[0]);
        CHECK_EQ(0, pixel[1]);
        CHECK_EQ(0, pixel[2]);
    }

    TEST_CASE_FIXTURE(window_fixture, "the fixed timestep is clamped to a usable range")
    {
        window.set_fixed_timestep(0.0);
        CHECK_GT(window.fixed_timestep(), 0.0);
        window.set_fixed_timestep(-5.0);
        CHECK_GT(window.fixed_timestep(), 0.0);
        window.set_fixed_timestep(60.0);
        CHECK_EQ(1.0, window.fixed_timestep());
        window.set_fixed_timestep(0.25);
        CHECK_EQ(0.25, window.fixed_timestep());
    }

    TEST_CASE_FIXTURE(window_fixture, "simulate runs no steps right after a reset")
    {
        window.set_fixed_timestep(0.0);
        window.reset_accumulator();

        st::int32 steps = 0;
        window.simulate([&steps](st::float64) { ++steps; });
        CHECK_EQ(0, steps);
    }

    TEST_CASE_FIXTURE(window_fixture, "a stall is limited to a bounded number of steps")
    {
        window.set_fixed_timestep(0.001);
        window.reset_accumulator();
        SDL_Delay(400);
        window.accumulate();

        st::int32 steps = 0;
        window.simulate([&steps](st::float64) { ++steps; });
        CHECK_LE(steps, 250);
        CHECK_LE(window.delta_time(), window.fixed_timestep());
    }

    TEST_CASE_FIXTURE(window_fixture, "adaptive vsync falls back when unsupported")
    {
        window.set_vsync(st::video::window::Adaptive);
        CHECK_NE(st::video::window::Disabled, window.vsync());
        window.set_vsync(st::video::window::Disabled);
        CHECK_EQ(st::video::window::Disabled, window.vsync());
    }

    TEST_CASE_FIXTURE(window_fixture, "poll and swap run a frame without error")
    {
        window.poll();
        window.clear();
        window.swap();
        CHECK_FALSE(window.close_requested());
    }
}
