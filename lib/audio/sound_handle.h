/*
 * sound_handle.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
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

namespace st::audio
{
    class sound_handle;

    typedef st::resource_registry<sound_handle> sound_registry;

    // A named sound loaded during the init phase of a mod.
    class sound_handle : public st::resource_handle<st::audio::sound>
    {
    public:
        sound_handle(std::string_view                         name,
                     const std::filesystem::path             &path,
                     const std::shared_ptr<st::audio::sound> &sound) noexcept;

        // Looks up a sound in the registry of the executing mod, or nullptr.
        [[nodiscard]] static const sound_handle *get(std::string_view name) noexcept;

        // Selects the registry used by `get`.
        static void set_registry(const sound_registry *registry) noexcept;
    };

} // namespace st::audio

#endif // STICKY_AUDIO_SOUND_HANDLE_H
