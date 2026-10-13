/*
 * button.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief A clickable push button.
 */

#ifndef STICKY_GUI_BUTTON_H
#define STICKY_GUI_BUTTON_H

#include <functional>
#include <memory>
#include <string>

#include "gui/component.h"

/** @addtogroup gui
 *  @{
 */

namespace st::gui
{
    /** @brief A push button with a text label that runs callbacks when clicked. */
    class button : public component
    {
    public:
        button() noexcept;
        ~button() noexcept;

        void render() noexcept override;

        /**
         * @brief Sets whether the button can be clicked. A disabled button is greyed out.
         * @param enabled Whether enabled.
         */
        void set_enabled(st::bool_t enabled) noexcept;
        /** @brief Sets the label. @param text Label in UTF-8. */
        void set_text(const std::string &text) noexcept;

        /** @brief Called when the button is clicked. */
        using on_click_callback = std::function<void()>;

        /** @brief Adds a click callback. Callbacks run in the order they were added. @param callback The callback. */
        void register_on_click(const on_click_callback &callback) noexcept;

        /** @brief Removes every click callback. */
        void clear_on_click() noexcept;

    protected:
        /** @brief Drops the click callbacks so that closures referencing the button are released. */
        void handle_close() noexcept override;

    private:
        struct button_impl;
        std::unique_ptr<button_impl> m_impl;
    };
} // namespace st::gui

/** @} */

#endif // STICKY_GUI_BUTTON_H
