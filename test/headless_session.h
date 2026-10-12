/*
 * headless_session.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#ifndef STICKY_TEST_HEADLESS_SESSION_H
#define STICKY_TEST_HEADLESS_SESSION_H

#include <SDL3/SDL.h>
#include <doctest/doctest.h>

#include "st_init.h"
#include "st_macros.h"

namespace st::test
{
    // OpenAL's null backend avoids needing an audio device.
    // The library can only be initialized once per process, so every test
    // file shares this single session.
    struct headless_session
    {
        static headless_session &instance()
        {
            static headless_session session;
            return session;
        }

        headless_session()
        {
            SDL_setenv_unsafe("ALSOFT_DRIVERS", "null", 1);
            CHECK(st::lib_init(false));
        }

        ~headless_session() { static_cast<void>(st::lib_free()); }

        ST_DELETE_COPY_CTORS(headless_session);
        ST_DELETE_MOVE_CTORS(headless_session);
    };
} // namespace st::test

#endif // STICKY_TEST_HEADLESS_SESSION_H
