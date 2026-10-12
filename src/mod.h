/*
 * mod.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/10/2026
 */

#ifndef STICKY_MOD_H
#define STICKY_MOD_H

#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include "lua/runtime.h"
#include "util/config.h"

namespace st
{

    class mod
    {
    public:
        mod(std::string_view             name,
            std::string_view             display_name,
            std::string_view             version,
            std::string_view             author,
            std::string_view             description,
            const std::filesystem::path &root_dir) noexcept;
        ~mod() noexcept;

        ST_DELETE_COPY_CTORS(mod);
        ST_DELETE_MOVE_CTORS(mod);

        [[nodiscard]] std::string author() noexcept;
        [[nodiscard]] std::string description() noexcept;
        [[nodiscard]] std::string display_name() noexcept;
        [[nodiscard]] std::string name() noexcept;
        [[nodiscard]] std::string version() noexcept;

        // Receives a status line and the fraction in [0, 1] of the init phase completed.
        using progress_callback = std::function<void(std::string_view, st::float32)>;

        // Runs `init.lua` with `st.init` available, then loads the textures and sounds it declared.
        st::bool_t execute_init(const progress_callback &progress = {});
        // Adds the settings declared in `st.init.config` that the config does not already hold, prefixed by the mod
        // name.
        void register_config(st::util::config &config) const noexcept;
        // Runs `runtime.lua` with `st.init` locked.
        st::bool_t execute_runtime();
        st::bool_t execute_function(std::string_view name) noexcept;
        st::bool_t execute_function(std::string_view                                            name,
                                    std::initializer_list<const st::lua::runtime::lua_arg_type> args) noexcept;

    private:
        struct mod_impl;
        std::unique_ptr<mod_impl> m_impl;
    };

} // namespace st

#endif // STICKY_MOD_H
