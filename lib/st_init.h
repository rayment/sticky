/*
 * st_init.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 31/08/2026
 */

/**
 * @file
 * @brief Start-up and shut-down of the engine library.
 */

#ifndef STICKY_ST_INIT_H
#define STICKY_ST_INIT_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include "st_types.h"

/** @addtogroup core
 *  @{
 */

namespace st
{

    /**
     * @brief Initialises the library: logging, then SDL and the audio server for the enabled modules.
     *
     * Call once, before anything else in the library. A missing audio device is not an error: the engine runs without
     * sound and st::audio::server::is_available() returns false.
     *
     * @param popups Whether st::os::popup shows message boxes. Pass false when running headless.
     * @return False if a required subsystem failed to start. The failure has been logged and shown to the user, and
     *         the library is left uninitialised.
     */
    [[nodiscard]] st::bool_t lib_init(st::bool_t popups = true) noexcept;

    /**
     * @brief Shuts down everything that lib_init() started.
     *
     * Every window, sound and texture must be destroyed first. Calling it when the library is not initialised does
     * nothing.
     *
     * @return True once the library is shut down.
     */
    [[nodiscard]] st::bool_t lib_free() noexcept;

} // namespace st

/** @} */

#endif // STICKY_ST_INIT_H
