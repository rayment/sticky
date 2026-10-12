/*
 * resource_registry.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
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

namespace st
{
    // Owns the handles loaded by one mod, keyed by their unique name.
    // `Handle` derives from `st::resource_handle` and is constructible from
    // a name, a path and a shared resource.
    template <typename Handle>
    class resource_registry
    {
    public:
        typedef typename Handle::resource_type resource_type;

        resource_registry() noexcept = default;
        ~resource_registry() noexcept = default;

        ST_DELETE_COPY_CTORS(resource_registry);
        ST_DELETE_MOVE_CTORS(resource_registry);

        // Fails if the name is empty or already in use.
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

#endif // STICKY_RESOURCE_RESOURCE_REGISTRY_H
