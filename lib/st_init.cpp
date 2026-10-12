/*
 * st_init.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 31/08/2026
 */

#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <memory>

#if ST_MODULE_VIDEO
#include <SDL3/SDL.h>
#endif

#if ST_MODULE_AUDIO
#include "audio/server.h"
#endif
#include "io/log.h"
#include "os/popup.h"
#include "st_init.h"
#include "st_macros.h"

#if ST_MODULE_VIDEO
#define ST_SDL_SAFE_CALL(name, expr)                                                                                   \
    if (ST_UNLIKELY(!(expr)))                                                                                          \
    {                                                                                                                  \
        const st_char *err = SDL_GetError();                                                                           \
        init_fail((name), err);                                                                                        \
    }
#endif // ST_MODULE_VIDEO

#if ST_MODULE_AUDIO
// TODO: Do better than this?
#define ST_OPENAL_SAFE_CALL(name, expr)                                                                                \
    if (ST_UNLIKELY(!(expr)))                                                                                          \
    {                                                                                                                  \
        init_fail((name), "Error occurred initialising OpenAL!");                                                      \
    }
#endif // ST_MODULE_AUDIO

static std::unique_ptr<st::io::log::logger> default_logger;
static st_bool                              show_popups = true;

#if ST_MODULE_VIDEO || ST_MODULE_AUDIO
[[noreturn]] static void
init_fail(const st_char *name,
          const st_char *message) noexcept
{
    if (show_popups)
        st::os::popup::error(name, message);
    else
        st::io::log::error(message);
    abort();
}
#endif // ST_MODULE_VIDEO || ST_MODULE_AUDIO

#if ST_MODULE_VIDEO
static void
init_sdl() noexcept
{
    st::io::log::debug("Init SDL resources.");
    ST_SDL_SAFE_CALL("SDL_Init", SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute",
                     SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8));
#ifdef __APPLE__
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute",
                     SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG));
#endif // __APPLE__
}

static void
free_sdl() noexcept
{
    st::io::log::debug("Free SDL resources.");
    SDL_Quit();
}
#endif // ST_MODULE_VIDEO

#if ST_MODULE_AUDIO
static void
init_openal() noexcept
{
    st::io::log::debug("Init OpenAL resources.");
    ST_OPENAL_SAFE_CALL("st::audio::server::init", st::audio::server::init());
}

static void
free_openal() noexcept
{
    st::io::log::debug("Free OpenAL resources.");
    ST_OPENAL_SAFE_CALL("st::audio::server::free", st::audio::server::free());
}
#endif // ST_MODULE_AUDIO

st_bool
st::lib_init(st_bool popups) noexcept
{
    show_popups = popups;
    default_logger = std::make_unique<st::io::log::logger>("engine");
    default_logger->set_default();
    st::io::log::debug("--------");
    st::io::log::debug("Begin libsticky static init.");
#if ST_MODULE_VIDEO
    init_sdl();
#endif
#if ST_MODULE_AUDIO
    init_openal();
#endif
    st::io::log::debug("End libsticky static init.");
    st::io::log::debug("--------");
    return true;
}

st_bool
st::lib_free() noexcept
{
    st::io::log::debug("--------");
    st::io::log::debug("Begin libsticky static free.");
#if ST_MODULE_AUDIO
    free_openal();
#endif
#if ST_MODULE_VIDEO
    free_sdl();
#endif
    st::io::log::debug("End libsticky static free.");
    st::io::log::debug("--------");
    return true;
}
