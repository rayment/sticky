/*
 * runtime.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include "log_bindings.h"
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "io/log.h"
#include "lua/runtime.h"

extern "C" int luaopen_sticky(lua_State *L);

struct st::lua::runtime::runtime_impl
{
    runtime_impl(std::string_view name, std::filesystem::path root_dir) noexcept
        : logger_name(name), lua_logger(name), root_dir_path(root_dir)
    {}
    ~runtime_impl() noexcept = default;

    std::string logger_name;
    st::io::log::logger lua_logger;
    std::filesystem::path root_dir_path;
    lua_State *L;
};

[[nodiscard]]
static bool is_within_directory(
    const std::filesystem::path &root,
    const std::filesystem::path &candidate) noexcept
{
    auto root_it = root.begin();
    auto candidate_it = candidate.begin();

    for (; root_it != root.end(); ++root_it, ++candidate_it)
    {
        if (candidate_it == candidate.end() || *root_it != *candidate_it)
            return false;
    }

    return true;
}

[[nodiscard]]
static bool resolve_file(
    const std::filesystem::path &root,
    const std::filesystem::path &requested,
    std::filesystem::path &resolved) noexcept
{
    std::error_code ec;

    const std::filesystem::path canonical_root = std::filesystem::canonical(root, ec);
    if (ec)
        return false;

    const auto candidate = requested.is_absolute()
        ? requested
        : canonical_root / requested;

    resolved = std::filesystem::canonical(candidate, ec);

    if (ec || !is_within_directory(canonical_root, resolved))
        return false;

    if (!std::filesystem::is_regular_file(resolved, ec) || ec)
        return false;

    return true;
}

st::lua::runtime::runtime(std::string_view name,
                          std::filesystem::path root_dir) noexcept
    : m_impl{std::make_unique<runtime_impl>(name, root_dir)}
{
    st::lua::set_logger(&m_impl->lua_logger);

    m_impl->L = luaL_newstate();
    ST_ASSERT(m_impl->L);
    luaL_openlibs(m_impl->L);
    lua_pushnil(m_impl->L);
    lua_setglobal(m_impl->L, "print");
    luaL_requiref(m_impl->L, "sticky", luaopen_sticky, 1);
    luaL_loadstring(m_impl->L, "st = require('sticky')");
    lua_pcallk(m_impl->L, 0, -1, 0, 0, nullptr);
    lua_pop(m_impl->L, 1);

    // Change lua package path to directory that file belongs to.
    lua_getglobal(m_impl->L, "package");
    lua_pushfstring(
        m_impl->L,
        "%s/?.lua;%s/?/init.lua",
        root_dir.generic_string().c_str(), root_dir.generic_string().c_str());
    lua_setfield(m_impl->L, -2, "path");

    // Disable native module loading.
    lua_pushstring(m_impl->L, "");
    lua_setfield(m_impl->L, -2, "cpath");
    lua_pop(m_impl->L, 1);
}

st::lua::runtime::~runtime() noexcept
{
    if (st::lua::logger() == &m_impl->lua_logger)
        st::lua::set_logger(nullptr);
}

st::bool_t
st::lua::runtime::execute_file(const std::filesystem::path &path) noexcept
{
    std::filesystem::path resolved;

    if (!resolve_file(m_impl->root_dir_path, path, resolved))
        return false;

    st::lua::set_logger(&m_impl->lua_logger);

    if (luaL_loadfile(m_impl->L, resolved.generic_string().c_str()) != LUA_OK)
    {
        luaL_traceback(m_impl->L, m_impl->L, lua_tostring(m_impl->L, -1), 1);
        st::io::log::error(lua_tostring(m_impl->L, -1));
        lua_pop(m_impl->L, 2);
        return false;
    }
    if (lua_pcallk(m_impl->L, 0, 0, 0, 0, nullptr) != LUA_OK)
    {
        luaL_traceback(m_impl->L, m_impl->L, lua_tostring(m_impl->L, -1), 1);
        st::io::log::error(lua_tostring(m_impl->L, -1));
        lua_pop(m_impl->L, 2);
        return false;
    }
    return true;
}

st::bool_t
st::lua::runtime::execute_function(std::string_view name) noexcept
{
    return execute_function(name, {});
}

st::bool_t
st::lua::runtime::execute_function(std::string_view name,
                                   std::initializer_list<const lua_arg_type> args) noexcept
{
    lua_getglobal(m_impl->L, name.data());
    if (!lua_isfunction((m_impl->L), -1))
    {
        lua_pop(m_impl->L, 1);
        return false;
    }

    for (const auto &arg : args)
    {
        std::visit([this](const auto &value)
        {
            using type = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<type, std::monostate>)
                lua_pushnil(m_impl->L);
            else if constexpr (std::is_same_v<type, st::bool_t>)
                lua_pushboolean(m_impl->L, value);
            else if constexpr (std::is_same_v<type, st::int64>)
                lua_pushinteger(m_impl->L, value);
            else if constexpr (std::is_same_v<type, st::float64>)
                lua_pushnumber(m_impl->L, value);
            else if constexpr (std::is_same_v<type, std::string_view>)
                lua_pushlstring(m_impl->L, value.data(), value.size());
        }, arg);
    }

    if (lua_pcall(m_impl->L, static_cast<st::int32>(args.size()), 0, 0) != LUA_OK)
    {
        luaL_traceback(m_impl->L, m_impl->L, lua_tostring(m_impl->L, -1), 1);
        st::io::log::error(lua_tostring(m_impl->L, -1));
        lua_pop(m_impl->L, 2);
        return false;
    }
    return true;
}

st::bool_t
st::lua::runtime::execute_string(ST_UNUSED std::string_view code) noexcept
{
    // TODO: Implement
    return true;
}
