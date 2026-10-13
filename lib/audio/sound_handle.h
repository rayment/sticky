/*
 * sound_handle.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief Named sounds that a mod declared in its init phase.
 */

#ifndef STICKY_AUDIO_SOUND_HANDLE_H
#define STICKY_AUDIO_SOUND_HANDLE_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <filesystem>
#include <memory>
#include <string_view>

#include "audio/sound.h"
#include "resource/resource_handle.h"
#include "resource/resource_registry.h"

/** @addtogroup audio
 *  @{
 */

namespace st::audio
{
    class sound_handle;

    /** @brief The sounds of one mod, by name. */
    typedef st::resource_registry<sound_handle> sound_registry;

    /** @brief A sound loaded from the `st.init.sounds` table of a mod. Lua sees it as `st.audio.sound`. */
    class sound_handle : public st::resource_handle<st::audio::sound>
    {
    public:
        /**
         * @brief Wraps a loaded sound.
         * @param name Unique name of the sound within its mod.
         * @param path Path the sound was loaded from, relative to the `res` folder of the mod.
         * @param sound The sound.
         */
        sound_handle(std::string_view                         name,
                     const std::filesystem::path             &path,
                     const std::shared_ptr<st::audio::sound> &sound) noexcept;
    };

} // namespace st::audio

/** @} */

#endif // STICKY_AUDIO_SOUND_HANDLE_H
