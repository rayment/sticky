/*
 * server.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include <cstring>

#include <AL/al.h>
#include <AL/alc.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <fmt/format.h>

#include "audio/server.h"
#include "io/log.h"

namespace
{
    ALCdevice  *device = nullptr;
    ALCcontext *context = nullptr;
    st::bool_t  mixer_ready = false;

    // The specifier is a list of strings, each terminated by a null character, ending with an empty string.
    void list_audio_devices(const ALCchar *devices) noexcept
    {
        if (devices == nullptr)
            return;

        st::io::log::debug("Audio devices:");
        for (const ALCchar *it = devices; *it != '\0'; it += std::strlen(it) + 1)
            st::io::log::debug(fmt::format("  {}", it));
    }

    void close_device() noexcept
    {
        alcMakeContextCurrent(nullptr);
        if (context != nullptr)
            alcDestroyContext(context);
        if (device != nullptr)
            alcCloseDevice(device);
        context = nullptr;
        device = nullptr;
    }
} // namespace

st::bool_t
st::audio::server::init() noexcept
{
    if (device != nullptr)
        return true;

    if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT") != ALC_FALSE)
        list_audio_devices(alcGetString(nullptr, ALC_DEVICE_SPECIFIER));

    device = alcOpenDevice(nullptr);
    if (device == nullptr)
        return false;

    context = alcCreateContext(device, nullptr);
    if (context == nullptr || alcMakeContextCurrent(context) == ALC_FALSE)
    {
        close_device();
        return false;
    }
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

    mixer_ready = MIX_Init();
    if (!mixer_ready)
        st::io::log::warn(fmt::format("Sound files cannot be decoded: {}", SDL_GetError()));
    return true;
}

st::bool_t
st::audio::server::free() noexcept
{
    if (mixer_ready)
        MIX_Quit();
    mixer_ready = false;
    close_device();
    return true;
}

st::bool_t
st::audio::server::is_available() noexcept
{
    return device != nullptr;
}
