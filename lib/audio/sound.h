/*
 * sound.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#ifndef STICKY_AUDIO_SOUND_H
#define STICKY_AUDIO_SOUND_H

#include <filesystem>
#include <memory>

#include "st_macros.h"
#include "st_types.h"

namespace st::audio
{
    class source; // forward-declare

    class sound
    {
    public:
        friend class st::audio::source;

        [[nodiscard]] static std::shared_ptr<sound> load_file(const std::filesystem::path &path) noexcept;

        ~sound() noexcept;

        ST_DELETE_COPY_CTORS(sound);
        ST_DELETE_MOVE_CTORS(sound);

    private:
        sound(st::uint32 buffer) noexcept;

        st::uint32 m_buffer;
    };
}

#endif // STICKY_AUDIO_SOUND_H
