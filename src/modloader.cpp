/*
 * modloader.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#include <fstream>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include "io/filesystem.h"
#include "io/log.h"

#include "modloader.h"

struct st::mod::mod_impl
{
    mod_impl(std::string_view name,
             std::string_view display_name,
             std::string_view version,
             std::string_view author,
             std::string_view description,
             std::filesystem::path root_dir) noexcept
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
             std::filesystem::path root_dir) noexcept
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

std::unordered_map<std::string, std::unique_ptr<st::mod>>
st::modloader::load_mods() noexcept
{
    std::unordered_map<std::string, std::unique_ptr<st::mod>> mods;

    st::io::log::info("Loading mods...");
    auto mod_folders = st::io::filesystem::list_subdirs("mod");
    for (auto &mod_folder : mod_folders)
    {
        st::bool_t has_metadata   = st::io::filesystem::exists(std::filesystem::path(mod_folder) / "metadata.json");
        st::bool_t has_entrypoint = st::io::filesystem::exists(std::filesystem::path(mod_folder) / "main.lua");
        if (!has_metadata)
        {
            st::io::log::info(fmt::format("Ignoring mod folder '{}' because no metadata.json file was found.", mod_folder));
            continue;
        }
        if (!has_entrypoint)
        {
            st::io::log::info(fmt::format("Ignoring mod folder '{}' because no main.lua file was found.", mod_folder));
            continue;
        }
        std::ifstream metadata_file((std::filesystem::path(mod_folder) / "metadata.json").string());
        try
        {
            nlohmann::json metadata = nlohmann::json::parse(metadata_file);
            if (!metadata.contains("name")         || !metadata["name"].is_string() ||
                !metadata.contains("display_name") || !metadata["display_name"].is_string() ||
                !metadata.contains("version")      || !metadata["version"].is_string() ||
                !metadata.contains("author")       || !metadata["author"].is_string() ||
                !metadata.contains("description")  || !metadata["description"].is_string()
                )
            {
                throw std::exception();
            }
            std::string mod_name = metadata["name"].get<std::string>();
            if (mods.find(mod_name) != mods.end())
            {
                st::io::log::error(fmt::format("Mod with name '{}' already loaded. Skipping mod in folder '{}'.", mod_name, mod_folder));
                continue;
            }
            st::mod *mod = new st::mod(
                mod_name,
                metadata["display_name"].get<std::string>(),
                metadata["version"].get<std::string>(),
                metadata["author"].get<std::string>(),
                metadata["description"].get<std::string>(),
                mod_folder
            );
            mods.emplace(mod_name, std::unique_ptr<st::mod>(mod));
        } catch (...)
        {
            st::io::log::error(fmt::format("Invalid metadata.json file for mod folder '{}'. Skipping mod.", mod_folder));
            continue;
        }
    }
    st::io::log::info("Finished loading mods.");

    return mods;
}
