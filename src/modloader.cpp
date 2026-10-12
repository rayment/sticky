/*
 * modloader.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#include <exception>
#include <fstream>
#include <stdexcept>
#include <vector>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include "io/filesystem.h"
#include "io/log.h"
#include "os/popup.h"

#include "modloader.h"

st::modloader::modloader() noexcept {}

st::modloader::~modloader() noexcept {}

using mod_map = std::unordered_map<std::string, std::unique_ptr<st::mod>>;

static std::unique_ptr<st::mod>
load_mod(const std::string        &mod_folder,
         const mod_map            &loaded,
         std::vector<std::string> &duplicates) noexcept
{
    try
    {
        const std::filesystem::path folder(mod_folder);
        if (!st::io::filesystem::exists(folder / "mod.json"))
        {
            st::io::log::info(fmt::format("Ignoring mod folder '{}' because no mod.json file was found.", mod_folder));
            return nullptr;
        }
        for (const char *entrypoint : {"init.lua", "runtime.lua"})
        {
            if (!st::io::filesystem::exists(folder / "lua" / entrypoint))
            {
                st::io::log::info(
                    fmt::format("Ignoring mod folder '{}' because no {} file was found.", mod_folder, entrypoint));
                return nullptr;
            }
        }

        std::ifstream        metadata_file((folder / "mod.json").string());
        const nlohmann::json metadata = nlohmann::json::parse(metadata_file);
        for (const char *key : {"name", "display_name", "version", "author", "description"})
        {
            if (!metadata.contains(key) || !metadata[key].is_string())
                throw std::runtime_error(fmt::format("missing or non-string key '{}'", key));
        }
        const std::string name = metadata["name"].get<std::string>();
        if (loaded.find(name) != loaded.end())
        {
            st::io::log::warn(
                fmt::format("Mod with name '{}' already loaded. Skipping mod in folder '{}'.", name, mod_folder));
            duplicates.push_back(mod_folder);
            return nullptr;
        }
        return std::make_unique<st::mod>(name, metadata["display_name"].get<std::string>(),
                                         metadata["version"].get<std::string>(), metadata["author"].get<std::string>(),
                                         metadata["description"].get<std::string>(), folder / "lua");
    }
    catch (const std::exception &e)
    {
        st::io::log::error(
            fmt::format("Skipping mod in folder '{}': invalid or unreadable mod.json ({}).", mod_folder, e.what()));
    }
    catch (...)
    {
        st::io::log::error(
            fmt::format("Skipping mod in folder '{}': unknown error while reading mod.json.", mod_folder));
    }
    return nullptr;
}

std::unordered_map<std::string,
                   std::unique_ptr<st::mod>>
st::modloader::load_mods() noexcept
{
    mod_map                  mods;
    std::vector<std::string> duplicates;

    try
    {
        st::io::log::info("Loading mods...");
        for (const auto &mod_folder : st::io::filesystem::list_subdirs("mod"))
        {
            auto loaded = load_mod(mod_folder, mods, duplicates);
            if (!loaded)
                continue;
            const std::string mod_name = loaded->name();
            mods.emplace(mod_name, std::move(loaded));
        }
    }
    catch (...)
    {
        st::io::log::error("Failed while loading mods. Remaining mods were skipped.");
    }
    st::io::log::info("Finished loading mods.");

    for (const auto &folder : duplicates)
    {
        st::os::popup::warn(
            "Mod not loaded",
            fmt::format(
                "The mod from folder '{}' was not loaded because another mod with the same name was already loaded.",
                folder));
    }

    return mods;
}
