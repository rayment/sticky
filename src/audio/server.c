/*
 * server.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include "st_includes.h"
#include "audio/server.h"
#include "log/log.h"

static ALCdevice *device;
static ALCcontext *context;

static void list_audio_devices(const ALCchar* devices)
{
    const ALCchar* dev = devices, * next = devices + 1;
    size_t len = 0;

    st_log_debug("Devices list:");
    st_log_debug("-------------");
    while (dev && *dev != '\0' && next && *next != '\0') {
        st_log_debug("%s", dev);
        len = strlen(dev);
        dev += (len + 1);
        next += (len + 2);
    }
    st_log_debug("-------------");
}

st_bool
st_audio_server_init(void)
{
    // TODO: Make calls safe.
    device = alcOpenDevice(nullptr);
    if (!alcIsExtensionPresent(device, "ALC_ENUMERATION_EXT"))
    {
        st_log_debug("alcIsExtensionPresent(): enumeration not enabled");
        return false;
    }
    list_audio_devices(alcGetString(nullptr, ALC_DEVICE_SPECIFIER));
    context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
    return true;
}

st_bool
st_audio_server_free(void)
{
    // TODO: Make calls safe.
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
    return true;
}
