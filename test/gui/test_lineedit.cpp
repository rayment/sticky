/*
 * test_lineedit.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#include <doctest/doctest.h>

#include "gui/frame.h"
#include "gui/lineedit.h"

TEST_SUITE("gui::lineedit")
{
    TEST_CASE("a new editor is empty, enabled, editable and unmasked")
    {
        st::gui::lineedit edit;

        CHECK(edit.text().empty());
        CHECK(edit.is_enabled());
        CHECK_FALSE(edit.is_read_only());
        CHECK_FALSE(edit.is_masked());
    }

    TEST_CASE("set_text replaces the text")
    {
        st::gui::lineedit edit;
        edit.set_text("hello");
        CHECK_EQ("hello", edit.text());
        edit.set_text("");
        CHECK(edit.text().empty());
    }

    TEST_CASE("the flags are independent")
    {
        st::gui::lineedit edit;

        edit.set_enabled(false);
        CHECK_FALSE(edit.is_enabled());
        CHECK_FALSE(edit.is_read_only());
        CHECK_FALSE(edit.is_masked());

        edit.set_read_only(true);
        CHECK_FALSE(edit.is_enabled());
        CHECK(edit.is_read_only());
        CHECK_FALSE(edit.is_masked());

        edit.set_masked(true);
        CHECK(edit.is_masked());
    }

    TEST_CASE("an editor is a child of a frame")
    {
        st::gui::frame    frame;
        st::gui::lineedit edit;

        edit.set_parent(&frame);

        CHECK_EQ(&frame, edit.parent());
    }

    TEST_CASE("closing the frame closes the editor")
    {
        st::gui::frame    frame;
        st::gui::lineedit edit;
        edit.set_parent(&frame);

        frame.close();

        CHECK(edit.is_closed());
    }
}
