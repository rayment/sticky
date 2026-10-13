/*
 * resource_handle.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief Base class of named resources.
 */

#ifndef STICKY_RESOURCE_RESOURCE_HANDLE_H
#define STICKY_RESOURCE_RESOURCE_HANDLE_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

#include "st_macros.h"

/** @addtogroup resource
 *  @{
 */

namespace st
{
    /**
     * @brief A named resource loaded during the init phase of a mod.
     *
     * Derive from this to expose resource-specific accessors to Lua.
     *
     * @tparam T Type of the resource.
     */
    template <typename T>
    class resource_handle
    {
    public:
        typedef T resource_type; ///< Type of the resource.

        /**
         * @brief Wraps a loaded resource.
         * @param name Unique name of the resource within its mod.
         * @param path Path the resource was loaded from, relative to the `res` folder of the mod.
         * @param resource The resource.
         */
        // NOLINTNEXTLINE(bugprone-exception-escape)
        resource_handle(std::string_view             name,
                        const std::filesystem::path &path,
                        const std::shared_ptr<T>    &resource) noexcept :
            m_name(name),
            m_path(path.generic_u8string()),
            m_resource(resource)
        {
        }
        ~resource_handle() noexcept = default;

        ST_DELETE_COPY_CTORS(resource_handle);
        ST_DELETE_MOVE_CTORS(resource_handle);

        /** @brief The name of the resource. */
        [[nodiscard]] std::string name() const noexcept { return m_name; }
        /** @brief The path of the resource relative to the `res` folder of the mod, with forward slashes. */
        [[nodiscard]] std::string path() const noexcept { return m_path; }
        /** @brief The resource. */
        [[nodiscard]] const std::shared_ptr<T> &shared() const noexcept { return m_resource; }

    private:
        std::string        m_name;
        std::string        m_path;
        std::shared_ptr<T> m_resource;
    };
} // namespace st

/** @} */

#endif // STICKY_RESOURCE_RESOURCE_HANDLE_H
