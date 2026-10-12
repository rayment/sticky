/*
 * texture_handle.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#ifndef STICKY_VIDEO_TEXTURE_HANDLE_H
#define STICKY_VIDEO_TEXTURE_HANDLE_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <filesystem>
#include <memory>
#include <string_view>

#include "resource/resource_handle.h"
#include "resource/resource_registry.h"
#include "st_types.h"
#include "video/texture.h"

namespace st::video
{
    class texture_handle;

    typedef st::resource_registry<texture_handle> texture_registry;

    // A named texture loaded during the init phase of a mod.
    class texture_handle : public st::resource_handle<st::video::texture>
    {
    public:
        texture_handle(std::string_view                           name,
                       const std::filesystem::path               &path,
                       const std::shared_ptr<st::video::texture> &texture) noexcept;

        // Looks up a texture in the registry of the executing mod, or nullptr.
        [[nodiscard]] static const texture_handle *get(std::string_view name) noexcept;

        // Selects the registry used by `get`.
        static void set_registry(const texture_registry *registry) noexcept;

        [[nodiscard]] st::uint32 width() const noexcept;
        [[nodiscard]] st::uint32 height() const noexcept;
    };

} // namespace st::video

#endif // STICKY_VIDEO_TEXTURE_HANDLE_H
