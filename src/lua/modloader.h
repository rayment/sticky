/*
 * modloader.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 25/08/2026
 */

#ifndef STICKY_LUA_MODLOADER_H
#define STICKY_LUA_MODLOADER_H

#include "st_primitives.h"
#include "lua/lua.h"
#include "type/string.h"

#ifdef __cplusplus
extern "C" {
#endif

st_bool st_lua_modloader_load_single(st_lua_t L, st_string_t mod_path);
st_bool st_lua_modloader_load_all   (st_lua_t L, st_string_t mod_root_path);

#ifdef __cplusplus
}
#endif

#endif // STICKY_LUA_MODLOADER_H
