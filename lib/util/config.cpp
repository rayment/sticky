/*
 * config.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <cmath>
#include <fstream>
#include <system_error>
#include <unordered_set>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include "io/filesystem.h"
#include "io/log.h"

#include "util/config.h"

namespace
{
    constexpr int json_indent = 4;

    [[nodiscard]] std::optional<st::util::config::value> to_value(const nlohmann::json &json) noexcept
    {
        if (json.is_string())
            return json.get<std::string>();
        if (json.is_boolean())
            return json.get<st::bool_t>();
        if (json.is_number())
            return json.get<st::float64>();
        return std::nullopt;
    }

    [[nodiscard]] nlohmann::json to_json(const st::util::config::value &setting)
    {
        if (const auto *text = std::get_if<std::string>(&setting))
            return *text;
        if (const auto *flag = std::get_if<st::bool_t>(&setting))
            return *flag;
        return std::get<st::float64>(setting);
    }
} // namespace

struct st::util::config::config_impl
{
    nlohmann::json                  m_data = nlohmann::json::object();
    std::unordered_set<std::string> m_declared;
};

st::util::config::config() noexcept : m_impl{std::make_unique<config_impl>()} {}

st::util::config::~config() noexcept {}

st::bool_t
st::util::config::load(const std::filesystem::path &path) noexcept
{
    m_impl->m_data = nlohmann::json::object();

    if (!st::io::filesystem::exists(path))
        return true;

    std::ifstream file(path);
    if (!file.is_open())
    {
        st::io::log::error(fmt::format("Could not open config file '{}'.", path.u8string()));
        return false;
    }

    nlohmann::json parsed = nlohmann::json::parse(file, nullptr, false);
    if (!parsed.is_object())
    {
        st::io::log::error(fmt::format("Config file '{}' is not a valid JSON object.", path.u8string()));
        return false;
    }
    m_impl->m_data = std::move(parsed);
    return true;
}

st::bool_t
st::util::config::save(const std::filesystem::path &path) const noexcept
{
    // Written next to the target and then moved over it, so that a failed write never loses the previous settings.
    std::filesystem::path temporary = path;
    temporary += ".tmp";
    {
        std::ofstream file(temporary, std::ios::trunc);
        if (file.is_open())
            file << m_impl->m_data.dump(json_indent, ' ', false, nlohmann::json::error_handler_t::replace) << '\n';
        file.close();
        if (!file.fail())
        {
            std::error_code ec;
            std::filesystem::rename(temporary, path, ec);
            if (!ec)
                return true;
        }
    }
    std::error_code ignored;
    std::filesystem::remove(temporary, ignored);
    st::io::log::error(fmt::format("Could not write config file '{}'.", path.u8string()));
    return false;
}

std::optional<st::util::config::value>
st::util::config::get(std::string_view key) const noexcept
{
    const auto it = m_impl->m_data.find(std::string(key));
    if (it == m_impl->m_data.end())
        return std::nullopt;
    return to_value(*it);
}

void
st::util::config::set(std::string_view key,
                      const value     &setting) noexcept
{
    m_impl->m_data[std::string(key)] = to_json(setting);
}

st::bool_t
st::util::config::set_default(std::string_view key,
                              const value     &fallback) noexcept
{
    const auto current = get(key);
    if (current && current->index() == fallback.index())
        return false;
    set(key, fallback);
    return true;
}

st::bool_t
st::util::config::declare(std::string_view key,
                          const value     &fallback) noexcept
{
    if (!m_impl->m_declared.emplace(key).second)
        return false;
    static_cast<void>(set_default(key, fallback));
    return true;
}

st::bool_t
st::util::config::is_declared(std::string_view key) const noexcept
{
    return m_impl->m_declared.count(std::string(key)) != 0;
}

struct st::util::config_scope::config_scope_impl
{
    config_scope_impl(config          &config,
                      std::string_view owner) :
        m_config(config),
        m_owner(owner)
    {
    }

    config                         &m_config;
    std::string                     m_owner;
    std::unordered_set<std::string> m_owned;
};

st::util::config_scope::config_scope(config          &config,
                                     std::string_view owner) noexcept :
    m_impl{
        std::make_unique<config_scope_impl>(config,
                                            owner),
    }
{
}

st::util::config_scope::~config_scope() noexcept {}

std::string
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
st::util::config_scope::key_for(std::string_view owner,
                                std::string_view name) noexcept
{
    std::string key(owner);
    if (key.empty() || key.back() != '.')
        key += '.';
    key += name;
    return key;
}

st::bool_t
st::util::config_scope::declare(std::string_view     name,
                                const config::value &fallback) noexcept
{
    const std::string key = key_for(m_impl->m_owner, name);
    if (!m_impl->m_config.declare(key, fallback))
    {
        st::io::log::error(
            fmt::format("Setting '{}' of '{}' is already declared by another owner.", key, m_impl->m_owner));
        return false;
    }
    m_impl->m_owned.insert(key);
    return true;
}

std::optional<st::util::config::value>
st::util::config_scope::get(std::string_view name) const noexcept
{
    return get_from(m_impl->m_owner, name);
}

std::optional<st::util::config::value>
st::util::config_scope::get_from(std::string_view owner,
                                 std::string_view name) const noexcept
{
    const std::string key = key_for(owner, name);
    if (!m_impl->m_config.is_declared(key))
        return std::nullopt;
    return m_impl->m_config.get(key);
}

st::bool_t
st::util::config_scope::set(std::string_view     name,
                            const config::value &setting) noexcept
{
    const std::string key = key_for(m_impl->m_owner, name);
    if (m_impl->m_owned.count(key) == 0)
    {
        st::io::log::error(
            fmt::format("'{}' cannot change setting '{}' because it did not declare it.", m_impl->m_owner, name));
        return false;
    }

    const auto current = m_impl->m_config.get(key);
    if (!current || current->index() != setting.index())
    {
        st::io::log::error(
            fmt::format("Setting '{}' of '{}' was given a value of the wrong type.", name, m_impl->m_owner));
        return false;
    }
    if (const auto *number = std::get_if<st::float64>(&setting); number != nullptr && !std::isfinite(*number))
    {
        st::io::log::error(fmt::format("Setting '{}' of '{}' must be a finite number.", name, m_impl->m_owner));
        return false;
    }

    m_impl->m_config.set(key, setting);
    return true;
}
