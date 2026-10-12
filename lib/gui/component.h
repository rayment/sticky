/*
 * component.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#ifndef STICKY_GUI_COMPONENT_H
#define STICKY_GUI_COMPONENT_H

#include <memory>
#include <vector>

#include "st_macros.h"
#include "st_types.h"

namespace st::gui
{
    class nestable_component; // forward-declare

    class component
    {
    public:
        component(st::gui::nestable_component *parent = nullptr) noexcept;
        virtual ~component() noexcept = 0;

        ST_DELETE_COPY_CTORS(component);
        ST_DELETE_MOVE_CTORS(component);

        virtual void render() noexcept = 0;

        [[nodiscard]] st::uint64                   id()     const;
        [[nodiscard]] st::gui::nestable_component *parent() const;

        void set_parent(st::gui::nestable_component *parent) noexcept;

    private:
        struct component_impl;
        std::unique_ptr<component_impl> m_impl;
    };

    class nestable_component : public component
    {
    public:
        friend class component;

        nestable_component(st::gui::nestable_component *parent = nullptr) noexcept;
        virtual ~nestable_component() noexcept = 0;

        ST_DELETE_COPY_CTORS(nestable_component);
        ST_DELETE_MOVE_CTORS(nestable_component);

        std::vector<st::gui::component *> children() noexcept;

    private:
        struct nestable_component_impl;
        std::unique_ptr<nestable_component_impl> m_impl;
    };
}

#endif // STICKY_GUI_COMPONENT_H
