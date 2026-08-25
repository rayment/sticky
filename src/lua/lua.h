/*
 * lua.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 25/08/2026
 */

#ifndef STICKY_LUA_LUA_H
#define STICKY_LUA_LUA_H

#include "st_includes.h"
#include "st_primitives.h"
#include "type/string.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum st_lua_arg_type
{
    ST_LUA_ARG_NIL,
    ST_LUA_ARG_BOOLEAN,
    ST_LUA_ARG_INTEGER,
    ST_LUA_ARG_NUMBER,
    ST_LUA_ARG_STRING,
    ST_LUA_ARG_POINTER,
} st_lua_arg_type_t;

typedef struct st_lua_arg
{
    st_lua_arg_type_t type;

    union
    {
        st_bool boolean;
        lua_Integer integer;
        lua_Number number;
        const st_char *string;
        st_voidptr pointer;
    } value;
} st_lua_arg_t;

typedef lua_State *st_lua_t;

st_bool st_lua_create(st_lua_t *L);
st_bool st_lua_destroy(st_lua_t L);
st_bool st_lua_exec_file(st_lua_t L, st_string_t path);
st_bool st_lua_call_function(st_lua_t L, st_string_t name, st_size argc, const st_lua_arg_t *argv);

#ifdef __cplusplus
}
#endif

#endif // STICKY_LUA_LUA_H
