/*
 * sound_handle.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include "audio/sound_handle.h"

namespace
{
    const st::audio::sound_registry *current_registry = nullptr;
} // namespace

st::audio::sound_handle::sound_handle(std::string_view                         name,
                                      const std::filesystem::path             &path,
                                      const std::shared_ptr<st::audio::sound> &sound) noexcept :
    resource_handle(name,
                    path,
                    sound)
{
}

const st::audio::sound_handle *
st::audio::sound_handle::get(std::string_view name) noexcept
{
    if (current_registry == nullptr)
        return nullptr;
    return current_registry->find(name);
}

void
st::audio::sound_handle::set_registry(const sound_registry *registry) noexcept
{
    current_registry = registry;
}
