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
#if ST_MODULE_AUDIO
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

    constexpr const char *init_tables[] = {"textures", "sounds"};

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

    [[nodiscard]] std::vector<resource_definition> collect_definitions(lua_State  *L,
                                                                       const char *table) noexcept
    {
        std::vector<resource_definition> definitions;

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
                    resource_definition definition;
                    if (lua_type(L, -1) == LUA_TTABLE && read_string_field(L, "name", definition.name) &&
                        read_string_field(L, "path", definition.path))
                        definitions.push_back(std::move(definition));
                    else
                        st::io::log::error(
                            fmt::format("st.init.{}[{}] needs string 'name' and 'path' keys.", table, i));
                    lua_pop(L, 1);
                }
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 2);

        return definitions;
    }

    // Loads each definition into the registry, logging and skipping the ones that fail.
    template <typename Registry,
              typename Loader>
    void load_resources(Registry                               &registry,
                        const std::filesystem::path            &res_dir,
                        const std::vector<resource_definition> &definitions,
                        const char                             *kind,
                        Loader                                  load)
    {
        for (const auto &definition : definitions)
        {
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
        lua_getglobal(L, "st");
        lua_rawsetp(L, LUA_REGISTRYINDEX, &st_table_key);
        set_init_table(L, true);
    }

    ~mod_impl() noexcept
    {
#if ST_MODULE_AUDIO
        st::audio::sound_handle::set_registry(nullptr);
#endif
#if ST_MODULE_VIDEO
        st::video::texture_handle::set_registry(nullptr);
#endif
    }

    // Points the process-wide Lua bindings at this mod before running its code.
    void activate() noexcept
    {
#if ST_MODULE_AUDIO
        st::audio::sound_handle::set_registry(&m_sounds);
#endif
#if ST_MODULE_VIDEO
        st::video::texture_handle::set_registry(&m_textures);
#endif
    }

    std::string m_name;
    std::string m_display_name;
    std::string m_version;
    std::string m_author;
    std::string m_description;
    // The Lua folder of the mod; the `res` folder sits next to it.
    std::filesystem::path m_root_dir;
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
st::mod::author() noexcept
{
    return m_impl->m_author;
}

std::string
st::mod::description() noexcept
{
    return m_impl->m_description;
}

std::string
st::mod::display_name() noexcept
{
    return m_impl->m_display_name;
}

std::string
st::mod::name() noexcept
{
    return m_impl->m_name;
}

st::bool_t
st::mod::execute_init()
{
    m_impl->activate();

    lua_State *L = m_impl->m_runtime.state();

    set_init_table(L, false);
    const st::bool_t ok = m_impl->m_runtime.execute_file("init.lua");
    const auto       textures = ok ? collect_definitions(L, "textures") : std::vector<resource_definition>{};
    const auto       sounds = ok ? collect_definitions(L, "sounds") : std::vector<resource_definition>{};
    set_init_table(L, true);

    if (!ok)
        return false;

    const std::filesystem::path res_dir = m_impl->m_root_dir.parent_path() / "res";
#if ST_MODULE_VIDEO
    load_resources(m_impl->m_textures, res_dir, textures, "Texture", st::video::texture::load_file);
#else
    if (!textures.empty())
        st::io::log::warn("Textures were declared but the video module is disabled.");
#endif
#if ST_MODULE_AUDIO
    load_resources(m_impl->m_sounds, res_dir, sounds, "Sound", st::audio::sound::load_file);
#else
    if (!sounds.empty())
        st::io::log::warn("Sounds were declared but the audio module is disabled.");
#endif
    return true;
}

st::bool_t
st::mod::execute_runtime()
{
    m_impl->activate();

    return m_impl->m_runtime.execute_file("runtime.lua");
}

st::bool_t
st::mod::execute_function(std::string_view name) noexcept
{
    m_impl->activate();
    return m_impl->m_runtime.execute_function(name);
}

st::bool_t
st::mod::execute_function(std::string_view                                            name,
                          std::initializer_list<const st::lua::runtime::lua_arg_type> args) noexcept
{
    m_impl->activate();
    return m_impl->m_runtime.execute_function(name, args);
}

std::string
st::mod::version() noexcept
{
    return m_impl->m_version;
}
