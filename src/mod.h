/*
 * mod.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

/**
 * @file
 * @brief A mod: its metadata, its Lua runtime and the resources it loaded.
 */

#ifndef STICKY_MOD_H
#define STICKY_MOD_H

#include <filesystem>
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "lua/runtime.h"
#include "util/config.h"

/** @addtogroup app
 *  @{
 */

namespace st
{

    /** @brief A setting declared by a mod in `st.init.config`. */
    struct config_setting
    {
        std::string key; ///< The full key in the config, `<mod>.<name>`.
        std::string description; ///< Text shown to the user, possibly empty.
    };

    /**
     * @brief One mod and the Lua state its scripts run in.
     *
     * Run execute_init(), then bind_config(), then execute_runtime(). After that, the engine calls global Lua
     * functions such as `on_step` with execute_function().
     */
    class mod
    {
    public:
        /**
         * @brief Creates the mod and its Lua state. No script runs yet.
         * @param name Unique name, which prefixes its settings and names its logger.
         * @param display_name Name shown to the user.
         * @param version Version shown to the user.
         * @param author Author shown to the user.
         * @param description Description shown to the user.
         * @param root_dir The `lua` folder of the mod. Its `res` folder is next to it.
         */
        mod(std::string_view             name,
            std::string_view             display_name,
            std::string_view             version,
            std::string_view             author,
            std::string_view             description,
            const std::filesystem::path &root_dir) noexcept;
        ~mod() noexcept;

        ST_DELETE_COPY_CTORS(mod);
        ST_DELETE_MOVE_CTORS(mod);

        /** @brief The author. */
        [[nodiscard]] std::string author() const noexcept;
        /** @brief The description. */
        [[nodiscard]] std::string description() const noexcept;
        /** @brief The name shown to the user. */
        [[nodiscard]] std::string display_name() const noexcept;
        /** @brief The unique name. */
        [[nodiscard]] std::string name() const noexcept;
        /** @brief The version. */
        [[nodiscard]] std::string version() const noexcept;

        /** @brief Receives a status line and the fraction, from 0 to 1, of the init phase completed. */
        using progress_callback = std::function<void(std::string_view, st::float32)>;

        /**
         * @brief Runs `init.lua` with `st.init` available, then loads the textures and sounds it declared.
         *
         * Resources that fail to load are logged and skipped.
         *
         * @param progress Receives progress updates, or empty.
         * @return False if `init.lua` failed.
         */
        st::bool_t execute_init(const progress_callback &progress = {}) noexcept;
        /**
         * @brief Declares the settings of `st.init.config` and makes them available to `st.util.config`.
         *
         * Call after execute_init() and before execute_runtime().
         *
         * @param config The settings, which must outlive the mod.
         */
        void bind_config(st::util::config &config) noexcept;
        /**
         * @brief Runs `runtime.lua`, with `st.init` locked.
         * @return False if `runtime.lua` failed.
         */
        st::bool_t execute_runtime() noexcept;
        /** @brief The settings declared by the last init phase, in declaration order. */
        [[nodiscard]] std::vector<config_setting> config_settings() const noexcept;
        /**
         * @brief Calls a global function of the mod without arguments.
         * @param name Name of the global.
         * @return False if the global is not a function or the call raised an error.
         */
        st::bool_t execute_function(std::string_view name) noexcept;
        /**
         * @brief Calls a global function of the mod.
         * @param name Name of the global.
         * @param args Arguments of the call, in order.
         * @return False if the global is not a function or the call raised an error.
         */
        st::bool_t execute_function(std::string_view                                            name,
                                    std::initializer_list<const st::lua::runtime::lua_arg_type> args) noexcept;

    private:
        struct mod_impl;
        std::unique_ptr<mod_impl> m_impl;
    };

} // namespace st

/** @} */

#endif // STICKY_MOD_H
