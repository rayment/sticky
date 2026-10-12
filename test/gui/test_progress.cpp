/*
 * test_progress.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <doctest/doctest.h>

#include "gui/progress.h"

TEST_SUITE("gui::progress")
{
    TEST_CASE("value starts at zero")
    {
        st::gui::progress progress;
        CHECK_EQ(0.0f, progress.value());
    }

    TEST_CASE("set_value stores values within range")
    {
        st::gui::progress progress;
        progress.set_value(0.25f);
        CHECK_EQ(0.25f, progress.value());
    }

    TEST_CASE("set_value clamps values outside of range")
    {
        st::gui::progress progress;
        progress.set_value(2.0f);
        CHECK_EQ(1.0f, progress.value());
        progress.set_value(-1.0f);
        CHECK_EQ(0.0f, progress.value());
    }
}
