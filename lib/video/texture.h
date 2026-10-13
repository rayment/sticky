/*
 * texture.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief Images uploaded to the GPU.
 */

#ifndef STICKY_VIDEO_TEXTURE_H
#define STICKY_VIDEO_TEXTURE_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <filesystem>
#include <memory>

#include "st_macros.h"
#include "st_types.h"

/** @addtogroup video
 *  @{
 */

namespace st::video
{
    /**
     * @brief An RGBA image in an OpenGL texture.
     *
     * Requires the OpenGL context of a st::video::window to be current, both to load and to destroy it.
     */
    class texture
    {
    public:
        /**
         * @brief Loads a PNG. Any bit depth, with or without alpha, is converted to 8-bit RGBA.
         * @param path The file.
         * @return The texture, or null if the file is missing, not a PNG or could not be uploaded. The reason is
         * logged.
         */
        [[nodiscard]] static std::shared_ptr<texture> load_file(const std::filesystem::path &path) noexcept;

        ~texture() noexcept;

        ST_DELETE_COPY_CTORS(texture);
        ST_DELETE_MOVE_CTORS(texture);

        /** @brief Width in pixels. */
        [[nodiscard]] st::uint32 width() const noexcept;
        /** @brief Height in pixels. */
        [[nodiscard]] st::uint32 height() const noexcept;
        /** @brief The OpenGL texture name. */
        [[nodiscard]] st::uint32 gl_handle() const noexcept;
        /** @brief The identifier the GUI backend draws the texture with. */
        [[nodiscard]] st::uint64 imgui_id() const noexcept;

    private:
        struct texture_impl;

        explicit texture(std::unique_ptr<texture_impl> impl) noexcept;

        std::unique_ptr<texture_impl> m_impl;
    };
} // namespace st::video

/** @} */

#endif // STICKY_VIDEO_TEXTURE_H
