/*
 * lineedit.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

/**
 * @file
 * @brief A single-line text editor.
 */

#ifndef STICKY_GUI_LINEEDIT_H
#define STICKY_GUI_LINEEDIT_H

#include <memory>
#include <string>

#include "gui/component.h"

/** @addtogroup gui
 *  @{
 */

namespace st::gui
{
    /** @brief A single-line text editor that fills the width available to it. */
    class lineedit : public component
    {
    public:
        lineedit() noexcept;
        ~lineedit() noexcept;

        void render() noexcept override;

        /** @brief The current text, in UTF-8. */
        [[nodiscard]] const std::string &text() const noexcept;
        /** @brief Whether the editor can be focused. */
        [[nodiscard]] st::bool_t is_enabled() const noexcept;
        /** @brief Whether the text can be selected but not edited. */
        [[nodiscard]] st::bool_t is_read_only() const noexcept;
        /** @brief Whether every character is shown as '*'. */
        [[nodiscard]] st::bool_t is_masked() const noexcept;

        /** @brief Replaces the text. @param text Text in UTF-8. */
        void set_text(const std::string &text) noexcept;
        /** @brief Sets the text shown in grey while the editor is empty. @param hint Text in UTF-8. */
        void set_hint(const std::string &hint) noexcept;
        /**
         * @brief Sets whether the editor can be focused. A disabled editor is greyed out.
         * @param enabled Whether enabled.
         */
        void set_enabled(st::bool_t enabled) noexcept;
        /** @brief Sets whether the text can be selected but not edited. @param read_only Whether read-only. */
        void set_read_only(st::bool_t read_only) noexcept;
        /** @brief Sets whether every character is shown as '*' and copying is blocked. @param masked Whether masked. */
        void set_masked(st::bool_t masked) noexcept;

    private:
        struct lineedit_impl;
        std::unique_ptr<lineedit_impl> m_impl;
    };
} // namespace st::gui

/** @} */

#endif // STICKY_GUI_LINEEDIT_H
