/*
 * config.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief Persistent settings and the per-owner view of them.
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

/** @addtogroup util
 *  @{
 */

namespace st::util
{

    /** @brief A flat set of named settings backed by a JSON file. */
    class config
    {
    public:
        /** @brief The value of a setting: a string, a number or a boolean. */
        using value = std::variant<std::string, st::float64, st::bool_t>;

        config() noexcept;
        ~config() noexcept;

        ST_DELETE_COPY_CTORS(config);
        ST_DELETE_MOVE_CTORS(config);

        /**
         * @brief Replaces the settings with the contents of a JSON file.
         * @param path The file.
         * @return True if the file was read or does not exist, which leaves the config empty. False, with the config
         *         empty and the reason logged, if it is unreadable or not a JSON object.
         */
        [[nodiscard]] st::bool_t load(const std::filesystem::path &path) noexcept;
        /**
         * @brief Writes the settings to a JSON file.
         *
         * The file is replaced in one step, so a failed write leaves the previous file intact.
         *
         * @param path The file.
         * @return False, with the reason logged, if the file could not be written.
         */
        [[nodiscard]] st::bool_t save(const std::filesystem::path &path) const noexcept;

        /**
         * @brief The value of a key, or nothing if it is missing or not a string, number or boolean.
         * @param key The key.
         */
        [[nodiscard]] std::optional<value> get(std::string_view key) const noexcept;
        /** @brief Stores a value. @param key The key. @param setting The value. */
        void set(std::string_view key,
                 const value     &setting) noexcept;
        /**
         * @brief Stores a fallback unless the key already holds a value of the same type.
         * @param key The key. @param fallback The value to store.
         * @return True if the fallback was stored.
         */
        st::bool_t set_default(std::string_view key,
                               const value     &fallback) noexcept;
        /**
         * @brief Claims a key for a single owner and applies set_default().
         * @param key The key. @param fallback The default value.
         * @return False, changing nothing, if the key was already claimed.
         */
        st::bool_t declare(std::string_view key,
                           const value     &fallback) noexcept;
        /** @brief Whether a key was claimed with declare(). @param key The key. */
        [[nodiscard]] st::bool_t is_declared(std::string_view key) const noexcept;

    private:
        struct config_impl;
        std::unique_ptr<config_impl> m_impl;
    };

    /**
     * @brief The settings one owner, such as a mod, may use.
     *
     * An owner reads any declared setting but only writes the ones it declared itself. Settings are stored under the
     * key `<owner>.<name>`.
     */
    class config_scope
    {
    public:
        /** @brief Creates a view. @param config The settings, which must outlive the view. @param owner Owner name. */
        config_scope(config          &config,
                     std::string_view owner) noexcept;
        ~config_scope() noexcept;

        ST_DELETE_COPY_CTORS(config_scope);
        ST_DELETE_MOVE_CTORS(config_scope);

        /**
         * @brief Declares one of the owner's settings.
         * @param name Short name of the setting. @param fallback Default value, which also fixes its type.
         * @return False, with the reason logged, if another owner declared the key.
         */
        st::bool_t declare(std::string_view     name,
                           const config::value &fallback) noexcept;
        /** @brief A setting of the owner, or nothing if it did not declare it. @param name Short name. */
        [[nodiscard]] std::optional<config::value> get(std::string_view name) const noexcept;
        /**
         * @brief A setting of another owner.
         * @param owner Owner name. @param name Short name.
         * @return The value, or nothing if that owner did not declare it.
         */
        [[nodiscard]] std::optional<config::value> get_from(std::string_view owner,
                                                            std::string_view name) const noexcept;
        /**
         * @brief Changes one of the owner's settings.
         * @param name Short name. @param setting New value.
         * @return False, with the reason logged, if the owner did not declare the setting, the type differs from the
         *         declared one or the value is not a finite number.
         */
        st::bool_t set(std::string_view     name,
                       const config::value &setting) noexcept;

        /**
         * @brief The full key of a setting: the owner name and a full stop, unless it already ends with one, then the
         *        short name.
         * @param owner Owner name. @param name Short name.
         */
        [[nodiscard]] static std::string key_for(std::string_view owner,
                                                 std::string_view name) noexcept;

    private:
        struct config_scope_impl;
        std::unique_ptr<config_scope_impl> m_impl;
    };

} // namespace st::util

/** @} */

#endif // STICKY_UTIL_CONFIG_H
