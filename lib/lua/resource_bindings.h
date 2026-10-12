/*
 * resource_bindings.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#ifndef STICKY_LUA_RESOURCE_BINDINGS_H
#define STICKY_LUA_RESOURCE_BINDINGS_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <string_view>

#if ST_MODULE_AUDIO
#include "audio/sound_handle.h"
#endif
#if ST_MODULE_VIDEO
#include "video/texture_handle.h"
#endif

struct lua_State;

namespace st::lua
{

    // Registries are found through the Lua state of the caller, so that a lookup made from a callback that the engine
    // fired at any time still resolves in the registry belonging to that state. The registry must outlive the state.
#if ST_MODULE_VIDEO
    void set_texture_registry(lua_State                         *L,
                              const st::video::texture_registry *registry) noexcept;
    [[nodiscard]] const st::video::texture_handle *find_texture(lua_State       *L,
                                                                std::string_view name) noexcept;
#endif
#if ST_MODULE_AUDIO
    void                                         set_sound_registry(lua_State                       *L,
                                                                    const st::audio::sound_registry *registry) noexcept;
    [[nodiscard]] const st::audio::sound_handle *find_sound(lua_State       *L,
                                                            std::string_view name) noexcept;
#endif

} // namespace st::lua

#endif // STICKY_LUA_RESOURCE_BINDINGS_H
