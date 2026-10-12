/*
 * menubar.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#ifndef STICKY_GUI_MENUBAR_H
#define STICKY_GUI_MENUBAR_H

#include <functional>
#include <memory>
#include <string>

#include "gui/component.h"

namespace st::gui
{
    // A bar across the top of a frame that holds menus, buttons, checkboxes and separators. It is drawn only when
    // it is a direct child of a frame, and a frame draws one bar.
    class menubar : public nestable_component
    {
    public:
        menubar() noexcept;
        ~menubar() noexcept;

        void render() noexcept override;

        [[nodiscard]] st::bool_t is_menubar() const noexcept override;
    };

    // A named menu that opens when hovered or clicked. It can be nested in a menubar or in another menu.
    class menu : public nestable_component
    {
    public:
        menu() noexcept;
        ~menu() noexcept;

        void render() noexcept override;

        void set_enabled(st::bool_t enabled) noexcept;
        void set_text(const std::string &text) noexcept;

    private:
        struct menu_impl;
        std::unique_ptr<menu_impl> m_impl;
    };

    // An entry that does something when it is clicked.
    class menu_button : public component
    {
    public:
        menu_button() noexcept;
        ~menu_button() noexcept;

        void render() noexcept override;

        void set_enabled(st::bool_t enabled) noexcept;
        void set_text(const std::string &text) noexcept;

        using on_click_callback = std::function<void()>;

        void register_on_click(const on_click_callback &callback) noexcept;

        void clear_on_click() noexcept;

    protected:
        // Drops the click callbacks so that closures referencing the button are released.
        void handle_close() noexcept override;

    private:
        struct menu_button_impl;
        std::unique_ptr<menu_button_impl> m_impl;
    };

    // An entry with a check mark that the user toggles.
    class menu_checkbox : public component
    {
    public:
        menu_checkbox() noexcept;
        ~menu_checkbox() noexcept;

        void render() noexcept override;

        void set_checked(st::bool_t checked) noexcept;
        void set_enabled(st::bool_t enabled) noexcept;
        void set_text(const std::string &text) noexcept;

        [[nodiscard]] st::bool_t is_checked() const noexcept;

        // Called with the new state when the user toggles the checkbox. set_checked() does not call it.
        using on_toggle_callback = std::function<void(st::bool_t)>;

        void register_on_toggle(const on_toggle_callback &callback) noexcept;

        void clear_on_toggle() noexcept;

    protected:
        // Drops the toggle callbacks so that closures referencing the checkbox are released.
        void handle_close() noexcept override;

    private:
        struct menu_checkbox_impl;
        std::unique_ptr<menu_checkbox_impl> m_impl;
    };

    // A line that groups the entries of a menu.
    class menu_separator : public component
    {
    public:
        menu_separator() noexcept;
        ~menu_separator() noexcept;

        void render() noexcept override;
    };
} // namespace st::gui

#endif // STICKY_GUI_MENUBAR_H
