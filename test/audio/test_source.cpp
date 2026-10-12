/*
 * test_source.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <filesystem>
#include <memory>

#include <AL/al.h>
#include <SDL3/SDL.h>
#include <doctest/doctest.h>
#include <glm/vec3.hpp>

#include "audio/sound.h"
#include "audio/source.h"
#include "headless_session.h"

namespace
{
    namespace fs = std::filesystem;

    const fs::path res_dir = fs::path(ST_TEST_RES_DIR);

    // About one second of stereo 44.1kHz PCM.
    const fs::path good_wav = res_dir / "0006-good-wav.wav";
    // Around 35 seconds long: only ever played briefly, then stopped.
    const fs::path good_modfile = res_dir / "0007-good-modfile.it";

    constexpr st::uint32 clip_length_ms = 1000;
    constexpr st::uint32 poll_step_ms = 50;
    constexpr st::uint32 poll_timeout_ms = 5000;

    // The sound must outlive the source that has it selected, so it is
    // declared first and destroyed last.
    struct source_fixture
    {
        st::test::headless_session       &session = st::test::headless_session::instance();
        std::shared_ptr<st::audio::sound> piano = st::audio::sound::load_file(good_wav);
        st::audio::source                 source;

        source_fixture()
        {
            alGetError();
            CHECK_NE(nullptr, piano);
            if (piano)
                CHECK(source.select(*piano));
        }
    };

    template <typename predicate>
    [[nodiscard]] bool wait_until(predicate done)
    {
        for (st::uint32 elapsed = 0; elapsed < poll_timeout_ms; elapsed += poll_step_ms)
        {
            if (done())
                return true;
            SDL_Delay(poll_step_ms);
        }
        return done();
    }
} // namespace

TEST_SUITE("audio::source")
{
    TEST_CASE_FIXTURE(source_fixture, "a new source is neither playing, paused nor stopped")
    {
        CHECK_FALSE(source.is_playing());
        CHECK_FALSE(source.is_paused());
        CHECK_FALSE(source.is_stopped());
    }

    TEST_CASE_FIXTURE(source_fixture, "construction and selection raise no OpenAL errors")
    {
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE_FIXTURE(source_fixture, "play starts playback")
    {
        CHECK(source.play());

        CHECK(source.is_playing());
        CHECK_FALSE(source.is_paused());
        CHECK_FALSE(source.is_stopped());
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE_FIXTURE(source_fixture, "pause holds playback and resume continues it")
    {
        CHECK(source.play());

        CHECK(source.pause());
        CHECK(source.is_paused());
        CHECK_FALSE(source.is_playing());

        CHECK(source.resume());
        CHECK(source.is_playing());
        CHECK_FALSE(source.is_paused());
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE_FIXTURE(source_fixture, "stop ends playback")
    {
        CHECK(source.play());

        CHECK(source.stop());
        CHECK(source.is_stopped());
        CHECK_FALSE(source.is_playing());
        CHECK_FALSE(source.is_paused());
    }

    TEST_CASE_FIXTURE(source_fixture, "stop works on a paused source")
    {
        CHECK(source.play());
        CHECK(source.pause());

        CHECK(source.stop());
        CHECK(source.is_stopped());
    }

    TEST_CASE_FIXTURE(source_fixture, "play restarts a stopped source")
    {
        CHECK(source.play());
        CHECK(source.stop());
        CHECK(source.is_stopped());

        CHECK(source.play());
        CHECK(source.is_playing());
    }

    TEST_CASE_FIXTURE(source_fixture, "play restarts a source that is already playing")
    {
        CHECK(source.play());

        CHECK(source.play());
        CHECK(source.is_playing());
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE_FIXTURE(source_fixture, "a one-shot sound stops by itself")
    {
        CHECK(source.play());

        CHECK(wait_until([&] { return source.is_stopped(); }));
        CHECK_FALSE(source.is_playing());
    }

    TEST_CASE_FIXTURE(source_fixture, "a looping sound keeps playing past its length")
    {
        CHECK(source.set_loop(true));
        CHECK(source.play());

        SDL_Delay(clip_length_ms + 500);
        CHECK(source.is_playing());

        CHECK(source.set_loop(false));
        CHECK(source.stop());
        CHECK(source.is_stopped());
    }

    TEST_CASE_FIXTURE(source_fixture, "select swaps the sound a source plays")
    {
        const std::shared_ptr<st::audio::sound> other = st::audio::sound::load_file(good_wav);
        CHECK_NE(nullptr, other);
        if (!other)
            return;

        CHECK(source.select(*other));
        CHECK(source.play());
        CHECK(source.is_playing());

        // Detach the sound that is about to go out of scope.
        CHECK(source.stop());
        CHECK(source.select(*piano));
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE_FIXTURE(source_fixture, "sources play the same sound independently")
    {
        st::audio::source second;
        CHECK(second.select(*piano));

        CHECK(source.play());
        CHECK(source.is_playing());
        CHECK_FALSE(second.is_playing());

        CHECK(second.play());
        CHECK(source.stop());
        CHECK(second.is_playing());
        CHECK(source.is_stopped());

        CHECK(second.stop());
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE_FIXTURE(source_fixture, "setters accept values and raise no OpenAL errors")
    {
        CHECK(source.set_position({1.0, 2.0, 3.0}));
        CHECK(source.set_velocity({-1.0, 0.5, 0.0}));
        CHECK(source.set_pitch(1.5F));
        CHECK(source.set_gain(0.5F));
        CHECK(source.set_loop(true));
        CHECK(source.set_loop(false));
        CHECK(source.set_spatial(true));
        CHECK(source.set_spatial(false));
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE_FIXTURE(source_fixture, "setters work while a source is playing")
    {
        CHECK(source.play());

        CHECK(source.set_position({5.0, 0.0, 0.0}));
        CHECK(source.set_pitch(2.0F));
        CHECK(source.set_gain(0.25F));
        CHECK(source.is_playing());
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE_FIXTURE(source_fixture, "a muted source still plays")
    {
        CHECK(source.set_gain(0.0F));
        CHECK(source.play());

        CHECK(source.is_playing());
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE_FIXTURE(source_fixture, "a tracker module loads and plays")
    {
        const std::shared_ptr<st::audio::sound> module = st::audio::sound::load_file(good_modfile);
        CHECK_NE(nullptr, module);
        if (!module)
            return;

        st::audio::source module_source;
        CHECK(module_source.select(*module));
        CHECK(module_source.play());
        CHECK(module_source.is_playing());

        // The module is far longer than this; just prove it keeps playing.
        SDL_Delay(clip_length_ms);
        CHECK(module_source.is_playing());

        CHECK(module_source.stop());
        CHECK(module_source.is_stopped());
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }
}
