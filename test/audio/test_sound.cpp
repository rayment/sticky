/*
 * test_sound.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <filesystem>
#include <memory>

#include <AL/al.h>
#include <doctest/doctest.h>

#include "audio/sound.h"
#include "headless_session.h"

namespace
{
    namespace fs = std::filesystem;

    const fs::path res_dir = fs::path(ST_TEST_RES_DIR);

    const fs::path good_empty = res_dir / "0001-good-empty.txt";
    const fs::path good_text = res_dir / "0002-good-text.txt";
    const fs::path good_multi = res_dir / "0003-good-multi.tar.gz";
    const fs::path good_tree = res_dir / "0005-good-tree";
    const fs::path good_wav = res_dir / "0006-good-wav.wav";
    const fs::path good_modfile = res_dir / "0007-good-modfile.it";

    // Never created on disk.
    const fs::path bad_missing = res_dir / "0099-bad-missing.wav";

    struct sound_fixture
    {
        st::test::headless_session &session = st::test::headless_session::instance();

        sound_fixture() { alGetError(); }
    };
} // namespace

TEST_SUITE("audio::sound")
{
    TEST_CASE_FIXTURE(sound_fixture, "load_file loads WAV files")
    {
        for (const fs::path &path : {good_wav, })
        {
            const std::shared_ptr<st::audio::sound> sound = st::audio::sound::load_file(path);

            CHECK_NE(nullptr, sound);
            CHECK_EQ(AL_NO_ERROR, alGetError());
        }
    }

    TEST_CASE_FIXTURE(sound_fixture, "load_file loads tracker module files")
    {
        const std::shared_ptr<st::audio::sound> sound = st::audio::sound::load_file(good_modfile);

        CHECK_NE(nullptr, sound);
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE_FIXTURE(sound_fixture, "load_file returns distinct sounds for the same file")
    {
        const std::shared_ptr<st::audio::sound> first = st::audio::sound::load_file(good_wav);
        const std::shared_ptr<st::audio::sound> second = st::audio::sound::load_file(good_wav);

        CHECK_NE(nullptr, first);
        CHECK_NE(nullptr, second);
        CHECK_NE(first, second);
    }

    TEST_CASE_FIXTURE(sound_fixture, "load_file returns null for missing paths")
    {
        CHECK_EQ(nullptr, st::audio::sound::load_file(bad_missing));
        CHECK_EQ(nullptr, st::audio::sound::load_file(fs::path{}));
        CHECK_EQ(nullptr, st::audio::sound::load_file(good_text / "0099-bad-below-file.wav"));
    }

    TEST_CASE_FIXTURE(sound_fixture, "load_file returns null for directories")
    {
        CHECK_EQ(nullptr, st::audio::sound::load_file(res_dir));
        CHECK_EQ(nullptr, st::audio::sound::load_file(good_tree));
    }

    TEST_CASE_FIXTURE(sound_fixture, "load_file returns null for non-audio files")
    {
        CHECK_EQ(nullptr, st::audio::sound::load_file(good_empty));
        CHECK_EQ(nullptr, st::audio::sound::load_file(good_text));
        CHECK_EQ(nullptr, st::audio::sound::load_file(good_multi));
    }

    TEST_CASE_FIXTURE(sound_fixture, "failed loads leave OpenAL without errors")
    {
        CHECK_EQ(nullptr, st::audio::sound::load_file(bad_missing));
        CHECK_EQ(nullptr, st::audio::sound::load_file(good_text));
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE_FIXTURE(sound_fixture, "a load can succeed after a failed one")
    {
        CHECK_EQ(nullptr, st::audio::sound::load_file(bad_missing));
        CHECK_NE(nullptr, st::audio::sound::load_file(good_wav));
    }

    TEST_CASE_FIXTURE(sound_fixture, "destroying a sound releases its buffer without errors")
    {
        std::shared_ptr<st::audio::sound> sound = st::audio::sound::load_file(good_wav);

        CHECK_NE(nullptr, sound);
        sound.reset();
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }
}
