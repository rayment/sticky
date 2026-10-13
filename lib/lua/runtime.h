/*
 * runtime.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

/**
 * @file
 * @brief A sandboxed Lua state with the engine API loaded.
 */

#ifndef STICKY_LUA_RUNTIME_H
#define STICKY_LUA_RUNTIME_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <filesystem>
#include <initializer_list>
#include <memory>
#include <string_view>
#include <variant>

#include "st_macros.h"
#include "st_types.h"

struct lua_State;

/** @addtogroup lua
 *  @{
 */

namespace st::lua
{

    /**
     * @brief A Lua state that runs the scripts of one mod.
     *
     * The state only offers the safe standard libraries, a `require` restricted to the root directory and the `st`
     * engine module. Script errors never propagate: they are logged with a traceback to a logger named after the
     * runtime, and the call reports failure. See the Lua manual page "Built-in Lua runtime" for the exact environment.
     */
    class runtime
    {
    public:
        /** @brief A value that can be passed from C++ to a Lua function. std::monostate is passed as `nil`. */
        typedef std::variant<std::monostate, st::bool_t, st::int64, st::float64, std::string_view> lua_arg_type;

        /**
         * @brief Creates the Lua state.
         * @param name Name of the logger that receives the output and errors of the scripts.
         * @param root_dir Directory that scripts are loaded from. Nothing outside of it can be loaded.
         */
        runtime(std::string_view             name,
                const std::filesystem::path &root_dir) noexcept;
        ~runtime() noexcept;

        ST_DELETE_COPY_CTORS(runtime);
        ST_DELETE_MOVE_CTORS(runtime);

        /**
         * @brief Loads and runs a Lua source file.
         * @param path File relative to the root directory, or an absolute path inside it.
         * @return False if the file is outside the root directory, cannot be read, is not Lua source or raised an
         *         error.
         */
        st::bool_t execute_file(const std::filesystem::path &path) noexcept;

        /**
         * @brief Calls a global function without arguments.
         * @param name Name of the global.
         * @return False if the global is not a function or the call raised an error.
         */
        st::bool_t execute_function(std::string_view name) noexcept;

        /**
         * @brief Calls a global function.
         * @param name Name of the global.
         * @param args Arguments of the call, in order.
         * @return False if the global is not a function or the call raised an error.
         */
        st::bool_t execute_function(std::string_view                          name,
                                    std::initializer_list<const lua_arg_type> args) noexcept;

        /**
         * @brief Runs a string of Lua source code.
         * @param code The source code.
         * @return False if the code does not compile or raised an error.
         */
        st::bool_t execute_string(std::string_view code) noexcept;

        /**
         * @brief The raw Lua state, for embedders that extend the environment natively.
         * @return The state, or null if it could not be allocated.
         */
        [[nodiscard]] lua_State *state() noexcept;

    private:
        struct runtime_impl;
        std::unique_ptr<runtime_impl> m_impl;
    };

} // namespace st::lua

/** @} */

#endif // STICKY_LUA_RUNTIME_H
