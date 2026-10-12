/*
 * label.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#ifndef STICKY_GUI_LABEL_H
#define STICKY_GUI_LABEL_H

#include <memory>
#include <string>

#include "gui/component.h"

namespace st::gui
{
    class label : public component
    {
    public:
        label() noexcept;
        ~label() noexcept;

        void render() noexcept override;

        void set_text(const std::string &text) noexcept;

        // With both dimensions above zero the text is centred in a box of that size. Otherwise the label fits the text.
        void set_size(st::float32 width,
                      st::float32 height) noexcept;

    private:
        struct label_impl;
        std::unique_ptr<label_impl> m_impl;
    };
} // namespace st::gui

#endif // STICKY_GUI_LABEL_H
