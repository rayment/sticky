%{
#include "lua/config_bindings.h"

using st::lua::config_get;
using st::lua::config_get_from;
using st::lua::config_set;
%}

%native(config_get) int config_get(lua_State *L);
%native(config_get_from) int config_get_from(lua_State *L);
%native(config_set) int config_set(lua_State *L);

%luacode {
    local M = sticky

    M.util = M.util or {}
    M.util.config = M.util.config or {}

    M.util.config.get = M.config_get
    M.util.config.get_from = M.config_get_from
    M.util.config.set = M.config_set

    M.config_get = nil
    M.config_get_from = nil
    M.config_set = nil
}
