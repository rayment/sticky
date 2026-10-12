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

#include <utility>

#include "io/log.h"
#include "lua/runtime.h"

extern "C" int luaopen_sticky(lua_State *L);

struct st::lua::runtime::runtime_impl
{
    runtime_impl(std::string_view name, std::filesystem::path root_dir) noexcept
        : logger_name(name), lua_logger(name), root_dir_path(std::move(root_dir))
    {}
    ~runtime_impl() noexcept = default;

    std::string logger_name;
    st::io::log::logger lua_logger;
    std::filesystem::path root_dir_path;
    lua_State *L;
};

[[nodiscard]]
static bool
is_within_directory(const std::filesystem::path &root,
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
static st::bool_t
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
resolve_path(const std::filesystem::path &root,
             const std::filesystem::path &requested,
             std::filesystem::path &resolved) noexcept
{
    std::error_code ec;

    const auto canonical_root = std::filesystem::canonical(root, ec);
    if (ec)
        return false;

    const auto candidate = requested.is_absolute()
        ? requested
        : canonical_root / requested;

    resolved = std::filesystem::canonical(candidate, ec);

    return !ec && is_within_directory(canonical_root, resolved);
}

[[nodiscard]]
static st::bool_t
resolve_file(const std::filesystem::path &root,
             const std::filesystem::path &requested,
             std::filesystem::path &resolved) noexcept
{
    if (!resolve_path(root, requested, resolved))
        return false;

    std::error_code ec;
    return std::filesystem::is_regular_file(resolved, ec) && !ec;
}

[[nodiscard]]
static bool
is_valid_module_name(const std::string &name) noexcept
{
    if (name.empty() || name.front() == '.' || name.back() == '.')
        return false;
    bool previous_dot = false;
    for (const unsigned char c : name)
    {
        if (c == '.')
        {
            if (previous_dot)
                return false;
            previous_dot = true;
            continue;
        }
        const bool is_alnum = (c >= 'a' && c <= 'z') ||
                              (c >= 'A' && c <= 'Z') ||
                              (c >= '0' && c <= '9');
        if (!is_alnum && c != '_')
            return false;
        previous_dot = false;
    }
    return true;
}

static void
load_std_libraries(lua_State *L)
{
    luaL_requiref(L, LUA_GNAME, luaopen_base, 1);
    lua_pop(L, 1);
    luaL_requiref(L, LUA_COLIBNAME, luaopen_coroutine, 1);
    lua_pop(L, 1);
    luaL_requiref(L, LUA_TABLIBNAME, luaopen_table, 1);
    lua_pop(L, 1);
    luaL_requiref(L, LUA_STRLIBNAME, luaopen_string, 1);
    lua_pop(L, 1);
    luaL_requiref(L, LUA_UTF8LIBNAME, luaopen_utf8, 1);
    lua_pop(L, 1);
    luaL_requiref(L, LUA_MATHLIBNAME, luaopen_math, 1);
    lua_pop(L, 1);

    constexpr const char *unsafe_globals[] = {
        "dofile",
        "loadfile",
        "load",
        "collectgarbage",
    };

    for (const char *name : unsafe_globals)
    {
        lua_pushnil(L);
        lua_setglobal(L, name);
    }
}

static st::char_t require_cache_key;

static int
require_mod(lua_State *L)
{
    const char *name_arg = luaL_checkstring(L, 1);
    const std::string name(name_arg);

    if (!is_valid_module_name(name))
        return luaL_error(L, "invalid module name '%s'", name.c_str());

    // Retrieve the module cache from the Lua registry.
    lua_rawgetp(L, LUA_REGISTRYINDEX, &require_cache_key);
    const int cache_index = lua_gettop(L);

    // Return the cached module, if present.
    lua_pushlstring(L, name.data(), name.size());
    lua_rawget(L, cache_index);

    if (!lua_isnil(L, -1))
    {
        lua_remove(L, cache_index);
        return 1;
    }

    lua_pop(L, 1);

    const char *root_arg = lua_tostring(L, lua_upvalueindex(1));
    if (root_arg == nullptr)
    {
        lua_pop(L, 1);
        return luaL_error(L, "missing Lua root directory");
    }

    std::string relative_name = name;
    for (char &c : relative_name)
    {
        if (c == '.')
            c = '/';
    }
    relative_name += ".lua";

    std::filesystem::path candidate;
    if (!resolve_file(root_arg, relative_name, candidate))
    {
        lua_pop(L, 1);
        return luaL_error(
            L, "module '%s' could not be resolved within the Lua directory",
            name.c_str());
    }

    if (luaL_loadfile(L, candidate.string().c_str()) != LUA_OK)
    {
        lua_remove(L, cache_index);
        return lua_error(L);
    }

    if (lua_pcall(L, 0, 1, 0) != LUA_OK)
    {
        lua_remove(L, cache_index);
        return lua_error(L);
    }

    if (lua_isnil(L, -1))
    {
        lua_pop(L, 1);
        lua_pushboolean(L, 1);
    }

    lua_pushlstring(L, name.data(), name.size());
    lua_pushvalue(L, -2);
    lua_rawset(L, cache_index);

    lua_remove(L, cache_index);
    return 1;
}

st::lua::runtime::runtime(std::string_view name,
                          const std::filesystem::path &root_dir) noexcept
    : m_impl{std::make_unique<runtime_impl>(name, root_dir)}
{
    st::lua::set_logger(&m_impl->lua_logger);

    m_impl->L = luaL_newstate();
    ST_ASSERT(m_impl->L);

    load_std_libraries(m_impl->L);

    const auto lua_root = std::filesystem::canonical(root_dir);
    lua_newtable(m_impl->L);
    lua_rawsetp(m_impl->L, LUA_REGISTRYINDEX, &require_cache_key);
    lua_pushstring(m_impl->L, lua_root.generic_string().c_str());
    lua_pushcclosure(m_impl->L, require_mod, 1);
    lua_setglobal(m_impl->L, "require");

    lua_pushnil(m_impl->L);
    lua_setglobal(m_impl->L, "print");

    luaL_requiref(m_impl->L, "sticky", luaopen_sticky, 0);
    lua_setglobal(m_impl->L, "st");
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
    lua_getglobal(m_impl->L, std::string(name).c_str());
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

    st::lua::set_logger(&m_impl->lua_logger);

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
