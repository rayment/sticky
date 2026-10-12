/*
 * component.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#include <algorithm>
#include <utility>

#include "io/log.h"
#include "st_macros.h"

#include "gui/component.h"

namespace
{
    st::uint64 next_id = 0;

    // Components that are rendered by the core rather than by a parent. Entries are nulled instead of erased while
    // rendering, so that a callback may close or destroy components without invalidating the iteration.
    std::vector<st::gui::component *> shown_components;
    st::bool_t                        rendering_shown = false;

    void remove_shown(const st::gui::component *component) noexcept
    {
        const auto it = std::find(shown_components.begin(), shown_components.end(), component);
        if (it == shown_components.end())
            return;
        if (rendering_shown)
            *it = nullptr;
        else
            shown_components.erase(it);
    }

    [[nodiscard]] st::bool_t is_ancestor(const st::gui::component          *component,
                                         const st::gui::nestable_component *candidate) noexcept
    {
        for (const st::gui::component *it = candidate; it != nullptr; it = it->parent())
        {
            if (it == component)
                return true;
        }
        return false;
    }
} // namespace

struct st::gui::component::component_impl
{
    component_impl(st::gui::nestable_component *parent) noexcept :
        m_id(next_id++),
        m_parent(parent),
        m_closed(false),
        m_shown(false)
    {
    }
    ~component_impl() noexcept = default;

    st::uint64                   m_id;
    st::gui::nestable_component *m_parent;
    st::bool_t                   m_closed;
    st::bool_t                   m_shown;
    std::shared_ptr<void>        m_keepalive;
};

struct st::gui::nestable_component::nestable_component_impl
{
    nestable_component_impl() noexcept {}
    ~nestable_component_impl() noexcept = default;

    std::vector<st::gui::component *> m_children;
};

st::gui::component::component(st::gui::nestable_component *parent) noexcept :
    m_impl{std::make_unique<component_impl>(parent)}
{
    if (parent != nullptr)
        parent->m_impl->m_children.push_back(this);
}

st::gui::component::~component() noexcept { detach(); }

st::uint64
st::gui::component::id() const
{
    return m_impl->m_id;
}

st::bool_t
st::gui::component::is_closed() const noexcept
{
    return m_impl->m_closed;
}

st::gui::nestable_component *
st::gui::component::parent() const
{
    return m_impl->m_parent;
}

void
st::gui::component::detach() noexcept
{
    if (m_impl->m_parent != nullptr)
    {
        auto &siblings = m_impl->m_parent->m_impl->m_children;
        siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        m_impl->m_parent = nullptr;
    }
    if (m_impl->m_shown)
    {
        remove_shown(this);
        m_impl->m_shown = false;
    }
}

void
st::gui::component::orphan() noexcept
{
    m_impl->m_parent = nullptr;
    m_impl->m_keepalive.reset();
}

void
st::gui::component::handle_close() noexcept
{
}

st::bool_t
st::gui::component::can_have_parent() const noexcept
{
    return true;
}

void
st::gui::component::close() noexcept
{
    if (m_impl->m_closed)
        return;
    m_impl->m_closed = true;

    handle_close();
    detach();
    m_impl->m_keepalive.reset();
}

void
st::gui::component::show(std::shared_ptr<void> keepalive) noexcept
{
    if (m_impl->m_closed)
    {
        st::io::log::warn("Cannot show a GUI component that has been closed.");
        return;
    }
    if (m_impl->m_parent != nullptr)
    {
        st::io::log::warn("Cannot show a GUI component that has a parent; the parent renders it.");
        return;
    }

    m_impl->m_keepalive = std::move(keepalive);
    if (m_impl->m_shown)
        return;
    m_impl->m_shown = true;
    shown_components.push_back(this);
}

void
st::gui::component::set_parent(st::gui::nestable_component *parent,
                               std::shared_ptr<void>        keepalive) noexcept
{
    if (parent == nullptr)
    {
        st::io::log::warn("Cannot set a GUI component's parent to nothing.");
        return;
    }
    if (!can_have_parent())
    {
        st::io::log::warn("Cannot set the parent of a root GUI component.");
        return;
    }
    if (m_impl->m_closed || parent->is_closed())
    {
        st::io::log::warn("Cannot parent a GUI component to or from a closed component.");
        return;
    }
    if (is_ancestor(this, parent))
    {
        st::io::log::warn("Cannot parent a GUI component to itself or one of its descendants.");
        return;
    }

    m_impl->m_keepalive = std::move(keepalive);
    if (parent == m_impl->m_parent)
    {
        // Set to the same parent. Move to the end of child list.
        auto &siblings = parent->m_impl->m_children;
        auto  it = std::find(siblings.begin(), siblings.end(), this);
        ST_ASSERT(it != siblings.end());
        std::rotate(it, it + 1, siblings.end());
        return;
    }

    detach();
    m_impl->m_parent = parent;
    parent->m_impl->m_children.push_back(this);
}

void
st::gui::component::render_shown() noexcept
{
    rendering_shown = true;
    for (st::size i = 0; i < shown_components.size(); ++i)
    {
        if (shown_components[i] != nullptr)
            shown_components[i]->render();
    }
    rendering_shown = false;

    shown_components.erase(std::remove(shown_components.begin(), shown_components.end(), nullptr),
                           shown_components.end());
}

st::gui::nestable_component::nestable_component(st::gui::nestable_component *parent) noexcept :
    component(parent),
    m_impl{std::make_unique<nestable_component_impl>()}
{
}

st::gui::nestable_component::~nestable_component() noexcept
{
    auto children = std::move(m_impl->m_children);
    m_impl->m_children.clear();
    for (component *child : children)
        child->orphan();
}

std::vector<st::gui::component *>
st::gui::nestable_component::children() noexcept
{
    return m_impl->m_children;
}

void
st::gui::nestable_component::handle_close() noexcept
{
    const auto children = m_impl->m_children;
    for (component *child : children)
        child->close();
}

st::gui::root_component::root_component() noexcept : nestable_component(nullptr) {}

st::gui::root_component::~root_component() noexcept {}

st::bool_t
st::gui::root_component::can_have_parent() const noexcept
{
    return false;
}

st::gui::mono_component::mono_component() noexcept : component(nullptr) {}

st::gui::mono_component::~mono_component() noexcept {}

st::bool_t
st::gui::mono_component::can_have_parent() const noexcept
{
    return false;
}

void
st::gui::nestable_component::render_children() noexcept
{
    auto &children = m_impl->m_children;
    for (st::size i = 0; i < children.size();)
    {
        component *child = children[i];
        child->render();
        // A child that closed or moved itself was removed from the list, which shifted its successor into `i`.
        if (i < children.size() && children[i] == child)
            ++i;
    }
}
