/*
 * st_init.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 31/08/2026
 */

#include <memory>

#include <fmt/format.h>

#if ST_MODULE_VIDEO
#include <SDL3/SDL.h>
#endif

#if ST_MODULE_AUDIO
#include "audio/server.h"
#endif
#include "io/log.h"
#include "os/popup.h"
#include "st_init.h"

namespace
{
    std::unique_ptr<st::io::log::logger> default_logger;
    st::bool_t                           initialised = false;

#if ST_MODULE_VIDEO
    // Returns false and reports the SDL error if `ok` is false.
    [[nodiscard]] st::bool_t sdl_check(st::bool_t  ok,
                                       const char *call) noexcept
    {
        if (!ok)
            st::os::popup::error("Could not start the engine", fmt::format("{} failed: {}", call, SDL_GetError()));
        return ok;
    }

    [[nodiscard]] st::bool_t init_sdl() noexcept
    {
        st::io::log::debug("Init SDL resources.");
        if (!sdl_check(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD), "SDL_Init"))
            return false;

        const st::bool_t ok = sdl_check(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0), "SDL_GL_SetAttribute") &&
            sdl_check(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE),
                      "SDL_GL_SetAttribute") &&
            sdl_check(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3), "SDL_GL_SetAttribute") &&
            sdl_check(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0), "SDL_GL_SetAttribute") &&
            sdl_check(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1), "SDL_GL_SetAttribute") &&
            sdl_check(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24), "SDL_GL_SetAttribute") &&
            sdl_check(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8), "SDL_GL_SetAttribute")
#ifdef __APPLE__
            && sdl_check(SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG),
                         "SDL_GL_SetAttribute")
#endif // __APPLE__
            ;
        if (!ok)
            SDL_Quit();
        return ok;
    }

    void free_sdl() noexcept
    {
        st::io::log::debug("Free SDL resources.");
        SDL_Quit();
    }
#endif // ST_MODULE_VIDEO
} // namespace

st::bool_t
st::lib_init(st::bool_t popups) noexcept
{
    if (initialised)
        return true;

    st::os::popup::set_enabled(popups);
    default_logger = std::make_unique<st::io::log::logger>("engine");
    default_logger->set_default();
    st::io::log::debug("--------");
    st::io::log::debug("Begin libsticky static init.");
#if ST_MODULE_VIDEO
    if (!init_sdl())
        return false;
#endif
#if ST_MODULE_AUDIO
    st::io::log::debug("Init audio resources.");
    if (!st::audio::server::init())
        st::io::log::warn("No audio device could be opened. The engine will run without sound.");
#endif
    st::io::log::debug("End libsticky static init.");
    st::io::log::debug("--------");
    initialised = true;
    return true;
}

st::bool_t
st::lib_free() noexcept
{
    if (!initialised)
        return true;

    st::io::log::debug("--------");
    st::io::log::debug("Begin libsticky static free.");
#if ST_MODULE_AUDIO
    st::io::log::debug("Free audio resources.");
    st::audio::server::free();
#endif
#if ST_MODULE_VIDEO
    free_sdl();
#endif
    st::io::log::debug("End libsticky static free.");
    st::io::log::debug("--------");
    initialised = false;
    return true;
}
