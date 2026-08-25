/*
 * sound.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 21/08/2026
 */

#include "st_includes.h"
#include "st_macros.h"
#include "audio/sound.h"
#include "log/log.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <AL/al.h>
#include <AL/alc.h>

static st_bool
load_mod_to_openal_buffer(const char *path, ALuint *out_buffer)
{
    MIX_AudioDecoder *decoder = NULL;
    SDL_AudioSpec spec;
    SDL_AudioSpec decoder_spec;
    void *pcm = NULL;
    size_t pcm_size = 0;
    size_t pcm_capacity = 0;
    ALuint buffer = 0;

    if (!MIX_Init())
        return false;

    /*
     * This causes SDL3_mixer to select the appropriate decoder,
     * which should be libxmp for MOD files when XMP support is enabled.
     */
    decoder = MIX_CreateAudioDecoder(path, 0);
    if (decoder == NULL)
        goto fail;

    /*
     * We don't actually need the source format, but this can be useful
     * for debugging.
     */
    if (!MIX_GetAudioDecoderFormat(decoder, &decoder_spec))
        goto fail;

    /*
     * Ask SDL3_mixer to give us exactly the PCM format OpenAL wants.
     */
    SDL_zero(spec);
    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;
    spec.freq = 44100;

    /*
     * Decode in chunks because the final size of the MOD isn't
     * necessarily known to us up front.
     */
    for (;;)
    {
        const st_uint32 chunk_size = 64 * 1024;
        const st_size required = pcm_size + chunk_size;

        if (required > pcm_capacity)
        {
            st_size new_capacity = pcm_capacity ? pcm_capacity * 2 : chunk_size;

            while (new_capacity < required)
                new_capacity *= 2;

            st_voidptr new_pcm = SDL_realloc(pcm, new_capacity);
            if (new_pcm == NULL)
                goto fail;

            pcm = new_pcm;
            pcm_capacity = new_capacity;
        }

        const st_int32 decoded = MIX_DecodeAudio(
            decoder,
            (Uint8 *)pcm + pcm_size,
            ST_CAST(st_int32, chunk_size),
            &spec
        );

        if (decoded < 0)
            goto fail;

        pcm_size += (size_t)decoded;

        /*
         * SDL3_mixer defines zero as end-of-file.
         */
        if (decoded == 0)
            break;
    }

    MIX_DestroyAudioDecoder(decoder);
    decoder = NULL;

    /*
     * Now pcm contains:
     *
     *   S16
     *   stereo
     *   44100 Hz
     *
     * exactly what AL_FORMAT_STEREO16 expects.
     */
    alGenBuffers(1, &buffer);

    if (alGetError() != AL_NO_ERROR)
        goto fail;

    alBufferData(
        buffer,
        AL_FORMAT_STEREO16,
        pcm,
        (ALsizei)pcm_size,
        44100
    );

    if (alGetError() != AL_NO_ERROR)
        goto fail;

    SDL_free(pcm);

    *out_buffer = buffer;
    return true;

fail:
    if (buffer != 0)
        alDeleteBuffers(1, &buffer);

    if (decoder != NULL)
        MIX_DestroyAudioDecoder(decoder);

    SDL_free(pcm);

    return false;
}

st_bool
st_audio_sound_load_file(st_audio_sound_t *sound,
                        st_string_t path)
{
    ST_STRING_SAFE_CSTR(path, safe_path);

    sound->valid = false;
    if (load_mod_to_openal_buffer(safe_path, &sound->buffer))
    {
        sound->valid = true;
        return true;
    }
    return false;
}

st_bool
st_audio_sound_destroy(st_audio_sound_t sound)
{
    alDeleteBuffers(1, &sound.buffer);
    return true;
}
