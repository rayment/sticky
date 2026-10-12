/*
 * texture_handle.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include "video/texture_handle.h"

st::video::texture_handle::texture_handle(std::string_view                           name,
                                          const std::filesystem::path               &path,
                                          const std::shared_ptr<st::video::texture> &texture) noexcept :
    resource_handle(name,
                    path,
                    texture)
{
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
