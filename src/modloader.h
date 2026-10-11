/*
 * modloader.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#ifndef STICKY_MODLOADER_H
#define STICKY_MODLOADER_H

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

#include "lua/runtime.h"

namespace st
{

    class mod
    {
    public:
         mod(std::string_view name,
             std::string_view display_name,
             std::string_view version,
             std::string_view author,
             std::string_view description,
             std::filesystem::path root_dir) noexcept;
        ~mod() noexcept;

        ST_DELETE_COPY_CTORS(mod);
        ST_DELETE_MOVE_CTORS(mod);

        [[nodiscard]] std::string       author      () noexcept;
        [[nodiscard]] std::string       description () noexcept;
        [[nodiscard]] std::string       display_name() noexcept;
        [[nodiscard]] std::string       name        () noexcept;
        [[nodiscard]] st::lua::runtime &runtime     () noexcept;
        [[nodiscard]] std::string       version     () noexcept;

    private:
        struct mod_impl;
        std::unique_ptr<mod_impl> m_impl;
    };

    class modloader
    {
    public:
         modloader() noexcept;
        ~modloader() noexcept;

        [[nodiscard]] std::unordered_map<std::string, std::unique_ptr<st::mod>> load_mods() noexcept;
    };

}

#endif // STICKY_MODLOADER_H
