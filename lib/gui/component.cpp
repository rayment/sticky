/*
 * component.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#include "gui/component.h"

st::uint64 next_id = 0;

struct st::gui::component::component_impl
{
    component_impl(st::gui::nestable_component *parent) noexcept
        : m_id(next_id++),
          m_parent(parent)
    {}
    ~component_impl() noexcept = default;

    st::uint64 m_id;
    st::gui::nestable_component *m_parent;
};

struct st::gui::nestable_component::nestable_component_impl
{
    nestable_component_impl() noexcept
    {}
    ~nestable_component_impl() noexcept = default;

    std::vector<st::gui::component *> m_children;
};

st::gui::component::component(st::gui::nestable_component *parent) noexcept
    : m_impl{std::make_unique<component_impl>(parent)}
{}

st::gui::component::~component() noexcept
{}

st::uint64
st::gui::component::id() const
{
    return m_impl->m_id;
}

st::gui::nestable_component *
st::gui::component::parent() const
{
    return m_impl->m_parent;
}

st::gui::nestable_component::nestable_component(st::gui::nestable_component *parent) noexcept
    : component(parent),
      m_impl{std::make_unique<nestable_component_impl>()}
{}

st::gui::nestable_component::~nestable_component() noexcept
{}

std::vector<st::gui::component *>
st::gui::nestable_component::children() noexcept
{
    return m_impl->m_children;
}
