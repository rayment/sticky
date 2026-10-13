/*
 * texture_handle.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief Named textures that a mod declared in its init phase.
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

/** @addtogroup video
 *  @{
 */

namespace st::video
{
    class texture_handle;

    /** @brief The textures of one mod, by name. */
    typedef st::resource_registry<texture_handle> texture_registry;

    /** @brief A texture loaded from the `st.init.textures` table of a mod. Lua sees it as `st.video.texture`. */
    class texture_handle : public st::resource_handle<st::video::texture>
    {
    public:
        /**
         * @brief Wraps a loaded texture.
         * @param name Unique name of the texture within its mod.
         * @param path Path the texture was loaded from, relative to the `res` folder of the mod.
         * @param texture The texture.
         */
        texture_handle(std::string_view                           name,
                       const std::filesystem::path               &path,
                       const std::shared_ptr<st::video::texture> &texture) noexcept;

        /** @brief Width of the texture in pixels. */
        [[nodiscard]] st::uint32 width() const noexcept;
        /** @brief Height of the texture in pixels. */
        [[nodiscard]] st::uint32 height() const noexcept;
    };

} // namespace st::video

/** @} */

#endif // STICKY_VIDEO_TEXTURE_HANDLE_H
