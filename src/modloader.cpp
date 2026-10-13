/*
 * modloader.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#include <algorithm>
#include <fstream>
#include <optional>
#include <utility>
#include <vector>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include "io/filesystem.h"
#include "io/log.h"
#include "os/popup.h"

#include "modloader.h"

namespace
{
    constexpr st::size max_name_length = 64;

    struct metadata
    {
        std::string name;
        std::string display_name;
        std::string version;
        std::string author;
        std::string description;
    };

    // Reads mod.json, logging why it is unusable.
    [[nodiscard]] std::optional<metadata> read_metadata(const std::filesystem::path &folder) noexcept
    {
        const std::string    folder_name = folder.u8string();
        std::ifstream        file(folder / "mod.json");
        const nlohmann::json json = nlohmann::json::parse(file, nullptr, false);
        if (!file.is_open() || json.is_discarded() || !json.is_object())
        {
            st::io::log::error(
                fmt::format("Skipping mod in folder '{}': mod.json is unreadable or not a JSON object.", folder_name));
            return std::nullopt;
        }

        metadata                                     result;
        const std::pair<const char *, std::string *> fields[] = {
            {"name", &result.name},     {"display_name", &result.display_name}, {"version", &result.version},
            {"author", &result.author}, {"description", &result.description},
        };
        for (const auto &[key, out] : fields)
        {
            const auto it = json.find(key);
            if (it == json.end() || !it->is_string())
            {
                st::io::log::error(
                    fmt::format("Skipping mod in folder '{}': mod.json needs a string '{}'.", folder_name, key));
                return std::nullopt;
            }
            *out = it->get<std::string>();
        }

        if (!st::modloader::is_valid_name(result.name))
        {
            st::io::log::error(fmt::format("Skipping mod in folder '{}': '{}' is not a valid mod name. Use 1 to {} "
                                           "lowercase letters, digits or underscores.",
                                           folder_name, result.name, max_name_length));
            return std::nullopt;
        }
        return result;
    }

    [[nodiscard]] st::bool_t has_entrypoints(const std::filesystem::path &folder) noexcept
    {
        if (!st::io::filesystem::exists(folder / "mod.json"))
        {
            st::io::log::info(
                fmt::format("Ignoring mod folder '{}' because no mod.json file was found.", folder.u8string()));
            return false;
        }
        for (const char *entrypoint : {"init.lua", "runtime.lua"})
        {
            if (!st::io::filesystem::is_file(folder / "lua" / entrypoint))
            {
                st::io::log::info(fmt::format("Ignoring mod folder '{}' because no {} file was found.",
                                              folder.u8string(), entrypoint));
                return false;
            }
        }
        return true;
    }
} // namespace

st::modloader::modloader() noexcept {}

st::modloader::~modloader() noexcept {}

st::bool_t
st::modloader::is_valid_name(std::string_view name) noexcept
{
    if (name.empty() || name.size() > max_name_length)
        return false;
    return std::all_of(name.begin(), name.end(),
                       [](const char c) { return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'; });
}

st::mod_map
st::modloader::load_mods() noexcept
{
    mod_map                  mods;
    std::vector<std::string> duplicates;

    st::io::log::info("Loading mods...");
    // Sorted, so that the same folder wins a name clash on every platform.
    auto folders = st::io::filesystem::list_subdirs("mod");
    std::sort(folders.begin(), folders.end());
    for (const auto &folder_name : folders)
    {
        const std::filesystem::path folder = std::filesystem::u8path(folder_name);
        if (!has_entrypoints(folder))
            continue;

        const auto meta = read_metadata(folder);
        if (!meta)
            continue;

        if (mods.find(meta->name) != mods.end())
        {
            st::io::log::warn(fmt::format("Mod with name '{}' already loaded. Skipping mod in folder '{}'.", meta->name,
                                          folder_name));
            duplicates.push_back(folder_name);
            continue;
        }
        mods.emplace(meta->name,
                     std::make_unique<st::mod>(meta->name, meta->display_name, meta->version, meta->author,
                                               meta->description, folder / "lua"));
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
