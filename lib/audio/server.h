/*
 * server.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

/**
 * @file
 * @brief The audio device and context shared by every sound and source.
 */

#ifndef STICKY_AUDIO_SERVER_H
#define STICKY_AUDIO_SERVER_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include "st_types.h"

/** @addtogroup audio
 *  @{
 */

/**
 * @brief The audio output device, opened by st::lib_init().
 *
 * Without a device, sounds fail to load and sources stay silent, but nothing else is affected.
 */
namespace st::audio::server
{

    /**
     * @brief Opens the default output device and the decoders for sound files.
     * @return False if no device or context could be opened. The engine then runs without sound.
     */
    st::bool_t init() noexcept;

    /**
     * @brief Closes the device. Every sound and source must be destroyed first. Does nothing if init() failed.
     * @return True once the device is closed.
     */
    st::bool_t free() noexcept;

    /** @brief Whether an output device is open, so that sounds can be loaded and played. */
    [[nodiscard]] st::bool_t is_available() noexcept;

} // namespace st::audio::server

/** @} */

#endif // STICKY_AUDIO_SERVER_H
