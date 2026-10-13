/*
 * resource_registry.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief Storage of named resources.
 */

#ifndef STICKY_RESOURCE_RESOURCE_REGISTRY_H
#define STICKY_RESOURCE_RESOURCE_REGISTRY_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "st_macros.h"
#include "st_types.h"

/** @addtogroup resource
 *  @{
 */

namespace st
{
    /**
     * @brief Owns the handles loaded by one mod, keyed by their unique name.
     *
     * @tparam Handle A st::resource_handle subclass, constructible from a name, a path and a shared resource.
     */
    template <typename Handle>
    class resource_registry
    {
    public:
        typedef typename Handle::resource_type resource_type; ///< Type of the resources.

        resource_registry() noexcept = default;
        ~resource_registry() noexcept = default;

        ST_DELETE_COPY_CTORS(resource_registry);
        ST_DELETE_MOVE_CTORS(resource_registry);

        /**
         * @brief Adds a resource under a name.
         * @param name Unique name of the resource.
         * @param path Path the resource was loaded from, relative to the `res` folder of the mod.
         * @param resource The resource.
         * @return False, and nothing is added, if the name is empty or in use or the resource is null.
         */
        st::bool_t add(std::string_view                      name,
                       const std::filesystem::path          &path,
                       const std::shared_ptr<resource_type> &resource) noexcept
        {
            if (name.empty() || resource == nullptr)
                return false;

            const std::string key(name);
            if (m_handles.find(key) != m_handles.end())
                return false;

            m_handles.emplace(key, std::make_unique<Handle>(name, path, resource));
            return true;
        }

        /**
         * @brief Looks up a resource by name.
         * @param name Name given to add().
         * @return The handle, or null if no resource has that name.
         */
        [[nodiscard]] const Handle *find(std::string_view name) const noexcept
        {
            const auto it = m_handles.find(std::string(name));
            if (it == m_handles.end())
                return nullptr;
            return it->second.get();
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<Handle>> m_handles;
    };
} // namespace st

/** @} */

#endif // STICKY_RESOURCE_RESOURCE_REGISTRY_H
