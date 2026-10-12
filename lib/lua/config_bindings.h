/*
 * config_bindings.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#ifndef STICKY_LUA_CONFIG_BINDINGS_H
#define STICKY_LUA_CONFIG_BINDINGS_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

struct lua_State;

namespace st::util
{
    class config_scope; // forward declaration
}

namespace st::lua
{

    // Makes the scope the settings that `st.util.config` reads and writes from the given Lua state, or removes it
    // when null. The scope must outlive the state.
    void set_config_scope(lua_State              *L,
                          st::util::config_scope *scope) noexcept;

    // Lua functions behind `st.util.config.get`, `get_from` and `set`.
    int config_get(lua_State *L);
    int config_get_from(lua_State *L);
    int config_set(lua_State *L);

} // namespace st::lua

#endif // STICKY_LUA_CONFIG_BINDINGS_H
