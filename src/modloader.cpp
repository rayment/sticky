/*
 * modloader.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#include <fstream>
#include <stdexcept>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include "io/filesystem.h"
#include "io/log.h"

#include "modloader.h"

st::modloader::modloader() noexcept {}

st::modloader::~modloader() noexcept {}

static std::unique_ptr<st::mod>
load_mod(const std::string &mod_folder) noexcept
{
    try
    {
        const std::filesystem::path folder(mod_folder);
        if (!st::io::filesystem::exists(folder / "mod.json"))
        {
            st::io::log::info(fmt::format("Ignoring mod folder '{}' because no metadata.json file was found.", mod_folder));
            return nullptr;
        }
        for (const char *entrypoint : {"init.lua", "runtime.lua"})
        {
            if (!st::io::filesystem::exists(folder / "lua" / entrypoint))
            {
                st::io::log::info(fmt::format("Ignoring mod folder '{}' because no {} file was found.", mod_folder, entrypoint));
                return nullptr;
            }
        }

        std::ifstream metadata_file((folder / "mod.json").string());
        const nlohmann::json metadata = nlohmann::json::parse(metadata_file);
        for (const char *key : {"name", "display_name", "version", "author", "description"})
        {
            if (!metadata.contains(key) || !metadata[key].is_string())
                throw std::runtime_error(key);
        }
        return std::make_unique<st::mod>(
            metadata["name"].get<std::string>(),
            metadata["display_name"].get<std::string>(),
            metadata["version"].get<std::string>(),
            metadata["author"].get<std::string>(),
            metadata["description"].get<std::string>(),
            folder / "lua");
    }
    catch (...)
    {
        st::io::log::error("Skipping mod with invalid or unreadable metadata.json in folder:");
        st::io::log::error(mod_folder);
    }
    return nullptr;
}

std::unordered_map<std::string, std::unique_ptr<st::mod>>
st::modloader::load_mods() noexcept
{
    std::unordered_map<std::string, std::unique_ptr<st::mod>> mods;

    try
    {
        st::io::log::info("Loading mods...");
        for (const auto &mod_folder : st::io::filesystem::list_subdirs("mod"))
        {
            auto loaded = load_mod(mod_folder);
            if (!loaded)
                continue;
            const std::string mod_name = loaded->name();
            if (mods.find(mod_name) != mods.end())
            {
                st::io::log::error(fmt::format("Mod with name '{}' already loaded. Skipping mod in folder '{}'.", mod_name, mod_folder));
                continue;
            }
            mods.emplace(mod_name, std::move(loaded));
        }
    }
    catch (...)
    {
        st::io::log::error("Failed while loading mods. Remaining mods were skipped.");
    }
    st::io::log::info("Finished loading mods.");

    return mods;
}
