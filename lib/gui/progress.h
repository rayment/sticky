/*
 * progress.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#ifndef STICKY_GUI_PROGRESS_H
#define STICKY_GUI_PROGRESS_H

#include <memory>
#include <string>

#include "gui/component.h"

namespace st::gui
{
    class progress : public component
    {
    public:
        progress() noexcept;
        ~progress() noexcept;

        void render() noexcept override;

        [[nodiscard]] st::float32 value() const noexcept;

        // Clamped to the range [0, 1].
        void set_value(st::float32 value) noexcept;
        // Overlay text. An empty string shows the percentage complete.
        void set_text(const std::string &text) noexcept;

    private:
        struct progress_impl;
        std::unique_ptr<progress_impl> m_impl;
    };
} // namespace st::gui

#endif // STICKY_GUI_PROGRESS_H
