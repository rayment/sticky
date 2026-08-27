/*
 * runtime.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#ifndef STICKY_LUA_RUNTIME_H
#define STICKY_LUA_RUNTIME_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <filesystem>
#include <memory>
#include <string_view>
#include <variant>

#include "st_macros.h"

namespace st::lua
{

    class runtime
    {
    public:
        typedef std::variant<std::monostate, st::bool_t, st::int64, st::float64, std::string_view> lua_arg_type;

        runtime(std::string_view name) noexcept;
        ~runtime() noexcept;

        ST_DELETE_COPY_CTORS(runtime);
        ST_DELETE_MOVE_CTORS(runtime);

        st::bool_t execute_file    (const std::filesystem::path &path) noexcept;
        st::bool_t execute_function(std::string_view name) noexcept;
        st::bool_t execute_function(std::string_view name, std::initializer_list<const lua_arg_type> args) noexcept;
        st::bool_t execute_string  (std::string_view code) noexcept;

    private:
        struct runtime_impl;
        std::unique_ptr<runtime_impl> m_impl;
    };

}

#endif // STICKY_LUA_RUNTIME_H
