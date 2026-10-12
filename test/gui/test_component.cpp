/*
 * test_component.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#include <memory>

#include <doctest/doctest.h>

#include "st_types.h"

#include "gui/component.h"
#include "gui/frame.h"
#include "gui/popup.h"

namespace
{
    struct probe : st::gui::component
    {
        probe(st::gui::nestable_component *parent = nullptr) noexcept : component(parent) {}

        void render() noexcept override { ++renders; }

        st::int32 renders = 0;
        st::int32 closes = 0;

    protected:
        void handle_close() noexcept override { ++closes; }
    };

    struct nestable_probe : st::gui::nestable_component
    {
        nestable_probe(st::gui::nestable_component *parent = nullptr) noexcept : nestable_component(parent) {}

        void render() noexcept override
        {
            ++renders;
            render_children();
        }

        st::int32 renders = 0;
        st::int32 closes = 0;

    protected:
        void handle_close() noexcept override
        {
            ++closes;
            nestable_component::handle_close();
        }
    };

    struct closes_on_render : st::gui::component
    {
        st::gui::component *target = nullptr;

        void render() noexcept override
        {
            ++renders;
            if (target != nullptr)
                target->close();
        }

        st::int32 renders = 0;
    };

    [[nodiscard]] st::int32 child_count(st::gui::nestable_component &parent)
    {
        return static_cast<st::int32>(parent.children().size());
    }
} // namespace

TEST_SUITE("gui::component")
{
    TEST_CASE("a component constructed with a parent is one of its children")
    {
        nestable_probe parent;
        probe          child(&parent);
        CHECK_EQ(&parent, child.parent());
        CHECK_EQ(1, child_count(parent));
    }

    TEST_CASE("a child that is destroyed first leaves its parent")
    {
        nestable_probe parent;
        {
            probe child(&parent);
            CHECK_EQ(1, child_count(parent));
        }
        CHECK_EQ(0, child_count(parent));
    }

    TEST_CASE("a parent that is destroyed first orphans its children")
    {
        probe child;
        {
            nestable_probe parent;
            child.set_parent(&parent);
            CHECK_EQ(&parent, child.parent());
        }
        CHECK(child.parent() == nullptr);
        CHECK_FALSE(child.is_closed());
    }

    TEST_CASE("set_parent moves a component between parents")
    {
        nestable_probe first;
        nestable_probe second;
        probe          child(&first);

        child.set_parent(&second);

        CHECK_EQ(&second, child.parent());
        CHECK_EQ(0, child_count(first));
        CHECK_EQ(1, child_count(second));
    }

    TEST_CASE("set_parent to the same parent moves the child to the end")
    {
        nestable_probe parent;
        probe          first(&parent);
        probe          second(&parent);

        first.set_parent(&parent);

        const auto children = parent.children();
        CHECK_EQ(2, child_count(parent));
        CHECK_EQ(&second, children.front());
        CHECK_EQ(&first, children.back());
    }

    TEST_CASE("set_parent rejects cycles")
    {
        nestable_probe outer;
        nestable_probe inner(&outer);

        outer.set_parent(&inner);
        outer.set_parent(&outer);

        CHECK(outer.parent() == nullptr);
        CHECK_EQ(&outer, inner.parent());
    }

    TEST_CASE("set_parent rejects closed components")
    {
        nestable_probe parent;
        probe          child;

        child.close();
        child.set_parent(&parent);
        CHECK(child.parent() == nullptr);

        probe          other;
        nestable_probe closed_parent;
        closed_parent.close();
        other.set_parent(&closed_parent);
        CHECK(other.parent() == nullptr);
    }

    TEST_CASE("set_parent rejects root components")
    {
        nestable_probe parent;
        st::gui::frame frame;
        st::gui::popup popup;

        frame.set_parent(&parent);
        popup.set_parent(&parent);

        CHECK(frame.parent() == nullptr);
        CHECK(popup.parent() == nullptr);
        CHECK_EQ(0, child_count(parent));
    }

    TEST_CASE("close is idempotent")
    {
        probe component;
        component.close();
        component.close();
        CHECK(component.is_closed());
        CHECK_EQ(1, component.closes);
    }

    TEST_CASE("closing a component detaches it from its parent")
    {
        nestable_probe parent;
        probe          child(&parent);

        child.close();

        CHECK(child.parent() == nullptr);
        CHECK_EQ(0, child_count(parent));
        CHECK_FALSE(parent.is_closed());
    }

    TEST_CASE("closing a parent closes its children")
    {
        nestable_probe parent;
        nestable_probe middle(&parent);
        probe          leaf(&middle);

        parent.close();

        CHECK(parent.is_closed());
        CHECK(middle.is_closed());
        CHECK(leaf.is_closed());
        CHECK_EQ(1, middle.closes);
        CHECK_EQ(1, leaf.closes);
        CHECK_EQ(0, child_count(parent));
    }

    TEST_CASE("a closed parent does not render its children")
    {
        nestable_probe parent;
        probe          child(&parent);

        parent.render();
        CHECK_EQ(1, child.renders);

        parent.close();
        child.set_parent(&parent);
        parent.render();
        CHECK_EQ(1, child.renders);
    }

    TEST_CASE("a parent renders every child even when one closes itself")
    {
        nestable_probe   parent;
        closes_on_render first;
        probe            second;
        first.set_parent(&parent);
        second.set_parent(&parent);
        first.target = &first;

        parent.render();

        CHECK_EQ(1, first.renders);
        CHECK_EQ(1, second.renders);
        CHECK(first.is_closed());
    }
}

TEST_SUITE("gui::component shown")
{
    TEST_CASE("shown components are rendered until they are closed")
    {
        probe component;
        component.show();

        st::gui::component::render_shown();
        st::gui::component::render_shown();
        CHECK_EQ(2, component.renders);

        component.close();
        st::gui::component::render_shown();
        CHECK_EQ(2, component.renders);
    }

    TEST_CASE("showing twice renders once per frame")
    {
        probe component;
        component.show();
        component.show();

        st::gui::component::render_shown();

        CHECK_EQ(1, component.renders);
    }

    TEST_CASE("a component that is not shown is not rendered")
    {
        probe component;
        st::gui::component::render_shown();
        CHECK_EQ(0, component.renders);
    }

    TEST_CASE("a component that is destroyed is no longer rendered")
    {
        probe survivor;
        survivor.show();
        {
            probe doomed;
            doomed.show();
        }

        st::gui::component::render_shown();

        CHECK_EQ(1, survivor.renders);
    }

    TEST_CASE("a component with a parent cannot be shown")
    {
        nestable_probe parent;
        probe          child(&parent);

        child.show();
        st::gui::component::render_shown();

        CHECK_EQ(0, child.renders);
    }

    TEST_CASE("a closed component cannot be shown")
    {
        probe component;
        component.close();

        component.show();
        st::gui::component::render_shown();

        CHECK_EQ(0, component.renders);
    }

    TEST_CASE("giving a shown component a parent stops it being rendered by the core")
    {
        nestable_probe parent;
        probe          child;
        child.show();

        child.set_parent(&parent);
        st::gui::component::render_shown();

        CHECK_EQ(0, child.renders);
    }

    TEST_CASE("closing another shown component while rendering is safe")
    {
        probe            victim;
        closes_on_render closer;
        probe            bystander;
        closer.target = &victim;
        victim.show();
        closer.show();
        bystander.show();

        st::gui::component::render_shown();

        CHECK_EQ(1, victim.renders);
        CHECK(victim.is_closed());
        CHECK_EQ(1, bystander.renders);

        st::gui::component::render_shown();
        CHECK_EQ(1, victim.renders);
        CHECK_EQ(2, bystander.renders);
    }
}

TEST_SUITE("gui::component keepalive")
{
    TEST_CASE("the keepalive of a shown component is held until it is closed")
    {
        const auto token = std::make_shared<int>(0);
        probe      component;

        component.show(token);
        CHECK_EQ(2, token.use_count());

        component.close();
        CHECK_EQ(1, token.use_count());
    }

    TEST_CASE("the keepalive of a child is released when its parent closes")
    {
        const auto     token = std::make_shared<int>(0);
        nestable_probe parent;
        probe          child;

        child.set_parent(&parent, token);
        CHECK_EQ(2, token.use_count());

        parent.close();
        CHECK_EQ(1, token.use_count());
    }

    TEST_CASE("the keepalive of a child is released when its parent is destroyed")
    {
        const auto token = std::make_shared<int>(0);
        probe      child;
        {
            nestable_probe parent;
            child.set_parent(&parent, token);
            CHECK_EQ(2, token.use_count());
        }
        CHECK_EQ(1, token.use_count());
        CHECK_FALSE(child.is_closed());
    }

    TEST_CASE("a keepalive that is refused is not held")
    {
        const auto token = std::make_shared<int>(0);
        probe      component;
        component.close();

        component.show(token);

        CHECK_EQ(1, token.use_count());
    }

    TEST_CASE("a new keepalive replaces the previous one")
    {
        const auto first = std::make_shared<int>(0);
        const auto second = std::make_shared<int>(0);
        probe      component;

        component.show(first);
        component.show(second);

        CHECK_EQ(1, first.use_count());
        CHECK_EQ(2, second.use_count());
    }
}

TEST_SUITE("gui::frame")
{
    TEST_CASE("close invokes registered callbacks once")
    {
        st::gui::frame frame;
        st::int32      calls = 0;
        frame.register_on_close([&calls] { ++calls; });

        frame.close();
        frame.close();

        CHECK_EQ(1, calls);
        CHECK(frame.is_closed());
    }

    TEST_CASE("clear_on_close removes callbacks")
    {
        st::gui::frame frame;
        st::int32      calls = 0;
        frame.register_on_close([&calls] { ++calls; });
        frame.clear_on_close();

        frame.close();

        CHECK_EQ(0, calls);
    }

    TEST_CASE("a callback may close the frame again")
    {
        st::gui::frame frame;
        st::int32      calls = 0;
        frame.register_on_close(
            [&]
            {
                ++calls;
                frame.close();
            });

        frame.close();

        CHECK_EQ(1, calls);
    }

    TEST_CASE("closing a frame closes its children")
    {
        st::gui::frame frame;
        probe          child(&frame);

        frame.close();

        CHECK(child.is_closed());
        CHECK_EQ(1, child.closes);
    }

    TEST_CASE("callbacks are released when the frame closes")
    {
        const auto     token = std::make_shared<int>(0);
        st::gui::frame frame;
        frame.register_on_close([token] {});
        CHECK_EQ(2, token.use_count());

        frame.close();

        CHECK_EQ(1, token.use_count());
    }
}
