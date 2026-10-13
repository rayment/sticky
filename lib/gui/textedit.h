/*
 * textedit.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

/**
 * @file
 * @brief A multi-line text editor.
 */

#ifndef STICKY_GUI_TEXTEDIT_H
#define STICKY_GUI_TEXTEDIT_H

#include <memory>
#include <string>

#include "gui/component.h"

/** @addtogroup gui
 *  @{
 */

namespace st::gui
{
    /** @brief A multi-line text editor. */
    class textedit : public component
    {
    public:
        textedit() noexcept;
        ~textedit() noexcept;

        void render() noexcept override;

        /** @brief The current text, in UTF-8. */
        [[nodiscard]] const std::string &text() const noexcept;
        /** @brief Whether the editor can be focused. */
        [[nodiscard]] st::bool_t is_enabled() const noexcept;
        /** @brief Whether the text can be selected but not edited. */
        [[nodiscard]] st::bool_t is_read_only() const noexcept;
        /** @brief Whether every character is shown as '*'. */
        [[nodiscard]] st::bool_t is_masked() const noexcept;
        /** @brief Whether the vertical scrollbar is drawn. */
        [[nodiscard]] st::bool_t has_vertical_scrollbar() const noexcept;
        /** @brief Whether long lines wrap at the width of the editor rather than scrolling sideways. */
        [[nodiscard]] st::bool_t is_word_wrapped() const noexcept;

        /** @brief Replaces the text. @param text Text in UTF-8. */
        void set_text(const std::string &text) noexcept;
        /**
         * @brief Sets whether the editor can be focused. A disabled editor is greyed out.
         * @param enabled Whether enabled.
         */
        void set_enabled(st::bool_t enabled) noexcept;
        /** @brief Sets whether the text can be selected but not edited. @param read_only Whether read-only. */
        void set_read_only(st::bool_t read_only) noexcept;
        /** @brief Sets whether every character is shown as '*' and copying is blocked. @param masked Whether masked. */
        void set_masked(st::bool_t masked) noexcept;
        /**
         * @brief Shows or hides the vertical scrollbar. The text still scrolls with the wheel and the caret.
         * @param visible Whether the scrollbar is drawn. Shown by default.
         */
        void set_vertical_scrollbar(st::bool_t visible) noexcept;
        /**
         * @brief Sets whether long lines wrap at the width of the editor, or scroll sideways instead.
         * @param wrap Whether to wrap. Enabled by default.
         */
        void set_word_wrap(st::bool_t wrap) noexcept;

        /**
         * @brief Sets the size.
         * @param width Width in pixels, or zero or below to fill the available width.
         * @param height Height in pixels, or zero or below for the default height.
         */
        void set_size(st::float32 width,
                      st::float32 height) noexcept;

    private:
        struct textedit_impl;
        std::unique_ptr<textedit_impl> m_impl;
    };
} // namespace st::gui

/** @} */

#endif // STICKY_GUI_TEXTEDIT_H
