/*
 * config_bindings.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

extern "C" {
#include <lauxlib.h>
#include <lua.h>
}

#include <optional>
#include <string>
#include <string_view>

#include "io/log.h"
#include "util/config.h"

#include "lua/config_bindings.h"

namespace
{
    char scope_key;

    // The scope is found through the state, so it is the right one even when the call comes from a callback that
    // the engine fired outside of the code that registered it. The registry is shared by coroutines.
    [[nodiscard]] st::util::config_scope *scope_of(lua_State *L) noexcept
    {
        lua_rawgetp(L, LUA_REGISTRYINDEX, &scope_key);
        auto *scope = static_cast<st::util::config_scope *>(lua_touserdata(L, -1));
        lua_pop(L, 1);
        if (scope == nullptr)
            st::io::log::error("st.util.config is only available during the runtime phase.");
        return scope;
    }

    int push_setting(lua_State                                    *L,
                     const std::optional<st::util::config::value> &setting) noexcept
    {
        if (!setting)
            lua_pushnil(L);
        else if (const auto *text = std::get_if<std::string>(&*setting))
            lua_pushlstring(L, text->data(), text->size());
        else if (const auto *number = std::get_if<st::float64>(&*setting))
            lua_pushnumber(L, *number);
        else
            lua_pushboolean(L, std::get<st::bool_t>(*setting) ? 1 : 0);
        return 1;
    }

    [[nodiscard]] std::string_view string_argument(lua_State *L,
                                                   int        index)
    {
        std::size_t length = 0;
        const char *text = luaL_checklstring(L, index, &length);
        return {text, length};
    }
} // namespace

void
st::lua::set_config_scope(lua_State              *L,
                          st::util::config_scope *scope) noexcept
{
    if (scope != nullptr)
        lua_pushlightuserdata(L, scope);
    else
        lua_pushnil(L);
    lua_rawsetp(L, LUA_REGISTRYINDEX, &scope_key);
}

int
st::lua::config_get(lua_State *L)
{
    const std::string_view name = string_argument(L, 1);
    const auto            *scope = scope_of(L);
    return push_setting(L, scope != nullptr ? scope->get(name) : std::nullopt);
}

int
st::lua::config_get_from(lua_State *L)
{
    const std::string_view owner = string_argument(L, 1);
    const std::string_view name = string_argument(L, 2);
    const auto            *scope = scope_of(L);
    return push_setting(L, scope != nullptr ? scope->get_from(owner, name) : std::nullopt);
}

int
st::lua::config_set(lua_State *L)
{
    const std::string_view name = string_argument(L, 1);
    auto                  *scope = scope_of(L);

    std::optional<st::util::config::value> setting;
    switch (lua_type(L, 2))
    {
    case LUA_TSTRING:
        setting = std::string(string_argument(L, 2));
        break;
    case LUA_TNUMBER:
        setting = static_cast<st::float64>(lua_tonumber(L, 2));
        break;
    case LUA_TBOOLEAN:
        setting = lua_toboolean(L, 2) != 0;
        break;
    default:
        st::io::log::error("st.util.config.set expects a string, number or boolean value.");
        break;
    }

    lua_pushboolean(L, (scope != nullptr && setting && scope->set(name, *setting)) ? 1 : 0);
    return 1;
}
