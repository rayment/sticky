/*
 * test_modloader.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 13/10/2026
 */

#include <string>

#include <doctest/doctest.h>

#include "modloader.h"

TEST_SUITE("modloader")
{
    TEST_CASE("mod names are lowercase letters, digits and underscores")
    {
        for (const char *name : {"chess", "core", "a", "mod_2", "_", "0"})
        {
            CAPTURE(name);
            CHECK(st::modloader::is_valid_name(name));
        }
    }

    TEST_CASE("mod names that would clash with keys, paths or loggers are rejected")
    {
        for (const char *name : {"", "Chess", "my-mod", "my mod", "core.", "a.b", "../x", "a/b", "caf\xc3\xa9"})
        {
            CAPTURE(name);
            CHECK_FALSE(st::modloader::is_valid_name(name));
        }
    }

    TEST_CASE("mod names are at most 64 characters")
    {
        CHECK(st::modloader::is_valid_name(std::string(64, 'a')));
        CHECK_FALSE(st::modloader::is_valid_name(std::string(65, 'a')));
    }

    TEST_CASE("mods are kept in name order")
    {
        st::mod_map mods;
        mods.emplace("zeta", nullptr);
        mods.emplace("alpha", nullptr);
        mods.emplace("mid", nullptr);

        std::string order;
        for (const auto &[name, mod] : mods)
            order += name + " ";
        CHECK_EQ("alpha mid zeta ", order);
    }
}
