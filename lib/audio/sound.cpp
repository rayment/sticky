/*
 * sound.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include <algorithm>

#include <AL/al.h>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <fmt/format.h>

#include "audio/server.h"
#include "audio/sound.h"
#include "io/log.h"

namespace
{
    constexpr st::int32 sample_rate = 44100;
    constexpr st::int32 chunk_size = 64 * 1024;

    struct pcm_data
    {
        void     *data = nullptr;
        st::size  size = 0;
        st::uint8 channels = 0;
    };

    // Decodes the whole file to 16-bit PCM, keeping mono files mono and mixing anything else down to stereo, which
    // matches AL_FORMAT_MONO16 and AL_FORMAT_STEREO16. The caller frees the returned data with SDL_free.
    [[nodiscard]] pcm_data decode_file(const std::filesystem::path &path) noexcept
    {
        MIX_AudioDecoder *decoder = MIX_CreateAudioDecoder(path.u8string().c_str(), 0);
        if (decoder == nullptr)
            return {};

        SDL_AudioSpec spec;
        SDL_zero(spec);
        if (!MIX_GetAudioDecoderFormat(decoder, &spec))
        {
            MIX_DestroyAudioDecoder(decoder);
            return {};
        }
        const st::uint8 channels = spec.channels == 1 ? 1 : 2;
        spec.format = SDL_AUDIO_S16;
        spec.channels = channels;
        spec.freq = sample_rate;

        void    *pcm = nullptr;
        st::size capacity = 0;
        st::size size = 0;

        // The decoded size is not known up front, so the buffer grows as chunks arrive. Zero means end of file.
        for (;;)
        {
            const st::size required = size + chunk_size;
            if (required > capacity)
            {
                const st::size new_capacity = capacity == 0 ? required : std::max(capacity * 2, required);
                void          *grown = SDL_realloc(pcm, new_capacity);
                if (grown == nullptr)
                    break;
                pcm = grown;
                capacity = new_capacity;
            }

            const st::int32 decoded = MIX_DecodeAudio(decoder, static_cast<Uint8 *>(pcm) + size, chunk_size, &spec);
            if (decoded < 0)
                break;
            if (decoded == 0)
            {
                MIX_DestroyAudioDecoder(decoder);
                return {pcm, size, channels};
            }
            size += static_cast<st::size>(decoded);
        }

        MIX_DestroyAudioDecoder(decoder);
        SDL_free(pcm);
        return {};
    }

    [[nodiscard]] ALuint upload(const pcm_data &pcm) noexcept
    {
        // Clears errors left by earlier calls, so that only this upload is checked.
        static_cast<void>(alGetError());

        ALuint buffer = 0;
        alGenBuffers(1, &buffer);
        if (alGetError() != AL_NO_ERROR)
            return 0;

        const ALenum format = pcm.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        alBufferData(buffer, format, pcm.data, static_cast<ALsizei>(pcm.size), sample_rate);
        if (alGetError() != AL_NO_ERROR)
        {
            alDeleteBuffers(1, &buffer);
            return 0;
        }
        return buffer;
    }
} // namespace

st::audio::sound::sound(st::uint32 buffer) noexcept : m_buffer(buffer) {}

st::audio::sound::~sound() noexcept { alDeleteBuffers(1, &m_buffer); }

std::shared_ptr<st::audio::sound>
st::audio::sound::load_file(const std::filesystem::path &path) noexcept
{
    if (!st::audio::server::is_available())
    {
        st::io::log::error(fmt::format("Cannot load sound '{}' without an audio device.", path.u8string()));
        return nullptr;
    }

    const pcm_data pcm = decode_file(path);
    if (pcm.data == nullptr)
    {
        st::io::log::error(fmt::format("Failed to decode sound '{}': {}", path.u8string(), SDL_GetError()));
        return nullptr;
    }

    const ALuint buffer = upload(pcm);
    SDL_free(pcm.data);
    if (buffer == 0)
    {
        st::io::log::error(fmt::format("Failed to upload sound '{}' to the audio device.", path.u8string()));
        return nullptr;
    }
    return std::shared_ptr<sound>(new sound(buffer));
}

st::uint8
st::audio::sound::channels() const noexcept
{
    ALint channels = 0;
    alGetBufferi(m_buffer, AL_CHANNELS, &channels);
    return static_cast<st::uint8>(channels);
}
