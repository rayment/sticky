/*
 * test_popup.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <doctest/doctest.h>

#include "st_types.h"

#include "gui/popup.h"

TEST_SUITE("gui::popup")
{
    TEST_CASE("close invokes registered callbacks")
    {
        st::gui::popup popup;
        st::int32      calls = 0;
        popup.register_on_close([&calls] { ++calls; });
        popup.close();
        CHECK_EQ(1, calls);
    }

    TEST_CASE("clear_on_close removes callbacks")
    {
        st::gui::popup popup;
        st::int32      calls = 0;
        popup.register_on_close([&calls] { ++calls; });
        popup.clear_on_close();
        popup.close();
        CHECK_EQ(0, calls);
    }
}
