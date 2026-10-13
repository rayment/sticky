/*
 * popup.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief A message box drawn by the GUI.
 */

#ifndef STICKY_GUI_POPUP_H
#define STICKY_GUI_POPUP_H

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
     * @brief A frame with a message and a single button, which closes it.
     *
     * Unlike st::os::popup, it does not block: the game keeps running while it is shown.
     */
    class popup : public mono_component
    {
    public:
        popup() noexcept;
        ~popup() noexcept;

        void render() noexcept override;

        /** @brief Sets the title. @param title Title in UTF-8. */
        void set_title(const std::string &title) noexcept;
        /** @brief Sets the message. @param text Message in UTF-8. */
        void set_text(const std::string &text) noexcept;
        /** @brief Sets the label of the button, "OK" by default. @param button_text Label in UTF-8. */
        void set_button_text(const std::string &button_text) noexcept;

        /** @brief Called once when the popup closes. */
        using on_close_callback = std::function<void()>;

        /** @brief Adds a close callback. @param callback The callback. */
        void register_on_close(const on_close_callback &callback) noexcept;

        /** @brief Removes every close callback. */
        void clear_on_close() noexcept;

    protected:
        /** @brief Fires the close callbacks once and then drops them. */
        void handle_close() noexcept override;

    private:
        struct popup_impl;
        std::unique_ptr<popup_impl> m_impl;
    };
} // namespace st::gui

/** @} */

#endif // STICKY_GUI_POPUP_H
