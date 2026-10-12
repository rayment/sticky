/*
 * texture_handle.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include "video/texture_handle.h"

namespace
{
    const st::video::texture_registry *current_registry = nullptr;
} // namespace

st::video::texture_handle::texture_handle(std::string_view                           name,
                                          const std::filesystem::path               &path,
                                          const std::shared_ptr<st::video::texture> &texture) noexcept :
    resource_handle(name,
                    path,
                    texture)
{
}

const st::video::texture_handle *
st::video::texture_handle::get(std::string_view name) noexcept
{
    if (current_registry == nullptr)
        return nullptr;
    return current_registry->find(name);
}

void
st::video::texture_handle::set_registry(const texture_registry *registry) noexcept
{
    current_registry = registry;
}

st::uint32
st::video::texture_handle::width() const noexcept
{
    return shared()->width();
}

st::uint32
st::video::texture_handle::height() const noexcept
{
    return shared()->height();
}
