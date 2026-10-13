/*
 * menubar.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

/**
 * @file
 * @brief Menu bars, menus and their entries.
 */

#ifndef STICKY_GUI_MENUBAR_H
#define STICKY_GUI_MENUBAR_H

#include <functional>
#include <memory>
#include <string>

#include "gui/component.h"

/** @addtogroup gui
 *  @{
 */

namespace st::gui
{
    /**
     * @brief A bar across the top of a frame that holds menus, buttons, checkboxes and separators.
     *
     * It is drawn only when it is a direct child of a frame, and a frame draws one bar.
     */
    class menubar : public nestable_component
    {
    public:
        menubar() noexcept;
        ~menubar() noexcept;

        void render() noexcept override;

        [[nodiscard]] st::bool_t is_menubar() const noexcept override;
    };

    /** @brief A named menu that opens when hovered or clicked. It can be nested in a menubar or in another menu. */
    class menu : public nestable_component
    {
    public:
        menu() noexcept;
        ~menu() noexcept;

        void render() noexcept override;

        /** @brief Sets whether the menu can be opened. @param enabled Whether enabled. */
        void set_enabled(st::bool_t enabled) noexcept;
        /** @brief Sets the name of the menu. @param text Name in UTF-8. */
        void set_text(const std::string &text) noexcept;

    private:
        struct menu_impl;
        std::unique_ptr<menu_impl> m_impl;
    };

    /** @brief An entry that runs callbacks when it is clicked. */
    class menu_button : public component
    {
    public:
        menu_button() noexcept;
        ~menu_button() noexcept;

        void render() noexcept override;

        /** @brief Sets whether the entry can be clicked. @param enabled Whether enabled. */
        void set_enabled(st::bool_t enabled) noexcept;
        /** @brief Sets the label. @param text Label in UTF-8. */
        void set_text(const std::string &text) noexcept;

        /** @brief Called when the entry is clicked. */
        using on_click_callback = std::function<void()>;

        /** @brief Adds a click callback. @param callback The callback. */
        void register_on_click(const on_click_callback &callback) noexcept;

        /** @brief Removes every click callback. */
        void clear_on_click() noexcept;

    protected:
        /** @brief Drops the click callbacks so that closures referencing the entry are released. */
        void handle_close() noexcept override;

    private:
        struct menu_button_impl;
        std::unique_ptr<menu_button_impl> m_impl;
    };

    /** @brief An entry with a check mark that the user toggles. */
    class menu_checkbox : public component
    {
    public:
        menu_checkbox() noexcept;
        ~menu_checkbox() noexcept;

        void render() noexcept override;

        /** @brief Sets the check mark without calling the toggle callbacks. @param checked Whether checked. */
        void set_checked(st::bool_t checked) noexcept;
        /** @brief Sets whether the entry can be toggled. @param enabled Whether enabled. */
        void set_enabled(st::bool_t enabled) noexcept;
        /** @brief Sets the label. @param text Label in UTF-8. */
        void set_text(const std::string &text) noexcept;

        /** @brief Whether the entry is checked. */
        [[nodiscard]] st::bool_t is_checked() const noexcept;

        /** @brief Called with the new state when the user toggles the entry. */
        using on_toggle_callback = std::function<void(st::bool_t)>;

        /** @brief Adds a toggle callback. @param callback The callback. */
        void register_on_toggle(const on_toggle_callback &callback) noexcept;

        /** @brief Removes every toggle callback. */
        void clear_on_toggle() noexcept;

    protected:
        /** @brief Drops the toggle callbacks so that closures referencing the entry are released. */
        void handle_close() noexcept override;

    private:
        struct menu_checkbox_impl;
        std::unique_ptr<menu_checkbox_impl> m_impl;
    };

    /** @brief A line that groups the entries of a menu. */
    class menu_separator : public component
    {
    public:
        menu_separator() noexcept;
        ~menu_separator() noexcept;

        void render() noexcept override;
    };
} // namespace st::gui

/** @} */

#endif // STICKY_GUI_MENUBAR_H
