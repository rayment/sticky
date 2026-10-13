/*
 * label.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief Static text.
 */

#ifndef STICKY_GUI_LABEL_H
#define STICKY_GUI_LABEL_H

#include <memory>
#include <string>

#include "gui/component.h"

/** @addtogroup gui
 *  @{
 */

namespace st::gui
{
    /** @brief A line of text that the user cannot edit. */
    class label : public component
    {
    public:
        label() noexcept;
        ~label() noexcept;

        void render() noexcept override;

        /** @brief Sets the text. @param text Text in UTF-8. */
        void set_text(const std::string &text) noexcept;

        /**
         * @brief Sets the size of the box the text is centred in.
         *
         * With either dimension at zero or below, the label fits the text instead.
         *
         * @param width Width in pixels.
         * @param height Height in pixels.
         */
        void set_size(st::float32 width,
                      st::float32 height) noexcept;

    private:
        struct label_impl;
        std::unique_ptr<label_impl> m_impl;
    };
} // namespace st::gui

/** @} */

#endif // STICKY_GUI_LABEL_H
