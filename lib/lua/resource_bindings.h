/*
 * resource_bindings.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

/**
 * @file
 * @brief Lookup of named resources from Lua.
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

/** @addtogroup lua
 *  @{
 */

/**
 * Registries are found through the Lua state of the caller, so that a lookup made from a callback that the engine
 * fired at any time still resolves in the registry belonging to that state. A registry must outlive its state.
 */
namespace st::lua
{

#if ST_MODULE_VIDEO
    /**
     * @brief Sets the textures that `st.video.texture.get` finds.
     * @param L The Lua state.
     * @param registry The textures, or null.
     */
    void set_texture_registry(lua_State                         *L,
                              const st::video::texture_registry *registry) noexcept;
    /**
     * @brief Looks up a texture.
     * @param L The Lua state.
     * @param name Name of the texture.
     * @return The handle, or null.
     */
    [[nodiscard]] const st::video::texture_handle *find_texture(lua_State       *L,
                                                                std::string_view name) noexcept;
#endif
#if ST_MODULE_AUDIO
    /**
     * @brief Sets the sounds that `st.audio.sound.get` finds.
     * @param L The Lua state.
     * @param registry The sounds, or null.
     */
    void set_sound_registry(lua_State                       *L,
                            const st::audio::sound_registry *registry) noexcept;
    /** @brief Looks up a sound. @param L The Lua state. @param name Name of the sound. @return The handle, or null. */
    [[nodiscard]] const st::audio::sound_handle *find_sound(lua_State       *L,
                                                            std::string_view name) noexcept;
#endif

} // namespace st::lua

/** @} */

#endif // STICKY_LUA_RESOURCE_BINDINGS_H
