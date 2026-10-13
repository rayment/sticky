/*
 * test_textedit.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#include <doctest/doctest.h>

#include "gui/frame.h"
#include "gui/textedit.h"

TEST_SUITE("gui::textedit")
{
    TEST_CASE("a new editor is empty, enabled, editable, unmasked and wraps with a vertical scrollbar")
    {
        st::gui::textedit edit;

        CHECK(edit.text().empty());
        CHECK(edit.is_enabled());
        CHECK_FALSE(edit.is_read_only());
        CHECK_FALSE(edit.is_masked());
        CHECK(edit.has_vertical_scrollbar());
        CHECK(edit.is_word_wrapped());
    }

    TEST_CASE("set_text keeps new lines")
    {
        st::gui::textedit edit;
        edit.set_text("one\ntwo");
        CHECK_EQ("one\ntwo", edit.text());
    }

    TEST_CASE("the flags are independent")
    {
        st::gui::textedit edit;

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

    TEST_CASE("the scrollbars are independent")
    {
        st::gui::textedit edit;

        edit.set_vertical_scrollbar(false);
        CHECK_FALSE(edit.has_vertical_scrollbar());
        CHECK(edit.is_word_wrapped());

        edit.set_word_wrap(false);
        CHECK_FALSE(edit.has_vertical_scrollbar());
        CHECK_FALSE(edit.is_word_wrapped());
    }

    TEST_CASE("an editor is a child of a frame")
    {
        st::gui::frame    frame;
        st::gui::textedit edit;

        edit.set_parent(&frame);

        CHECK_EQ(&frame, edit.parent());
    }

    TEST_CASE("closing the frame closes the editor")
    {
        st::gui::frame    frame;
        st::gui::textedit edit;
        edit.set_parent(&frame);

        frame.close();

        CHECK(edit.is_closed());
    }
}
