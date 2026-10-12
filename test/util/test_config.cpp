/*
 * test_config.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <filesystem>
#include <fstream>
#include <limits>
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
    template <typename T,
              typename Source>
    [[nodiscard]] T setting_as(const Source    &source,
                               std::string_view key)
    {
        const auto setting = source.get(key);
        if (!setting)
            return T{};
        return std::get<T>(*setting);
    }

    template <typename T>
    [[nodiscard]] T from_as(const st::util::config_scope &scope,
                            std::string_view              mod_name,
                            std::string_view              name)
    {
        const auto setting = scope.get_from(mod_name, name);
        if (!setting)
            return T{};
        return std::get<T>(*setting);
    }
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

TEST_CASE("config declare claims a key once and keeps a stored value of the same type")
{
    st::util::config config;
    config.set("mod.key", 5.0);

    CHECK(config.declare("mod.key", 1.0));
    CHECK(config.is_declared("mod.key"));
    CHECK(setting_as<st::float64>(config, "mod.key") == 5.0);
    CHECK_FALSE(config.declare("mod.key", 2.0));
    CHECK_FALSE(config.is_declared("mod.other"));
}

TEST_CASE("config scope builds keys from the owner name")
{
    CHECK(st::util::config_scope::key_for("mod", "key") == "mod.key");
    CHECK(st::util::config_scope::key_for("mod.", "key") == "mod.key");
}

TEST_CASE("config scope reads own and other declared settings and writes own ones")
{
    st::util::config       config;
    st::util::config_scope first(config, "first");
    st::util::config_scope second(config, "second.");

    CHECK(first.declare("flag", false));
    CHECK(second.declare("name", std::string("x")));

    CHECK(setting_as<st::bool_t>(first, "flag") == false);
    CHECK(from_as<std::string>(first, "second.", "name") == "x");
    CHECK(from_as<std::string>(first, "second", "name") == "x");
    CHECK_FALSE(first.get("name").has_value());
    CHECK_FALSE(first.get_from("second", "missing").has_value());

    CHECK(first.set("flag", true));
    CHECK(setting_as<st::bool_t>(config, "first.flag") == true);
    CHECK_FALSE(first.set("name", std::string("hijacked")));
    CHECK_FALSE(first.set("flag", 1.0));
    CHECK_FALSE(first.set("missing", true));
    CHECK(setting_as<std::string>(config, "second.name") == "x");
}

TEST_CASE("config scope ignores stored keys that nobody declared")
{
    st::util::config       config;
    st::util::config_scope scope(config, "mod");
    config.set("mod.stale", true);

    CHECK_FALSE(scope.get("stale").has_value());
    CHECK_FALSE(scope.get_from("mod", "stale").has_value());
    CHECK_FALSE(scope.set("stale", false));
}

TEST_CASE("config scope rejects non-finite numbers")
{
    st::util::config       config;
    st::util::config_scope scope(config, "mod");

    CHECK(scope.declare("number", 1.0));
    CHECK_FALSE(scope.set("number", std::numeric_limits<st::float64>::quiet_NaN()));
    CHECK_FALSE(scope.set("number", std::numeric_limits<st::float64>::infinity()));
    CHECK(setting_as<st::float64>(scope, "number") == 1.0);
}

TEST_CASE("config scope cannot write the settings of an owner whose name extends its own")
{
    st::util::config       config;
    st::util::config_scope outer(config, "a");
    st::util::config_scope inner(config, "a.b");

    CHECK(inner.declare("c", 1.0));
    CHECK_FALSE(outer.set("b.c", 2.0));
    CHECK(setting_as<st::float64>(config, "a.b.c") == 1.0);
}

TEST_CASE("config scope refuses a setting that another owner declared under the same key")
{
    st::util::config       config;
    st::util::config_scope outer(config, "a");
    st::util::config_scope inner(config, "a.b");

    CHECK(outer.declare("b.c", 1.0));
    CHECK_FALSE(inner.declare("c", 2.0));
    CHECK_FALSE(inner.set("c", 3.0));
    CHECK(setting_as<st::float64>(config, "a.b.c") == 1.0);
}
