/*
 * mod.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

extern "C" {
#include <lauxlib.h>
#include <lua.h>
}

#include <vector>

#include <fmt/format.h>

#include "io/filesystem.h"
#include "io/log.h"
#include "lua/config_bindings.h"
#include "lua/resource_bindings.h"
#if ST_MODULE_AUDIO
#include "audio/server.h"
#include "audio/sound.h"
#include "audio/sound_handle.h"
#endif
#if ST_MODULE_VIDEO
#include "video/texture.h"
#include "video/texture_handle.h"
#endif

#include "mod.h"

namespace
{
    struct resource_definition
    {
        std::string name;
        std::string path;
    };

    st::char_t st_table_key;

    struct config_definition
    {
        std::string             name;
        std::string             description;
        st::util::config::value default_value;
    };

    constexpr const char *init_tables[] = {"config", "textures", "sounds"};

    int init_phase_only(lua_State *L) { return luaL_error(L, "st.init is only available during the init phase"); }

    // Replaces `st.init` with an empty definition table, or with a table that raises
    // on any access. Uses raw access so that scripts cannot intercept it.
    void set_init_table(lua_State *L,
                        st::bool_t locked) noexcept
    {
        lua_rawgetp(L, LUA_REGISTRYINDEX, &st_table_key);
        lua_pushstring(L, "init");
        lua_newtable(L);
        if (locked)
        {
            lua_newtable(L);
            lua_pushcfunction(L, init_phase_only);
            lua_setfield(L, -2, "__index");
            lua_pushcfunction(L, init_phase_only);
            lua_setfield(L, -2, "__newindex");
            lua_setmetatable(L, -2);
        }
        else
        {
            for (const char *table : init_tables)
            {
                lua_pushstring(L, table);
                lua_newtable(L);
                lua_rawset(L, -3);
            }
        }
        lua_rawset(L, -3);
        lua_pop(L, 1);
    }

    [[nodiscard]] st::bool_t read_string_field(lua_State   *L,
                                               const char  *key,
                                               std::string &out) noexcept
    {
        lua_pushstring(L, key);
        lua_rawget(L, -2);
        const st::bool_t is_string = lua_type(L, -1) == LUA_TSTRING;
        if (is_string)
            out = lua_tostring(L, -1);
        lua_pop(L, 1);
        return is_string;
    }

    // Calls `read` for each entry of `st.init.<table>` with the entry table on top of the stack,
    // logging the entries that are not tables or that `read` rejects.
    template <typename Reader>
    void for_each_entry(lua_State  *L,
                        const char *table,
                        const char *requirement,
                        Reader      read) noexcept
    {
        lua_rawgetp(L, LUA_REGISTRYINDEX, &st_table_key);
        lua_pushstring(L, "init");
        lua_rawget(L, -2);
        if (lua_type(L, -1) == LUA_TTABLE)
        {
            lua_pushstring(L, table);
            lua_rawget(L, -2);
            if (lua_type(L, -1) != LUA_TTABLE)
            {
                st::io::log::error(fmt::format("st.init.{} must be a table.", table));
            }
            else
            {
                const lua_Unsigned count = lua_rawlen(L, -1);
                for (lua_Unsigned i = 1; i <= count; ++i)
                {
                    lua_rawgeti(L, -1, static_cast<lua_Integer>(i));
                    if (lua_type(L, -1) != LUA_TTABLE || !read())
                        st::io::log::error(fmt::format("st.init.{}[{}] needs {}.", table, i, requirement));
                    lua_pop(L, 1);
                }
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 2);
    }

    [[nodiscard]] std::vector<resource_definition> collect_definitions(lua_State  *L,
                                                                       const char *table) noexcept
    {
        std::vector<resource_definition> definitions;

        for_each_entry(L, table, "string 'name' and 'path' keys",
                       [&]
                       {
                           resource_definition definition;
                           if (!read_string_field(L, "name", definition.name) ||
                               !read_string_field(L, "path", definition.path))
                               return false;
                           definitions.push_back(std::move(definition));
                           return true;
                       });

        return definitions;
    }

    // Reads the `default` key of the table on top of the stack, which must match `type`.
    [[nodiscard]] st::bool_t read_config_default(lua_State               *L,
                                                 const std::string       &type,
                                                 st::util::config::value &out) noexcept
    {
        lua_pushstring(L, "default");
        lua_rawget(L, -2);

        const int  lua_value_type = lua_type(L, -1);
        st::bool_t matches = true;
        if (type == "string" && lua_value_type == LUA_TSTRING)
            out = std::string(lua_tostring(L, -1));
        else if (type == "number" && lua_value_type == LUA_TNUMBER)
            out = lua_tonumber(L, -1);
        else if (type == "boolean" && lua_value_type == LUA_TBOOLEAN)
            out = lua_toboolean(L, -1) != 0;
        else
            matches = false;

        lua_pop(L, 1);
        return matches;
    }

    // Collects the valid settings of `st.init.config` by their short names.
    [[nodiscard]] std::vector<config_definition> collect_config(lua_State *L) noexcept
    {
        std::vector<config_definition> definitions;

        for_each_entry(L, "config", "string 'name' and 'type' keys and a 'default' matching the type",
                       [&]
                       {
                           config_definition definition;
                           std::string       type;
                           if (!read_string_field(L, "name", definition.name) || definition.name.empty() ||
                               !read_string_field(L, "type", type) ||
                               !read_config_default(L, type, definition.default_value))
                               return false;
                           static_cast<void>(read_string_field(L, "description", definition.description));
                           definitions.push_back(std::move(definition));
                           return true;
                       });

        return definitions;
    }

    // Loads each definition into the registry, logging and skipping the ones that fail.
    template <typename Registry,
              typename Loader,
              typename Progress>
    void load_resources(Registry                               &registry,
                        const std::filesystem::path            &res_dir,
                        const std::vector<resource_definition> &definitions,
                        const char                             *kind,
                        Loader                                  load,
                        Progress                               &progress)
    {
        for (const auto &definition : definitions)
        {
            progress(kind, definition.name);
            const std::filesystem::path requested(definition.path);
            std::filesystem::path       resolved;

            if (definition.name.empty())
            {
                st::io::log::error(fmt::format("{} name must not be empty.", kind));
                continue;
            }
            if (registry.find(definition.name) != nullptr)
            {
                st::io::log::error(fmt::format("{} name '{}' is already in use.", kind, definition.name));
                continue;
            }
            if (requested.is_absolute() || requested.has_root_name() ||
                !st::io::filesystem::resolve_file(res_dir, requested, resolved))
            {
                st::io::log::error(fmt::format("{} '{}' path '{}' is not a file within the res folder.", kind,
                                               definition.name, definition.path));
                continue;
            }

            const auto resource = load(resolved);
            if (resource == nullptr)
            {
                st::io::log::error(
                    fmt::format("{} '{}' could not be loaded from '{}'.", kind, definition.name, definition.path));
                continue;
            }
            static_cast<void>(registry.add(definition.name, requested, resource));
        }
    }
} // namespace

struct st::mod::mod_impl
{
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    mod_impl(std::string_view             name,
             std::string_view             display_name,
             std::string_view             version,
             std::string_view             author,
             std::string_view             description,
             const std::filesystem::path &root_dir) noexcept :
        m_name(name),
        m_display_name(display_name),
        m_version(version),
        m_author(author),
        m_description(description),
        m_root_dir(root_dir),
        m_runtime(name,
                  root_dir)
    {
        lua_State *L = m_runtime.state();
        if (L == nullptr)
            return;
        lua_getglobal(L, "st");
        lua_rawsetp(L, LUA_REGISTRYINDEX, &st_table_key);
        set_init_table(L, true);
#if ST_MODULE_AUDIO
        st::lua::set_sound_registry(L, &m_sounds);
#endif
#if ST_MODULE_VIDEO
        st::lua::set_texture_registry(L, &m_textures);
#endif
    }

    std::string m_name;
    std::string m_display_name;
    std::string m_version;
    std::string m_author;
    std::string m_description;
    // Settings declared by the last init phase.
    std::vector<config_definition> m_config;
    // The Lua folder of the mod; the `res` folder sits next to it.
    std::filesystem::path m_root_dir;
    // Outlives the runtime, whose registry holds a pointer to it.
    std::unique_ptr<st::util::config_scope> m_scope;
    // The registries outlive the runtime, whose scripts hold pointers into them.
#if ST_MODULE_AUDIO
    st::audio::sound_registry m_sounds;
#endif
#if ST_MODULE_VIDEO
    st::video::texture_registry m_textures;
#endif
    st::lua::runtime m_runtime;
};

st::mod::mod(std::string_view             name,
             std::string_view             display_name,
             std::string_view             version,
             std::string_view             author,
             std::string_view             description,
             const std::filesystem::path &root_dir) noexcept :
    m_impl{
        std::make_unique<mod_impl>(name,
                                   display_name,
                                   version,
                                   author,
                                   description,
                                   root_dir),
    }
{
}

st::mod::~mod() noexcept {}

std::string
st::mod::author() const noexcept
{
    return m_impl->m_author;
}

std::string
st::mod::description() const noexcept
{
    return m_impl->m_description;
}

std::string
st::mod::display_name() const noexcept
{
    return m_impl->m_display_name;
}

std::string
st::mod::name() const noexcept
{
    return m_impl->m_name;
}

st::bool_t
st::mod::execute_init(const progress_callback &progress) noexcept
{
    if (m_impl->m_runtime.state() == nullptr)
        return false;

    const auto report = [&](std::string_view status, st::float32 fraction)
    {
        if (progress)
            progress(status, fraction);
    };

    lua_State *L = m_impl->m_runtime.state();

    report(fmt::format("{}: running init.lua", m_impl->m_display_name), 0.0f);
    set_init_table(L, false);
    const st::bool_t ok = m_impl->m_runtime.execute_file("init.lua");
    const auto       textures = ok ? collect_definitions(L, "textures") : std::vector<resource_definition>{};
    const auto       sounds = ok ? collect_definitions(L, "sounds") : std::vector<resource_definition>{};
    m_impl->m_config = ok ? collect_config(L) : std::vector<config_definition>{};
    set_init_table(L, true);

    if (!ok)
        return false;

    const std::filesystem::path res_dir = m_impl->m_root_dir.parent_path() / "res";
    const st::float32           total = static_cast<st::float32>(textures.size() + sounds.size());
    st::float32                 loaded = 0.0f;
    [[maybe_unused]] const auto on_resource = [&](const char *kind, const std::string &name)
    {
        report(fmt::format("{}: loading {} '{}'", m_impl->m_display_name, kind, name), loaded / total);
        loaded += 1.0f;
    };
#if ST_MODULE_VIDEO
    load_resources(m_impl->m_textures, res_dir, textures, "texture", st::video::texture::load_file, on_resource);
#else
    if (!textures.empty())
        st::io::log::warn("Textures were declared but the video module is disabled.");
#endif
#if ST_MODULE_AUDIO
    if (!sounds.empty() && !st::audio::server::is_available())
        st::io::log::warn(
            fmt::format("The sounds of '{}' were not loaded because no audio device is open.", m_impl->m_display_name));
    else
        load_resources(m_impl->m_sounds, res_dir, sounds, "sound", st::audio::sound::load_file, on_resource);
#else
    if (!sounds.empty())
        st::io::log::warn("Sounds were declared but the audio module is disabled.");
#endif
    report(fmt::format("{}: initialised", m_impl->m_display_name), 1.0f);
    return true;
}

void
st::mod::bind_config(st::util::config &config) noexcept
{
    auto scope = std::make_unique<st::util::config_scope>(config, m_impl->m_name);
    for (const auto &definition : m_impl->m_config)
        static_cast<void>(scope->declare(definition.name, definition.default_value));

    if (m_impl->m_runtime.state() != nullptr)
        st::lua::set_config_scope(m_impl->m_runtime.state(), scope.get());
    m_impl->m_scope = std::move(scope);
}

st::bool_t
st::mod::execute_runtime() noexcept
{
    return m_impl->m_runtime.execute_file("runtime.lua");
}

st::bool_t
st::mod::execute_function(std::string_view name) noexcept
{
    return m_impl->m_runtime.execute_function(name);
}

st::bool_t
st::mod::execute_function(std::string_view                                            name,
                          std::initializer_list<const st::lua::runtime::lua_arg_type> args) noexcept
{
    return m_impl->m_runtime.execute_function(name, args);
}

std::vector<st::config_setting>
st::mod::config_settings() const noexcept
{
    std::vector<config_setting> settings;
    settings.reserve(m_impl->m_config.size());
    for (const auto &definition : m_impl->m_config)
    {
        settings.push_back({st::util::config_scope::key_for(m_impl->m_name, definition.name), definition.description});
    }
    return settings;
}

std::string
st::mod::version() const noexcept
{
    return m_impl->m_version;
}
