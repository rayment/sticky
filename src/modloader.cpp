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

struct st::mod::mod_impl
{
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    mod_impl(std::string_view name,
             std::string_view display_name,
             std::string_view version,
             std::string_view author,
             std::string_view description,
             const std::filesystem::path &root_dir) noexcept
        : m_name(name),
          m_display_name(display_name),
          m_version(version),
          m_author(author),
          m_description(description),
          m_runtime(name, root_dir)
    {}
    ~mod_impl() noexcept = default;

    std::string      m_name;
    std::string      m_display_name;
    std::string      m_version;
    std::string      m_author;
    std::string      m_description;
    st::lua::runtime m_runtime;
};

st::mod::mod(std::string_view name,
             std::string_view display_name,
             std::string_view version,
             std::string_view author,
             std::string_view description,
             const std::filesystem::path &root_dir) noexcept
    : m_impl{std::make_unique<mod_impl>(name, display_name, version, author, description, root_dir)}
{}

st::mod::~mod() noexcept
{}

std::string
st::mod::author() noexcept
{
    return m_impl->m_author;
}

std::string
st::mod::description() noexcept
{
    return m_impl->m_description;
}

std::string
st::mod::display_name() noexcept
{
    return m_impl->m_display_name;
}

std::string
st::mod::name() noexcept
{
    return m_impl->m_name;
}

st::lua::runtime &
st::mod::runtime() noexcept
{
    return m_impl->m_runtime;
}

std::string
st::mod::version() noexcept
{
    return m_impl->m_version;
}

st::modloader::modloader() noexcept
{}

st::modloader::~modloader() noexcept
{}

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
        if (!st::io::filesystem::exists(folder / "lua" / "main.lua"))
        {
            st::io::log::info(fmt::format("Ignoring mod folder '{}' because no main.lua file was found.", mod_folder));
            return nullptr;
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
