/*
 * modloader.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 25/08/2026
 */

#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif

#include <dirent.h>
#include <sys/stat.h>

#include "log/log.h"
#include "lua/modloader.h"

static st_bool
lua_file_exists(st_string_t mod_path,
                st_string_t filename)
{
    ST_STRING_SAFE_CONCAT_CSTR(mod_path, filename, full_path);

    struct stat file_st;
    return stat(full_path, &file_st) == 0 && S_ISREG(file_st.st_mode);
}

st_bool
st_lua_modloader_load_single(st_lua_t L,
                             st_string_t mod_path)
{
    if (lua_file_exists(mod_path, ST_STRING("/init.lua")) &&
        lua_file_exists(mod_path, ST_STRING("/runtime.lua")))
    {
        st_log_info("Loading Lua mod from directory '%.*s'.", mod_path.len, mod_path.data);
        ST_STRING_SAFE_CONCAT(mod_path, ST_STRING("/init.lua"), init_path);
        if (!st_lua_exec_file(L, init_path))
        {
            st_log_error("Failed to execute Lua init.lua from directory '%.*s'.", mod_path.len, mod_path.data);
            return false;
        }
        ST_STRING_SAFE_CONCAT(mod_path, ST_STRING("/runtime.lua"), runtime_path);
        if (!st_lua_exec_file(L, runtime_path))
        {
            st_log_error("Failed to execute Lua runtime.lua from directory '%.*s'.", mod_path.len, mod_path.data);
            return false;
        }
        return true;
    }
    st_log_warn("Directory '%.*s' does not contain necessary Lua files to load mod.", mod_path.len, mod_path.data);
    return false;
}

st_bool
st_lua_modloader_load_all(st_lua_t L,
                          st_string_t mod_root_path)
{
    ST_STRING_SAFE_CSTR(mod_root_path, safe_path);

    DIR *dir = opendir(safe_path);
    if (dir == nullptr)
        return false;

    struct dirent *entry;

    while ((entry = readdir(dir)) != nullptr)
    {
        // skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;
        char mod_path[PATH_MAX];
        st_int32 mod_path_len = snprintf(mod_path, sizeof(mod_path), "%s%c%s", safe_path, PATH_SEPARATOR, entry->d_name);
        struct stat st;

        if (mod_path_len < 0)
            continue;
        if (stat(mod_path, &st) != 0)
            continue;
        if (!S_ISDIR(st.st_mode))
            continue;

        if (!st_lua_modloader_load_single(L, ST_STRING(mod_path, ST_CAST(st_size, mod_path_len))))
            return false;
    }

    return true;
}
