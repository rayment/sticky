/*
 * resource_bindings.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

extern "C" {
#include <lua.h>
}

#include "lua/resource_bindings.h"

#if ST_MODULE_VIDEO || ST_MODULE_AUDIO
namespace
{
#if ST_MODULE_VIDEO
    char texture_registry_key;
#endif
#if ST_MODULE_AUDIO
    char sound_registry_key;
#endif

    void store(lua_State  *L,
               const char *key,
               const void *registry) noexcept
    {
        if (registry != nullptr)
            lua_pushlightuserdata(L, const_cast<void *>(registry));
        else
            lua_pushnil(L);
        lua_rawsetp(L, LUA_REGISTRYINDEX, key);
    }

    [[nodiscard]] const void *load(lua_State  *L,
                                   const char *key) noexcept
    {
        lua_rawgetp(L, LUA_REGISTRYINDEX, key);
        const void *registry = lua_touserdata(L, -1);
        lua_pop(L, 1);
        return registry;
    }
} // namespace
#endif

#if ST_MODULE_VIDEO
void
st::lua::set_texture_registry(lua_State                         *L,
                              const st::video::texture_registry *registry) noexcept
{
    store(L, &texture_registry_key, registry);
}

const st::video::texture_handle *
st::lua::find_texture(lua_State       *L,
                      std::string_view name) noexcept
{
    const auto *registry = static_cast<const st::video::texture_registry *>(load(L, &texture_registry_key));
    return registry != nullptr ? registry->find(name) : nullptr;
}
#endif

#if ST_MODULE_AUDIO
void
st::lua::set_sound_registry(lua_State                       *L,
                            const st::audio::sound_registry *registry) noexcept
{
    store(L, &sound_registry_key, registry);
}

const st::audio::sound_handle *
st::lua::find_sound(lua_State       *L,
                    std::string_view name) noexcept
{
    const auto *registry = static_cast<const st::audio::sound_registry *>(load(L, &sound_registry_key));
    return registry != nullptr ? registry->find(name) : nullptr;
}
#endif
