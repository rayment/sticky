/*
 * test_client.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 25/08/2026
 */

#include <doctest/doctest.h>

#include "audio/client.h"

TEST_SUITE("audio::client")
{
    TEST_CASE("create initializes listener state")
    {
        st_audio_client_t client;

        CHECK(st_audio_client_create(&client));
        CHECK_EQ(0.0f, client.position.x);
        CHECK_EQ(0.0f, client.position.y);
        CHECK_EQ(0.0f, client.position.z);
        CHECK_EQ(0.0f, client.velocity.x);
        CHECK_EQ(0.0f, client.velocity.y);
        CHECK_EQ(0.0f, client.velocity.z);
    }
}