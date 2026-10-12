/*
 * sound_handle.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include "audio/sound_handle.h"

st::audio::sound_handle::sound_handle(std::string_view                         name,
                                      const std::filesystem::path             &path,
                                      const std::shared_ptr<st::audio::sound> &sound) noexcept :
    resource_handle(name,
                    path,
                    sound)
{
}
