/*
 * popup.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#ifndef STICKY_GUI_POPUP_H
#define STICKY_GUI_POPUP_H

#include <functional>
#include <memory>
#include <string>

#include "gui/component.h"

namespace st::gui
{
    class popup : public component
    {
    public:
        popup() noexcept;
        ~popup() noexcept;

        void close() noexcept;
        void render() noexcept override;

        void set_title(const std::string &title) noexcept;
        void set_text(const std::string &text) noexcept;
        void set_button_text(const std::string &button_text) noexcept;

        using on_close_callback = std::function<void()>;

        void register_on_close(const on_close_callback &callback) noexcept;

        void clear_on_close() noexcept;

    private:
        struct popup_impl;
        std::unique_ptr<popup_impl> m_impl;
    };
} // namespace st::gui

#endif // STICKY_GUI_POPUP_H
