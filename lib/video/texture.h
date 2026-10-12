/*
 * texture.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
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

namespace st::video
{
    class texture
    {
    public:
        [[nodiscard]] static std::shared_ptr<texture> load_file(const std::filesystem::path &path) noexcept;

        ~texture() noexcept;

        ST_DELETE_COPY_CTORS(texture);
        ST_DELETE_MOVE_CTORS(texture);

        [[nodiscard]] st::uint32 width() const noexcept;
        [[nodiscard]] st::uint32 height() const noexcept;
        [[nodiscard]] st::uint32 gl_handle() const noexcept;
        [[nodiscard]] st::uint64 imgui_id() const noexcept;

    private:
        struct texture_impl;

        explicit texture(std::unique_ptr<texture_impl> impl) noexcept;

        std::unique_ptr<texture_impl> m_impl;
    };
} // namespace st::video

#endif // STICKY_VIDEO_TEXTURE_H
