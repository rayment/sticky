/*
 * server.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include <cstring>

#include <AL/al.h>
#include <AL/alc.h>
#include <fmt/format.h>

#include "audio/server.h"
#include "io/log.h"

static ALCdevice *device;
static ALCcontext *context;

static void list_audio_devices(const ALCchar* devices)
{
    const ALCchar* dev = devices, * next = devices + 1;
    size_t len = 0;

    st::io::log::debug("Devices list:");
    st::io::log::debug("-------------");
    while (dev && *dev != '\0' && next && *next != '\0') {
        st::io::log::debug(fmt::format("{}", dev));
        len = strlen(dev);
        dev += (len + 1);
        next += (len + 2);
    }
    st::io::log::debug("-------------");
}

st::bool_t
st::audio::server::init()
{
    // TODO: Make calls safe.
    device = alcOpenDevice(nullptr);
    if (!alcIsExtensionPresent(device, "ALC_ENUMERATION_EXT"))
    {
        st::io::log::debug("alcIsExtensionPresent(): enumeration not enabled");
        return false;
    }
    list_audio_devices(alcGetString(nullptr, ALC_DEVICE_SPECIFIER));
    context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    return true;
}

st::bool_t
st::audio::server::free()
{
    // TODO: Make calls safe.
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
    return true;
}
