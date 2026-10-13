/*
 * sound.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

/**
 * @file
 * @brief Decoded audio data held by the audio device.
 */

#ifndef STICKY_AUDIO_SOUND_H
#define STICKY_AUDIO_SOUND_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <filesystem>
#include <memory>

#include "st_macros.h"
#include "st_types.h"

/** @addtogroup audio
 *  @{
 */

namespace st::audio
{
    class source; // forward-declare

    /**
     * @brief A sound file decoded into an audio buffer, ready to be played by a st::audio::source.
     *
     * Sounds are decoded to 16-bit PCM at 44.1 kHz. Mono files stay mono and every other file is mixed down to
     * stereo. A source that plays a sound keeps it alive.
     */
    class sound
    {
    public:
        friend class st::audio::source;

        /**
         * @brief Decodes a sound file.
         *
         * Supports WAV and the tracker formats of libxmp (MOD, S3M, XM, IT).
         *
         * @param path File to decode.
         * @return The sound, or null if no audio device is open or the file is missing or cannot be decoded. The
         *         reason is logged.
         */
        [[nodiscard]] static std::shared_ptr<sound> load_file(const std::filesystem::path &path) noexcept;

        ~sound() noexcept;

        ST_DELETE_COPY_CTORS(sound);
        ST_DELETE_MOVE_CTORS(sound);

        /** @brief The number of channels: 1 for mono, 2 for stereo. */
        [[nodiscard]] st::uint8 channels() const noexcept;

    private:
        explicit sound(st::uint32 buffer) noexcept;

        st::uint32 m_buffer;
    };
} // namespace st::audio

/** @} */

#endif // STICKY_AUDIO_SOUND_H
