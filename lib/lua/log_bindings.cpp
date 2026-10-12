/*
 * log_bindings.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 28/08/2026
 */

extern "C" {
#include <lua.h>
}

#include "io/log.h"
#include "lua/log_bindings.h"

namespace
{
    char logger_key;

    [[nodiscard]] st::io::log::logger *logger_of(lua_State *L) noexcept
    {
        lua_rawgetp(L, LUA_REGISTRYINDEX, &logger_key);
        auto *logger = static_cast<st::io::log::logger *>(lua_touserdata(L, -1));
        lua_pop(L, 1);
        return logger;
    }
}

void st::lua::set_logger(lua_State *L, st::io::log::logger *logger) noexcept
{
    if (logger != nullptr)
        lua_pushlightuserdata(L, logger);
    else
        lua_pushnil(L);
    lua_rawsetp(L, LUA_REGISTRYINDEX, &logger_key);
}

void st::lua::log::trace(std::string_view m, lua_State *state) noexcept {
    if (auto *logger = logger_of(state))
        logger->trace(m);
}
void st::lua::log::debug(std::string_view m, lua_State *state) noexcept {
    if (auto *logger = logger_of(state))
        logger->debug(m);
}
void st::lua::log::info (std::string_view m, lua_State *state) noexcept {
    if (auto *logger = logger_of(state))
        logger->info(m);
}
void st::lua::log::warn (std::string_view m, lua_State *state) noexcept {
    if (auto *logger = logger_of(state))
        logger->warn(m);
}
void st::lua::log::error(std::string_view m, lua_State *state) noexcept {
    if (auto *logger = logger_of(state))
        logger->error(m);
}
