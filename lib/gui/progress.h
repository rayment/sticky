/*
 * progress.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief A progress bar.
 */

#ifndef STICKY_GUI_PROGRESS_H
#define STICKY_GUI_PROGRESS_H

#include <memory>
#include <string>

#include "gui/component.h"

/** @addtogroup gui
 *  @{
 */

namespace st::gui
{
    /** @brief A bar that fills the available width and shows how much of a task is done. */
    class progress : public component
    {
    public:
        progress() noexcept;
        ~progress() noexcept;

        void render() noexcept override;

        /** @brief The fraction done, from 0 to 1. */
        [[nodiscard]] st::float32 value() const noexcept;

        /** @brief Sets the fraction done. @param value Clamped to the range 0 to 1. */
        void set_value(st::float32 value) noexcept;
        /** @brief Sets the text drawn over the bar. @param text Text in UTF-8, or empty to show the percentage. */
        void set_text(const std::string &text) noexcept;

    private:
        struct progress_impl;
        std::unique_ptr<progress_impl> m_impl;
    };
} // namespace st::gui

/** @} */

#endif // STICKY_GUI_PROGRESS_H
