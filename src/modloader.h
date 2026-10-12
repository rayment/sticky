/*
 * modloader.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#ifndef STICKY_MODLOADER_H
#define STICKY_MODLOADER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "mod.h"

namespace st
{

    class modloader
    {
    public:
        modloader() noexcept;
        ~modloader() noexcept;

        [[nodiscard]] std::unordered_map<std::string,
                                         std::unique_ptr<st::mod>>
        load_mods() noexcept;
    };

} // namespace st

#endif // STICKY_MODLOADER_H
