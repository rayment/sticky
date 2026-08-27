/*
 * runtime.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#ifndef STICKY_RUNTIME_H
#define STICKY_RUNTIME_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <filesystem>
#include <memory>
#include <string_view>

#include "st_macros.h"

namespace st::lua
{

    class runtime
    {
    public:
        runtime(std::string_view name) noexcept;
        ~runtime() noexcept;

        ST_DELETE_COPY_CTORS(runtime);
        ST_DELETE_MOVE_CTORS(runtime);

        st::bool_t execute_file  (const std::filesystem::path &path) noexcept;
        st::bool_t execute_string(std::string_view code) noexcept;

    private:
        struct runtime_impl;
        std::unique_ptr<runtime_impl> m_impl;
    };

}

#endif // STICKY_RUNTIME_H
