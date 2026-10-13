/*
 * modloader.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

/**
 * @file
 * @brief Discovery of the mods in the `mod` folder.
 */

#ifndef STICKY_MODLOADER_H
#define STICKY_MODLOADER_H

#include <map>
#include <memory>
#include <string>
#include <string_view>

#include "mod.h"

/** @addtogroup app
 *  @{
 */

namespace st
{

    /** @brief The loaded mods, by name. Phases and callbacks run in this order, sorted by name. */
    typedef std::map<std::string, std::unique_ptr<st::mod>, std::less<>> mod_map;

    /**
     * @brief Finds and creates the mods in the `mod` folder of the working directory.
     *
     * A mod is a folder with a `mod.json` and the files `lua/init.lua` and `lua/runtime.lua`. `mod.json` is an object
     * with the string keys `name`, `display_name`, `version`, `author` and `description`.
     */
    class modloader
    {
    public:
        modloader() noexcept;
        ~modloader() noexcept;

        /**
         * @brief Loads every valid mod. Folders that are not valid mods are skipped and logged.
         *
         * A mod whose name is already taken is skipped, and the user is told with a popup.
         *
         * @return The mods, by name. None of their phases have run.
         */
        [[nodiscard]] mod_map load_mods() noexcept;

        /**
         * @brief Whether a string can name a mod: 1 to 64 characters, each a lowercase letter, a digit or `_`.
         * @param name Candidate name.
         */
        [[nodiscard]] static st::bool_t is_valid_name(std::string_view name) noexcept;
    };

} // namespace st

/** @} */

#endif // STICKY_MODLOADER_H
