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
    runtime_impl (std::string_view name) noexcept
        : logger_name(name), lua_logger(name)
    {}
    ~runtime_impl() noexcept = default;

    std::string logger_name;
    st::io::log::logger lua_logger;
    lua_State *L;
};

st::lua::runtime::runtime(std::string_view name) noexcept
    : m_impl{std::make_unique<runtime_impl>(name)}
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
}

st::lua::runtime::~runtime() noexcept
{
    if (st::lua::logger() == &m_impl->lua_logger)
        st::lua::set_logger(nullptr);
}

st::bool_t
st::lua::runtime::execute_file(const std::filesystem::path &path) noexcept
{
    if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
        return false;

    st::lua::set_logger(&m_impl->lua_logger);
    if (luaL_loadfile(m_impl->L, path.generic_string().c_str()) != LUA_OK)
        return false;
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
st::lua::runtime::execute_string(ST_UNUSED std::string_view code) noexcept
{
    // TODO: Implement
    return true;
}
