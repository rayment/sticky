/*
 * config_bindings.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

/**
 * @file
 * @brief The `st.util.config` Lua module.
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

/** @addtogroup lua
 *  @{
 */

namespace st::lua
{

    /**
     * @brief Sets the settings that `st.util.config` of a Lua state reads and writes.
     * @param L The Lua state.
     * @param scope The settings, which must outlive the state, or null to make `st.util.config` unavailable.
     */
    void set_config_scope(lua_State              *L,
                          st::util::config_scope *scope) noexcept;

    /** @brief Lua C function behind `st.util.config.get(name)`. */
    int config_get(lua_State *L);
    /** @brief Lua C function behind `st.util.config.get_from(owner, name)`. */
    int config_get_from(lua_State *L);
    /** @brief Lua C function behind `st.util.config.set(name, value)`. */
    int config_set(lua_State *L);

} // namespace st::lua

/** @} */

#endif // STICKY_LUA_CONFIG_BINDINGS_H
