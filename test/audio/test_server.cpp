/*
 * test_server.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <AL/al.h>
#include <AL/alc.h>
#include <doctest/doctest.h>

#include "audio/server.h"
#include "headless_session.h"

TEST_SUITE("audio::server")
{
    TEST_CASE("library init leaves a current context and device")
    {
        static_cast<void>(st::test::headless_session::instance());

        ALCcontext *context = alcGetCurrentContext();
        CHECK_NE(nullptr, context);
        CHECK_NE(nullptr, alcGetContextsDevice(context));
        CHECK_EQ(AL_NO_ERROR, alGetError());
    }

    TEST_CASE("init sets the inverse distance clamped model")
    {
        static_cast<void>(st::test::headless_session::instance());

        CHECK_EQ(AL_INVERSE_DISTANCE_CLAMPED, alGetInteger(AL_DISTANCE_MODEL));
    }

    TEST_CASE("free releases the context and init restores it")
    {
        static_cast<void>(st::test::headless_session::instance());

        CHECK(st::audio::server::free());
        CHECK_EQ(nullptr, alcGetCurrentContext());

        CHECK(st::audio::server::init());
        CHECK_NE(nullptr, alcGetCurrentContext());
        CHECK_EQ(AL_INVERSE_DISTANCE_CLAMPED, alGetInteger(AL_DISTANCE_MODEL));
    }
}
