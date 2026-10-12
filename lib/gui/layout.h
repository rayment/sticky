/*
 * layout.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#ifndef STICKY_GUI_LAYOUT_H
#define STICKY_GUI_LAYOUT_H

#include <memory>

#include "gui/component.h"

namespace st::gui
{
    // A layout arranges its children as one item, and can be nested in a frame or in another layout.
    class layout : public nestable_component
    {
    public:
        layout() noexcept;
        virtual ~layout() noexcept = 0;

        ST_DELETE_COPY_CTORS(layout);
        ST_DELETE_MOVE_CTORS(layout);

        // The gap between children, in pixels. A negative value restores the default spacing of the theme.
        void set_spacing(st::float32 spacing) noexcept;

    protected:
        void render_group(st::bool_t horizontal) noexcept;

    private:
        struct layout_impl;
        std::unique_ptr<layout_impl> m_impl;
    };

    // Places its children side by side, from left to right.
    class hlayout : public layout
    {
    public:
        hlayout() noexcept;
        ~hlayout() noexcept;

        void render() noexcept override;
    };

    // Places its children on top of each other, from top to bottom.
    class vlayout : public layout
    {
    public:
        vlayout() noexcept;
        ~vlayout() noexcept;

        void render() noexcept override;
    };
} // namespace st::gui

#endif // STICKY_GUI_LAYOUT_H
