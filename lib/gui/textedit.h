/*
 * textedit.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#ifndef STICKY_GUI_TEXTEDIT_H
#define STICKY_GUI_TEXTEDIT_H

#include <memory>
#include <string>

#include "gui/component.h"

namespace st::gui
{
    // A multi-line text editor.
    class textedit : public component
    {
    public:
        textedit() noexcept;
        ~textedit() noexcept;

        void render() noexcept override;

        [[nodiscard]] const std::string &text() const noexcept;
        [[nodiscard]] st::bool_t         is_enabled() const noexcept;
        [[nodiscard]] st::bool_t         is_read_only() const noexcept;
        [[nodiscard]] st::bool_t         is_masked() const noexcept;
        [[nodiscard]] st::bool_t         has_vertical_scrollbar() const noexcept;
        [[nodiscard]] st::bool_t         has_horizontal_scrollbar() const noexcept;

        void set_text(const std::string &text) noexcept;
        // A disabled editor is greyed out and cannot be focused.
        void set_enabled(st::bool_t enabled) noexcept;
        // A read-only editor can be focused and selected, but not edited.
        void set_read_only(st::bool_t read_only) noexcept;
        // A masked editor shows every character as '*' and cannot be copied from.
        void set_masked(st::bool_t masked) noexcept;
        // Shows or hides the vertical scrollbar. The text can still be scrolled with the wheel and the caret.
        void set_vertical_scrollbar(st::bool_t visible) noexcept;
        // Lines scroll horizontally instead of wrapping when enabled, and wrap at the width of the editor otherwise.
        void set_horizontal_scrollbar(st::bool_t visible) noexcept;

        // A width or height of zero or below fills the available width and gives the default height respectively.
        void set_size(st::float32 width,
                      st::float32 height) noexcept;

    private:
        struct textedit_impl;
        std::unique_ptr<textedit_impl> m_impl;
    };
} // namespace st::gui

#endif // STICKY_GUI_TEXTEDIT_H
