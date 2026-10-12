/*
 * config.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#ifndef STICKY_UTIL_CONFIG_H
#define STICKY_UTIL_CONFIG_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include "st_macros.h"
#include "st_types.h"

namespace st::util
{

    // A flat set of named settings backed by a JSON file.
    class config
    {
    public:
        using value = std::variant<std::string, st::float64, st::bool_t>;

        config() noexcept;
        ~config() noexcept;

        ST_DELETE_COPY_CTORS(config);
        ST_DELETE_MOVE_CTORS(config);

        // Replaces the settings with the contents of the file. A missing file is not an error and leaves the config
        // empty. An unreadable or malformed file is logged, leaves the config empty and returns false.
        [[nodiscard]] st::bool_t load(const std::filesystem::path &path) noexcept;
        // Writes the settings to the file. Failure is logged and returns false.
        [[nodiscard]] st::bool_t save(const std::filesystem::path &path) const noexcept;

        // Returns nothing if the key is missing or does not hold a string, number or boolean.
        [[nodiscard]] std::optional<value> get(std::string_view key) const noexcept;
        void                               set(std::string_view key,
                                               const value     &setting) noexcept;
        // Stores the fallback unless the key already holds a value of the same type. Returns true if it was stored.
        st::bool_t set_default(std::string_view key,
                               const value     &fallback) noexcept;

    private:
        struct config_impl;
        std::unique_ptr<config_impl> m_impl;
    };

} // namespace st::util

#endif // STICKY_UTIL_CONFIG_H
