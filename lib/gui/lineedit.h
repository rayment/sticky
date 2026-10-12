/*
 * lineedit.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#ifndef STICKY_GUI_LINEEDIT_H
#define STICKY_GUI_LINEEDIT_H

#include <memory>
#include <string>

#include "gui/component.h"

namespace st::gui
{
    // A single-line text editor that fills the width available to it.
    class lineedit : public component
    {
    public:
        lineedit() noexcept;
        ~lineedit() noexcept;

        void render() noexcept override;

        [[nodiscard]] const std::string &text() const noexcept;
        [[nodiscard]] st::bool_t         is_enabled() const noexcept;
        [[nodiscard]] st::bool_t         is_read_only() const noexcept;
        [[nodiscard]] st::bool_t         is_masked() const noexcept;

        void set_text(const std::string &text) noexcept;
        // Shown in grey while the editor is empty.
        void set_hint(const std::string &hint) noexcept;
        // A disabled editor is greyed out and cannot be focused.
        void set_enabled(st::bool_t enabled) noexcept;
        // A read-only editor can be focused and selected, but not edited.
        void set_read_only(st::bool_t read_only) noexcept;
        // A masked editor shows every character as '*' and cannot be copied from.
        void set_masked(st::bool_t masked) noexcept;

    private:
        struct lineedit_impl;
        std::unique_ptr<lineedit_impl> m_impl;
    };
} // namespace st::gui

#endif // STICKY_GUI_LINEEDIT_H
