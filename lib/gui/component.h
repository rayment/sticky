/*
 * component.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#ifndef STICKY_GUI_COMPONENT_H
#define STICKY_GUI_COMPONENT_H

#include <functional>
#include <memory>
#include <vector>

#include "st_macros.h"
#include "st_types.h"

namespace st::gui
{
    class nestable_component; // forward-declare

    /*
     * A component is created hidden. It is rendered by its parent, or once shown with show() when it has none.
     * Closing is final and idempotent: a closed component is detached from its parent and the render list and no
     * longer renders.
     *
     * The optional `keepalive` of show() and set_parent() is held until the component is closed, orphaned or
     * destroyed. The Lua bindings use it to pin the Lua object for as long as the engine references it.
     */
    class component
    {
    public:
        component(st::gui::nestable_component *parent = nullptr) noexcept;
        virtual ~component() noexcept = 0;

        ST_DELETE_COPY_CTORS(component);
        ST_DELETE_MOVE_CTORS(component);

        virtual void render() noexcept = 0;

        void show(std::shared_ptr<void> keepalive = nullptr) noexcept;
        void close() noexcept;
        void set_parent(st::gui::nestable_component *parent,
                        std::shared_ptr<void>        keepalive = nullptr) noexcept;

        [[nodiscard]] st::uint64                   id() const;
        [[nodiscard]] st::bool_t                   is_closed() const noexcept;
        [[nodiscard]] st::gui::nestable_component *parent() const;

        // Lets a frame reserve a menu bar without RTTI.
        [[nodiscard]] virtual st::bool_t is_menubar() const noexcept;

        // Renders every shown component. Called once per frame by the core.
        static void render_shown() noexcept;

    protected:
        // Called once when the component is closed, before it is detached.
        virtual void handle_close() noexcept;

    private:
        friend class nestable_component;

        void detach() noexcept;
        void orphan() noexcept;

        [[nodiscard]] virtual st::bool_t can_have_parent() const noexcept;

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

    protected:
        void handle_close() noexcept override;

        // Renders the children, tolerating any of them being closed or reparented while rendering. `between` is
        // called before every child that is rendered after the first.
        void render_children(const std::function<void()> &between = nullptr) noexcept;

    private:
        struct nestable_component_impl;
        std::unique_ptr<nestable_component_impl> m_impl;
    };

    /*
     * A root component is the top of a rendering tree: it is rendered by the core once shown and can never be
     * given a parent.
     */
    class root_component : public nestable_component
    {
    public:
        root_component() noexcept;
        virtual ~root_component() noexcept = 0;

        ST_DELETE_COPY_CTORS(root_component);
        ST_DELETE_MOVE_CTORS(root_component);

    private:
        [[nodiscard]] st::bool_t can_have_parent() const noexcept override;
    };

    /*
     * A mono component stands alone: it is rendered by the core once shown, and can neither have a parent nor
     * children.
     */
    class mono_component : public component
    {
    public:
        mono_component() noexcept;
        virtual ~mono_component() noexcept = 0;

        ST_DELETE_COPY_CTORS(mono_component);
        ST_DELETE_MOVE_CTORS(mono_component);

    private:
        [[nodiscard]] st::bool_t can_have_parent() const noexcept override;
    };
} // namespace st::gui

#endif // STICKY_GUI_COMPONENT_H
