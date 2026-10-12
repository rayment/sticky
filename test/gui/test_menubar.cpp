/*
 * test_menubar.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#include <memory>

#include <doctest/doctest.h>

#include "st_types.h"

#include "gui/frame.h"
#include "gui/menubar.h"

TEST_SUITE("gui::menubar")
{
    TEST_CASE("menus, buttons, checkboxes and separators can be nested")
    {
        st::gui::menubar        bar;
        st::gui::menu           file;
        st::gui::menu           recent;
        st::gui::menu_button    open;
        st::gui::menu_checkbox  grid;
        st::gui::menu_separator separator;

        file.set_parent(&bar);
        recent.set_parent(&file);
        open.set_parent(&file);
        separator.set_parent(&file);
        grid.set_parent(&recent);

        CHECK_EQ(&bar, file.parent());
        CHECK_EQ(&file, recent.parent());
        CHECK_EQ(3U, file.children().size());
        CHECK_EQ(&recent, grid.parent());
    }

    TEST_CASE("a menubar is a child of a frame")
    {
        st::gui::frame   frame;
        st::gui::menubar bar;

        bar.set_parent(&frame);

        CHECK_EQ(&frame, bar.parent());
        CHECK(bar.is_menubar());
    }

    TEST_CASE("only a menubar reports itself as one")
    {
        st::gui::menu        menu;
        st::gui::menu_button button;
        st::gui::frame       frame;

        CHECK_FALSE(menu.is_menubar());
        CHECK_FALSE(button.is_menubar());
        CHECK_FALSE(frame.is_menubar());
    }

    TEST_CASE("closing the frame closes its menubar")
    {
        st::gui::frame   frame;
        st::gui::menubar bar;
        bar.set_parent(&frame);

        frame.close();

        CHECK(bar.is_closed());
    }

    TEST_CASE("closing the menubar closes everything in it")
    {
        st::gui::menubar       bar;
        st::gui::menu          file;
        st::gui::menu_button   open;
        st::gui::menu_checkbox grid;
        file.set_parent(&bar);
        open.set_parent(&file);
        grid.set_parent(&file);

        bar.close();

        CHECK(file.is_closed());
        CHECK(open.is_closed());
        CHECK(grid.is_closed());
    }
}

TEST_SUITE("gui::menu_checkbox")
{
    TEST_CASE("starts unchecked")
    {
        st::gui::menu_checkbox checkbox;
        CHECK_FALSE(checkbox.is_checked());
    }

    TEST_CASE("set_checked stores the state without calling callbacks")
    {
        st::gui::menu_checkbox checkbox;
        st::int32              calls = 0;
        checkbox.register_on_toggle([&calls](st::bool_t) { ++calls; });

        checkbox.set_checked(true);

        CHECK(checkbox.is_checked());
        CHECK_EQ(0, calls);
    }

    TEST_CASE("callbacks are released when the checkbox closes")
    {
        const auto             token = std::make_shared<int>(0);
        st::gui::menu_checkbox checkbox;
        checkbox.register_on_toggle([token](st::bool_t) {});
        CHECK_EQ(2, token.use_count());

        checkbox.close();

        CHECK_EQ(1, token.use_count());
    }

    TEST_CASE("clear_on_toggle releases callbacks")
    {
        const auto             token = std::make_shared<int>(0);
        st::gui::menu_checkbox checkbox;
        checkbox.register_on_toggle([token](st::bool_t) {});

        checkbox.clear_on_toggle();

        CHECK_EQ(1, token.use_count());
    }
}

TEST_SUITE("gui::menu_button")
{
    TEST_CASE("callbacks are released when the button closes")
    {
        const auto           token = std::make_shared<int>(0);
        st::gui::menu_button button;
        button.register_on_click([token] {});
        CHECK_EQ(2, token.use_count());

        button.close();

        CHECK_EQ(1, token.use_count());
    }

    TEST_CASE("clear_on_click releases callbacks")
    {
        const auto           token = std::make_shared<int>(0);
        st::gui::menu_button button;
        button.register_on_click([token] {});

        button.clear_on_click();

        CHECK_EQ(1, token.use_count());
    }
}
