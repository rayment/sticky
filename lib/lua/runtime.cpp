/*
 * runtime.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include "log_bindings.h"
extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <algorithm>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <utility>

#include <fmt/format.h>

#include "io/filesystem.h"
#include "io/log.h"
#include "lua/runtime.h"

extern "C" int luaopen_sticky(lua_State *L);

struct st::lua::runtime::runtime_impl
{
    runtime_impl(std::string_view      name,
                 std::filesystem::path root_dir) noexcept :
        logger_name(name),
        lua_logger(name),
        root_dir_path(std::move(root_dir))
    {
    }
    ~runtime_impl() noexcept = default;

    std::string           logger_name;
    st::io::log::logger   lua_logger;
    std::filesystem::path root_dir_path;
    lua_State            *L;
};

namespace
{
    // The only globals scripts get: the Lua libraries, minus the ones that read or run files, the sandboxed
    // `require`, and the engine module. What `st` holds is decided by the SWIG bindings.
    constexpr const char *permitted_globals[] = {
        "_G",        "_VERSION",     "assert",   "error",    "getmetatable", "ipairs",  "next",
        "pairs",     "pcall",        "print",    "rawequal", "rawget",       "rawlen",  "rawset",
        "select",    "setmetatable", "tonumber", "tostring", "type",         "warn",    "xpcall",
        "coroutine", "math",         "string",   "table",    "utf8",         "require", "st",
    };

    [[nodiscard]] st::bool_t is_permitted_global(const char *name) noexcept
    {
        return std::any_of(std::begin(permitted_globals), std::end(permitted_globals),
                           [name](const char *entry) { return std::strcmp(entry, name) == 0; });
    }
} // namespace

// Removes every global that is not permitted, such as the SWIG helper functions.
static void
remove_unpermitted_globals(lua_State *L) noexcept
{
    lua_pushglobaltable(L);
    const int table = lua_gettop(L);

    lua_pushnil(L);
    while (lua_next(L, table) != 0)
    {
        lua_pop(L, 1);
        if (lua_type(L, -1) == LUA_TSTRING && is_permitted_global(lua_tostring(L, -1)))
            continue;
        lua_pushvalue(L, -1);
        lua_pushnil(L);
        lua_rawset(L, table);
    }
    lua_pop(L, 1);
}

[[nodiscard]]
static bool
is_valid_module_name(std::string_view name) noexcept
{
    if (name.empty() || name.front() == '.' || name.back() == '.')
        return false;
    bool previous_dot = false;
    for (const char character : name)
    {
        const auto c = static_cast<unsigned char>(character);
        if (c == '.')
        {
            if (previous_dot)
                return false;
            previous_dot = true;
            continue;
        }
        const bool is_alnum = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
        if (!is_alnum && c != '_')
            return false;
        previous_dot = false;
    }
    return true;
}

static void
load_std_libraries(lua_State *L)
{
    const luaL_Reg libraries[] = {
        {LUA_GNAME, luaopen_base},        {LUA_COLIBNAME, luaopen_coroutine}, {LUA_TABLIBNAME, luaopen_table},
        {LUA_STRLIBNAME, luaopen_string}, {LUA_UTF8LIBNAME, luaopen_utf8},    {LUA_MATHLIBNAME, luaopen_math},
    };
    for (const luaL_Reg &library : libraries)
    {
        luaL_requiref(L, library.name, library.func, 1);
        lua_pop(L, 1);
    }
}

static int
traceback_handler(lua_State *L)
{
    const char *message = lua_tostring(L, 1);
    if (message == nullptr)
    {
        if (luaL_callmeta(L, 1, "__tostring") != 0 && lua_type(L, -1) == LUA_TSTRING)
            return 1;
        message = lua_pushfstring(L, "(error object is a %s value)", luaL_typename(L, 1));
    }
    luaL_traceback(L, L, message, 1);
    return 1;
}

static st::bool_t
protected_call(lua_State *L,
               st::int32  arg_count) noexcept
{
    const int base = lua_gettop(L) - arg_count;
    lua_pushcfunction(L, traceback_handler);
    lua_insert(L, base);

    const st::bool_t ok = lua_pcall(L, arg_count, 0, base) == LUA_OK;
    if (!ok)
        st::io::log::error(lua_tostring(L, -1));
    lua_settop(L, base - 1);
    return ok;
}

static st::char_t require_cache_key;

constexpr st::size module_path_capacity = 4096;

// Lua is built as C, so a Lua error is a longjmp. Nothing with a destructor may
// be alive when a Lua API call that can raise is made, which is why the C++
// work is kept in functions like this one that never touch the Lua API.
[[nodiscard]]
static st::bool_t
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
resolve_module(std::string_view name,
               const char      *root,
               char            *out,
               st::size         out_size) noexcept
{
    std::string relative_name(name);
    std::replace(relative_name.begin(), relative_name.end(), '.', '/');
    relative_name += ".lua";

    std::filesystem::path candidate;
    if (!st::io::filesystem::resolve_file(root, relative_name, candidate))
        return false;

    const std::string resolved = candidate.string();
    if (resolved.size() >= out_size)
        return false;

    std::memcpy(out, resolved.c_str(), resolved.size() + 1);
    return true;
}

static int
require_mod(lua_State *L)
{
    st::size    name_length = 0;
    const char *name = luaL_checklstring(L, 1, &name_length);

    if (!is_valid_module_name(std::string_view(name, name_length)))
        return luaL_error(L, "invalid module name '%s'", name);

    // Retrieve the module cache from the Lua registry.
    lua_rawgetp(L, LUA_REGISTRYINDEX, &require_cache_key);
    const int cache_index = lua_gettop(L);

    // Return the cached module, if present.
    lua_pushvalue(L, 1);
    lua_rawget(L, cache_index);

    if (!lua_isnil(L, -1))
        return 1;

    lua_pop(L, 1);

    const char *root = lua_tostring(L, lua_upvalueindex(1));
    if (root == nullptr)
        return luaL_error(L, "missing Lua root directory");

    // The path lives on the Lua heap so that an error below frees it.
    char *path = static_cast<char *>(lua_newuserdatauv(L, module_path_capacity, 0));
    if (!resolve_module(std::string_view(name, name_length), root, path, module_path_capacity))
        return luaL_error(L, "module '%s' could not be resolved within the Lua directory", name);

    if (luaL_loadfilex(L, path, "t") != LUA_OK)
        return lua_error(L);

    lua_call(L, 0, 1);

    if (lua_isnil(L, -1))
    {
        lua_pop(L, 1);
        lua_pushboolean(L, 1);
    }

    lua_pushvalue(L, 1);
    lua_pushvalue(L, -2);
    lua_rawset(L, cache_index);

    return 1;
}

namespace
{
    struct function_call
    {
        std::string_view                                            name;
        std::initializer_list<const st::lua::runtime::lua_arg_type> args;
        st::bool_t                                                  found = false;
    };
} // namespace

static void
push_argument(lua_State                            *L,
              const st::lua::runtime::lua_arg_type &arg)
{
    std::visit(
        [L](const auto &value)
        {
            using type = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<type, std::monostate>)
                lua_pushnil(L);
            else if constexpr (std::is_same_v<type, st::bool_t>)
                lua_pushboolean(L, value);
            else if constexpr (std::is_same_v<type, st::int64>)
                lua_pushinteger(L, value);
            else if constexpr (std::is_same_v<type, st::float64>)
                lua_pushnumber(L, value);
            else if constexpr (std::is_same_v<type, std::string_view>)
                lua_pushlstring(L, value.data(), value.size());
        },
        arg);
}

// Runs under lua_pcall so that a failing global lookup (a metatable on _G can
// raise) or a failing push is reported like any other script error.
static int
call_global(lua_State *L)
{
    auto *call = static_cast<function_call *>(lua_touserdata(L, 1));

    luaL_checkstack(L, static_cast<int>(call->args.size()) + 2, "too many arguments");

    lua_pushglobaltable(L);
    lua_pushlstring(L, call->name.data(), call->name.size());
    lua_gettable(L, -2);
    if (!lua_isfunction(L, -1))
        return 0;

    call->found = true;
    for (const auto &arg : call->args)
        push_argument(L, arg);

    lua_call(L, static_cast<int>(call->args.size()), 0);
    return 0;
}

st::lua::runtime::runtime(std::string_view             name,
                          const std::filesystem::path &root_dir) noexcept :
    m_impl{
        std::make_unique<runtime_impl>(name,
                                       root_dir),
    }
{
    m_impl->L = luaL_newstate();
    ST_ASSERT(m_impl->L);
    st::lua::set_logger(m_impl->L, &m_impl->lua_logger);

    load_std_libraries(m_impl->L);

    std::error_code ec;
    auto            lua_root = st::io::filesystem::resolve(root_dir, ec);
    if (ec)
    {
        st::io::log::error(fmt::format("Cannot resolve Lua root directory '{}': {}", root_dir.string(), ec.message()));
        lua_root = root_dir;
    }
    lua_newtable(m_impl->L);
    lua_rawsetp(m_impl->L, LUA_REGISTRYINDEX, &require_cache_key);
    lua_pushstring(m_impl->L, lua_root.generic_string().c_str());
    lua_pushcclosure(m_impl->L, require_mod, 1);
    lua_setglobal(m_impl->L, "require");

    luaL_requiref(m_impl->L, "sticky", luaopen_sticky, 0);
    lua_setglobal(m_impl->L, "st");

    remove_unpermitted_globals(m_impl->L);

    constexpr const char *base_overrides = R"lua(
        local info = st.io.log.info
        local tostring = tostring
        local concat = table.concat
        local select = select
        function print(...)
            local parts = {}
            for i = 1, select('#', ...) do
                parts[i] = tostring((select(i, ...)))
            end
            info(concat(parts, "\t"))
        end

        local warn_log = st.io.log.warn
        local type = type
        local error = error
        local sub = string.sub
        function warn(...)
            local count = select('#', ...)
            if count == 0 then
                error("bad argument #1 to 'warn' (string expected, got no value)", 2)
            end
            local parts = {}
            for i = 1, count do
                local value = (select(i, ...))
                local kind = type(value)
                if kind ~= 'string' and kind ~= 'number' then
                    error("bad argument #" .. i .. " to 'warn' (string expected, got " .. kind .. ")", 2)
                end
                parts[i] = tostring(value)
            end
            -- A lone message starting with '@' is a control message for the warning system, not a warning.
            if count == 1 and sub(parts[1], 1, 1) == '@' then
                return
            end
            warn_log(concat(parts))
        end
    )lua";
    if (luaL_dostring(m_impl->L, base_overrides) != LUA_OK)
    {
        st::io::log::error(lua_tostring(m_impl->L, -1));
        lua_pop(m_impl->L, 1);
    }
}

st::lua::runtime::~runtime() noexcept
{
    // Finalizers run during lua_close and may log, so the logger outlives the state.
    lua_close(m_impl->L);
}

st::bool_t
st::lua::runtime::execute_file(const std::filesystem::path &path) noexcept
{
    std::filesystem::path resolved;

    if (!st::io::filesystem::resolve_file(m_impl->root_dir_path, path, resolved))
        return false;

    if (luaL_loadfilex(m_impl->L, resolved.generic_string().c_str(), "t") != LUA_OK)
    {
        st::io::log::error(lua_tostring(m_impl->L, -1));
        lua_pop(m_impl->L, 1);
        return false;
    }
    return protected_call(m_impl->L, 0);
}

st::bool_t
st::lua::runtime::execute_function(std::string_view name) noexcept
{
    return execute_function(name, {});
}

st::bool_t
st::lua::runtime::execute_function(std::string_view                          name,
                                   std::initializer_list<const lua_arg_type> args) noexcept
{
    function_call call{name, args};

    lua_pushcfunction(m_impl->L, call_global);
    lua_pushlightuserdata(m_impl->L, &call);

    const st::bool_t ok = protected_call(m_impl->L, 1);
    return ok && call.found;
}

st::bool_t
st::lua::runtime::execute_string(std::string_view code) noexcept
{
    if (luaL_loadbufferx(m_impl->L, code.data(), code.size(), "=(string)", "t") != LUA_OK)
    {
        st::io::log::error(lua_tostring(m_impl->L, -1));
        lua_pop(m_impl->L, 1);
        return false;
    }
    return protected_call(m_impl->L, 0);
}

lua_State *
st::lua::runtime::state() noexcept
{
    return m_impl->L;
}
