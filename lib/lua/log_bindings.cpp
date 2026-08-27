/*
 * log_bindings.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 28/08/2026
 */

#include "io/log.h"
#include "lua/log_bindings.h"

namespace
{
    st::io::log::logger *lua_logger = nullptr;
}

st::io::log::logger *st::lua::logger() noexcept { return lua_logger; }

void st::lua::set_logger(st::io::log::logger *logger) noexcept { lua_logger = logger; }

void st::lua::log::trace(std::string_view m) noexcept { if (lua_logger) lua_logger->trace(m); }
void st::lua::log::debug(std::string_view m) noexcept { if (lua_logger) lua_logger->debug(m); }
void st::lua::log::info (std::string_view m) noexcept { if (lua_logger) lua_logger->info(m);  }
void st::lua::log::warn (std::string_view m) noexcept { if (lua_logger) lua_logger->warn(m);  }
void st::lua::log::error(std::string_view m) noexcept { if (lua_logger) lua_logger->error(m); }
