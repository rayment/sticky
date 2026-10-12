/*
 * test_client.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <AL/al.h>
#include <doctest/doctest.h>
#include <glm/vec3.hpp>

#include "audio/client.h"
#include "headless_session.h"

namespace
{
    struct client_fixture
    {
        st::test::headless_session &session = st::test::headless_session::instance();
        st::audio::client           client;

        client_fixture() { alGetError(); }
    };

    [[nodiscard]] glm::vec3 listener_vec3(ALenum param)
    {
        glm::vec3 value(-1.0F);
        alGetListener3f(param, &value.x, &value.y, &value.z);
        return value;
    }
} // namespace

TEST_SUITE("audio::client")
{
    TEST_CASE_FIXTURE(client_fixture, "starts at rest at the origin")
    {
        CHECK_EQ(glm::f64vec3(0, 0, 0), client.position());
        CHECK_EQ(glm::f64vec3(0, 0, 0), client.velocity());
    }

    TEST_CASE_FIXTURE(client_fixture, "set_position stores the position")
    {
        client.set_position({1.5, -2.25, 3.0});

        CHECK_EQ(glm::f64vec3(1.5, -2.25, 3.0), client.position());
        CHECK_EQ(glm::f64vec3(0, 0, 0), client.velocity());
    }

    TEST_CASE_FIXTURE(client_fixture, "set_velocity stores the velocity")
    {
        client.set_velocity({4.0, 0.5, -6.0});

        CHECK_EQ(glm::f64vec3(4.0, 0.5, -6.0), client.velocity());
        CHECK_EQ(glm::f64vec3(0, 0, 0), client.position());
    }

    TEST_CASE_FIXTURE(client_fixture, "setters do not touch the listener until selected")
    {
        client.select();
        client.set_position({9.0, 9.0, 9.0});
        client.set_velocity({8.0, 8.0, 8.0});

        CHECK_EQ(glm::vec3(0.0F), listener_vec3(AL_POSITION));
        CHECK_EQ(glm::vec3(0.0F), listener_vec3(AL_VELOCITY));
    }

    TEST_CASE_FIXTURE(client_fixture, "select applies position and velocity to the listener")
    {
        client.set_position({1.5, -2.25, 3.0});
        client.set_velocity({4.0, 0.5, -6.0});
        client.select();

        CHECK_EQ(AL_NO_ERROR, alGetError());
        CHECK_EQ(glm::vec3(1.5F, -2.25F, 3.0F), listener_vec3(AL_POSITION));
        CHECK_EQ(glm::vec3(4.0F, 0.5F, -6.0F), listener_vec3(AL_VELOCITY));
    }

    TEST_CASE_FIXTURE(client_fixture, "the most recently selected client owns the listener")
    {
        st::audio::client other;
        other.set_position({7.0, 7.0, 7.0});

        client.set_position({1.0, 2.0, 3.0});
        client.select();
        other.select();
        CHECK_EQ(glm::vec3(7.0F), listener_vec3(AL_POSITION));

        client.select();
        CHECK_EQ(glm::vec3(1.0F, 2.0F, 3.0F), listener_vec3(AL_POSITION));
    }
}
