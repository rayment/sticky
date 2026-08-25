/*
 * lua.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 25/08/2026
 */

#include "st_includes.h"
#include "lua/lua.h"
#include "log/log.h"

extern int luaopen_sticky(lua_State *L);

st_bool
st_lua_create(st_lua_t *L)
{
    *L = luaL_newstate();
    if (!*L)
        return false;
    luaL_openlibs(*L);
    lua_pushnil(*L);
    lua_setglobal(*L, "print");
    luaL_requiref(*L, "sticky", luaopen_sticky, 1);
    luaL_loadstring(*L, "st = require('sticky')");
    lua_pcallk(*L, 0, -1, 0, 0, (st_voidptr)0);
    lua_pop(*L, 1);
    return true;
}

st_bool
st_lua_destroy(st_lua_t L)
{
    lua_close(L);
    return true;
}

st_bool
st_lua_exec_file(st_lua_t L,
                 st_string_t path)
{
    st_char safe_path[path.len + 1];
    memcpy(safe_path, path.data, path.len);
    safe_path[path.len] = '\0';
    if (luaL_loadfile(L, safe_path) != LUA_OK)
        return false;
    if (lua_pcallk(L, 0, -1, 0, 0, (st_voidptr)0) != LUA_OK)
    {
        st_log_error("%s\n", lua_tostring(L, -1));

        luaL_traceback(L, L, lua_tostring(L, -1), 1);
        st_log_error("%s\n", lua_tostring(L, -1));

        lua_pop(L, 2);
        return false;
    }
    return true;
}

st_bool
st_lua_call_function(
    lua_State *lua,
    st_string_t name,
    size_t argc,
    const st_lua_arg_t *argv)
{
    st_char safe_name[name.len + 1];
    memcpy(safe_name, name.data, name.len);
    safe_name[name.len] = '\0';

    lua_getglobal(lua, safe_name);

    if (!lua_isfunction(lua, -1))
    {
        lua_pop(lua, 1);
        return false;
    }

    for (size_t i = 0; i < argc; ++i)
    {
        const st_lua_arg_t *arg = &argv[i];

        switch (arg->type)
        {
        case ST_LUA_ARG_NIL:
            lua_pushnil(lua);
            break;

        case ST_LUA_ARG_BOOLEAN:
            lua_pushboolean(lua, arg->value.boolean);
            break;

        case ST_LUA_ARG_INTEGER:
            lua_pushinteger(lua, arg->value.integer);
            break;

        case ST_LUA_ARG_NUMBER:
            lua_pushnumber(lua, arg->value.number);
            break;

        case ST_LUA_ARG_STRING:
            lua_pushstring(lua, arg->value.string);
            break;

        case ST_LUA_ARG_POINTER:
            lua_pushlightuserdata(lua, arg->value.pointer);
            break;
        }
    }

    if (lua_pcall(lua, (int)argc, 0, 0) != LUA_OK)
    {
        const char *error = lua_tostring(lua, -1);
        st_log_error("Lua %s error: %s\n", name, error);
        lua_pop(lua, 1);
        return false;
    }

    return true;
}
