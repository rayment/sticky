/*
 * component.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

/**
 * @file
 * @brief Base classes of every GUI component and the rules of their lifetime.
 */

#ifndef STICKY_GUI_COMPONENT_H
#define STICKY_GUI_COMPONENT_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <functional>
#include <memory>
#include <vector>

#include "st_macros.h"
#include "st_types.h"

/** @addtogroup gui
 *  @{
 */

namespace st::gui
{
    class nestable_component; // forward-declare

    /**
     * @brief Base class of every GUI component.
     *
     * A component is created hidden. It is rendered by its parent, or, when it has none, by st::gui::core once shown
     * with show(). Closing is final and idempotent: a closed component is detached from its parent and the render
     * list and is never rendered again.
     *
     * The optional `keepalive` of show() and set_parent() is held until the component is closed, orphaned or
     * destroyed. The Lua bindings use it to pin the Lua object for as long as the engine references it.
     *
     * Components are not thread-safe and must only be used from the thread that renders the GUI.
     */
    class component
    {
    public:
        /** @brief Creates a component. @param parent Initial parent, or null for none. */
        component(st::gui::nestable_component *parent = nullptr) noexcept;
        virtual ~component() noexcept = 0;

        ST_DELETE_COPY_CTORS(component);
        ST_DELETE_MOVE_CTORS(component);

        /** @brief Draws the component. Called by the parent or the core, once per frame. */
        virtual void render() noexcept = 0;

        /**
         * @brief Renders the component every frame until it is closed.
         *
         * Ignored, with a warning, if the component is closed or has a parent.
         *
         * @param keepalive Object held until the component is closed or destroyed.
         */
        void show(std::shared_ptr<void> keepalive = nullptr) noexcept;
        /** @brief Closes the component and all of its children. Does nothing if it is already closed. */
        void close() noexcept;
        /**
         * @brief Makes the component the last child of a parent, which then renders it.
         *
         * Ignored, with a warning, if the parent is null or closed, if either component is closed, if the component is
         * a root or mono component, or if the parent is the component itself or one of its descendants.
         *
         * @param parent The new parent.
         * @param keepalive Object held until the component is closed, orphaned or destroyed.
         */
        void set_parent(st::gui::nestable_component *parent,
                        std::shared_ptr<void>        keepalive = nullptr) noexcept;

        /** @brief A number that is unique to the component for the lifetime of the process. */
        [[nodiscard]] st::uint64 id() const noexcept;
        /** @brief Whether the component was closed. */
        [[nodiscard]] st::bool_t is_closed() const noexcept;
        /** @brief The parent of the component, or null. */
        [[nodiscard]] st::gui::nestable_component *parent() const noexcept;

        /** @brief Whether the component is a st::gui::menubar, which frames reserve room for. */
        [[nodiscard]] virtual st::bool_t is_menubar() const noexcept;

        /** @brief Renders every shown component. Called once per frame by st::gui::core::render(). */
        static void render_shown() noexcept;

    protected:
        /** @brief Called once when the component is closed, before it is detached. */
        virtual void handle_close() noexcept;

    private:
        friend class nestable_component;

        void detach() noexcept;
        void orphan() noexcept;

        [[nodiscard]] virtual st::bool_t can_have_parent() const noexcept;

        struct component_impl;
        std::unique_ptr<component_impl> m_impl;
    };

    /**
     * @brief A component that holds and renders children.
     *
     * Children are rendered in the order they were added. Closing the component closes its children. Destroying it
     * orphans them, which leaves them hidden.
     */
    class nestable_component : public component
    {
    public:
        friend class component;

        /** @brief Creates a component. @param parent Initial parent, or null for none. */
        nestable_component(st::gui::nestable_component *parent = nullptr) noexcept;
        virtual ~nestable_component() noexcept = 0;

        ST_DELETE_COPY_CTORS(nestable_component);
        ST_DELETE_MOVE_CTORS(nestable_component);

        /** @brief The children, in rendering order. */
        [[nodiscard]] std::vector<st::gui::component *> children() const noexcept;

    protected:
        /** @brief Closes every child. */
        void handle_close() noexcept override;

        /**
         * @brief Renders the children, tolerating any of them being closed or reparented while rendering.
         * @param between Called before every child that is rendered after the first.
         */
        void render_children(const std::function<void()> &between = nullptr) noexcept;

    private:
        struct nestable_component_impl;
        std::unique_ptr<nestable_component_impl> m_impl;
    };

    /** @brief The top of a rendering tree: rendered by the core once shown, and never given a parent. */
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

    /** @brief A component that stands alone: rendered by the core once shown, without a parent or children. */
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

/** @} */

#endif // STICKY_GUI_COMPONENT_H
