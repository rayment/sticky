/*
 * config.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <fstream>

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
    nlohmann::json m_data = nlohmann::json::object();
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
        st::io::log::error(fmt::format("Could not open config file '{}'.", path.string()));
        return false;
    }

    nlohmann::json parsed = nlohmann::json::parse(file, nullptr, false);
    if (!parsed.is_object())
    {
        st::io::log::error(fmt::format("Config file '{}' is not a valid JSON object.", path.string()));
        return false;
    }
    m_impl->m_data = std::move(parsed);
    return true;
}

st::bool_t
st::util::config::save(const std::filesystem::path &path) const noexcept
{
    std::ofstream file(path, std::ios::trunc);
    if (file.is_open())
    {
        file << m_impl->m_data.dump(json_indent, ' ', false, nlohmann::json::error_handler_t::replace) << '\n';
        file.flush();
        if (file.good())
            return true;
    }
    st::io::log::error(fmt::format("Could not write config file '{}'.", path.string()));
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
