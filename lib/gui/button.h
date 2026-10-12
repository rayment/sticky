/*
 * button.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#ifndef STICKY_GUI_BUTTON_H
#define STICKY_GUI_BUTTON_H

#include <functional>
#include <memory>
#include <string>

#include "gui/component.h"

namespace st::gui
{
    class button : public component
    {
    public:
         button() noexcept;
        ~button() noexcept;

        void render() noexcept override;

        void set_enabled(st::bool_t enabled)      noexcept;
        void set_text   (const std::string &text) noexcept;

        using on_click_callback = std::function<void()>;

        void register_on_click(const on_click_callback &callback) noexcept;

        void clear_on_click() noexcept;

    private:
        struct button_impl;
        std::unique_ptr<button_impl> m_impl;
    };
}

#endif // STICKY_GUI_BUTTON_H
