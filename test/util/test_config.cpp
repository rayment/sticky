/*
 * test_config.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <filesystem>
#include <fstream>
#include <string>

#include <doctest/doctest.h>

#include "util/config.h"

namespace
{
    namespace fs = std::filesystem;

    const fs::path res_dir = fs::path(ST_TEST_RES_DIR);

    struct config_file
    {
        fs::path path;

        explicit config_file(const char *name) : path(res_dir / name)
        {
            std::error_code ec;
            fs::remove(path, ec);
        }

        ~config_file()
        {
            std::error_code ec;
            fs::remove(path, ec);
        }

        void write(const std::string &contents) const { std::ofstream(path) << contents; }
    };
} // namespace

TEST_CASE("config set and get round trip each type")
{
    st::util::config config;

    config.set("text", std::string("hello"));
    config.set("number", 1.5);
    config.set("flag", true);

    CHECK(std::get<std::string>(*config.get("text")) == "hello");
    CHECK(std::get<st::float64>(*config.get("number")) == 1.5);
    CHECK(std::get<st::bool_t>(*config.get("flag")) == true);
    CHECK_FALSE(config.get("missing").has_value());
}

TEST_CASE("config set_default keeps existing values of the same type")
{
    st::util::config config;

    CHECK(config.set_default("key", 1.0));
    config.set("key", 2.0);
    CHECK_FALSE(config.set_default("key", 3.0));
    CHECK(std::get<st::float64>(*config.get("key")) == 2.0);
}

TEST_CASE("config set_default replaces values of a different type")
{
    st::util::config config;

    config.set("key", std::string("text"));
    CHECK(config.set_default("key", false));
    CHECK(std::get<st::bool_t>(*config.get("key")) == false);
}

TEST_CASE("config save then load restores settings")
{
    config_file file("config_roundtrip.json");

    st::util::config saved;
    saved.set("mod.text", std::string("hello"));
    saved.set("mod.number", 42.0);
    saved.set("mod.flag", true);
    CHECK(saved.save(file.path));

    st::util::config loaded;
    CHECK(loaded.load(file.path));
    CHECK(std::get<std::string>(*loaded.get("mod.text")) == "hello");
    CHECK(std::get<st::float64>(*loaded.get("mod.number")) == 42.0);
    CHECK(std::get<st::bool_t>(*loaded.get("mod.flag")) == true);
}

TEST_CASE("config load treats a missing file as empty")
{
    st::util::config config;
    config.set("key", true);

    CHECK(config.load(res_dir / "config_does_not_exist.json"));
    CHECK_FALSE(config.get("key").has_value());
}

TEST_CASE("config load rejects malformed files")
{
    config_file      file("config_malformed.json");
    st::util::config config;

    file.write("{ not json");
    CHECK_FALSE(config.load(file.path));

    file.write("[1, 2, 3]");
    CHECK_FALSE(config.load(file.path));
    CHECK_FALSE(config.get("key").has_value());
}

TEST_CASE("config save fails when the destination is unwritable")
{
    st::util::config config;

    CHECK_FALSE(config.save(res_dir / "config_no_such_dir" / "config.json"));
}
