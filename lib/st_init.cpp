/*
 * st_init.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 31/08/2026
 */

#include <ctime>
#include <cstdlib>
#include <filesystem>
#include <memory>

#include <SDL3/SDL.h>

#include "audio/server.h"
#include "io/log.h"
#include "st_init.h"
#include "st_macros.h"

#define ST_SDL_SAFE_CALL(name, expr) \
    if (ST_UNLIKELY(!(expr))) \
    { \
        const st_char *err = SDL_GetError(); \
        st::os::popup::error((name), err); \
        abort(); \
    }

// TODO: Do better than this?
#define ST_OPENAL_SAFE_CALL(name, expr) \
    if (ST_UNLIKELY(!(expr))) \
    { \
        st::os::popup::error((name), "Error occurred initialising OpenAL!"); \
        abort(); \
    }

static std::unique_ptr<st::io::log::logger> default_logger;

static void
init_sdl() noexcept
{
    st::io::log::debug("Init SDL resources.");
    ST_SDL_SAFE_CALL("SDL_Init", SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8));
#ifdef __APPLE__
    ST_SDL_SAFE_CALL("SDL_GL_SetAttribute", SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG));
#endif // __APPLE__
}

static void
init_openal() noexcept
{
    st::io::log::debug("Init OpenAL resources.");
    ST_OPENAL_SAFE_CALL("st::audio::server::init", st::audio::server::init());
}

static void
free_sdl() noexcept
{
    st::io::log::debug("Free SDL resources.");
    SDL_Quit();
}

static void
free_openal() noexcept
{
    st::io::log::debug("Free OpenAL resources.");
    ST_OPENAL_SAFE_CALL("st::audio::server::free", st::audio::server::free());
}

st_bool
st::lib_init() noexcept
{
    default_logger = std::make_unique<st::io::log::logger>("engine");
    default_logger->set_default();
    st::io::log::debug("--------");
    st::io::log::debug("Begin libsticky static init.");
    init_sdl();
    init_openal();
    st::io::log::debug("End libsticky static init.");
    st::io::log::debug("--------");
    return true;
}

st_bool
st::lib_free() noexcept
{
    st::io::log::debug("--------");
    st::io::log::debug("Begin libsticky static free.");
    free_openal();
    free_sdl();
    st::io::log::debug("End libsticky static free.");
    st::io::log::debug("--------");
    return true;
}
