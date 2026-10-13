/*
 * filesystem.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 09/10/2026
 */

/**
 * @file
 * @brief Non-throwing filesystem queries that treat symbolic links safely.
 */

#ifndef STICKY_IO_FILESYSTEM_H
#define STICKY_IO_FILESYSTEM_H

#ifndef __cplusplus
#error "C++ compiler required to include this header."
#endif // __cplusplus

#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

#include "st_types.h"

/** @addtogroup io
 *  @{
 */

/**
 * @brief Filesystem helpers that never throw.
 *
 * Strings returned by these functions are UTF-8 on every platform. Errors such as missing files or denied access are
 * reported as false or an empty result.
 */
namespace st::io::filesystem
{

    /**
     * @brief Whether something exists at the path. A dangling symbolic link does not exist.
     * @param path Path to test.
     */
    [[nodiscard]] st::bool_t exists(const std::filesystem::path &path) noexcept;
    /** @brief Whether the path is a directory, or a link to one. @param path Path to test. */
    [[nodiscard]] st::bool_t is_directory(const std::filesystem::path &path) noexcept;
    /** @brief Whether the path is a regular file, or a link to one. @param path Path to test. */
    [[nodiscard]] st::bool_t is_file(const std::filesystem::path &path) noexcept;

    /** @brief The last component of the path, such as `file.txt`. @param path Path to split. */
    [[nodiscard]] std::string basename(const std::filesystem::path &path) noexcept;
    /** @brief Everything but the last component of the path. @param path Path to split. */
    [[nodiscard]] std::string dirname(const std::filesystem::path &path) noexcept;
    /** @brief The extension of the last component, with its dot, such as `.txt`. @param path Path to split. */
    [[nodiscard]] std::string extension(const std::filesystem::path &path) noexcept;
    /** @brief The last component without its extension, such as `file`. @param path Path to split. */
    [[nodiscard]] std::string stem(const std::filesystem::path &path) noexcept;

    /**
     * @brief Whether the path itself is a symbolic link (or, on Windows, any reparse point such as a junction).
     * @param path Path to test. The link is not followed.
     * @param ec Set when the path cannot be inspected, cleared otherwise.
     */
    [[nodiscard]] st::bool_t is_symlink(const std::filesystem::path &path,
                                        std::error_code             &ec) noexcept;

    /**
     * @brief Resolves every symbolic link and relative component of an existing path.
     * @param path Path to resolve.
     * @param ec Set when the path does not exist or cannot be resolved, cleared otherwise.
     * @return The absolute, canonical path, or an empty path on error.
     */
    [[nodiscard]] std::filesystem::path resolve(const std::filesystem::path &path,
                                                std::error_code             &ec) noexcept;

    /**
     * @brief Resolves a path that must stay within a root directory, even through symbolic links.
     * @param root Directory the result must be inside.
     * @param requested Path relative to the root, or an absolute path.
     * @param resolved Receives the canonical path on success.
     * @return True if the path resolves to a regular file inside the root.
     */
    [[nodiscard]] st::bool_t resolve_file(const std::filesystem::path &root,
                                          const std::filesystem::path &requested,
                                          std::filesystem::path       &resolved) noexcept;

    /**
     * @brief Lists the entries of a directory.
     * @param path Directory to list.
     * @param follow_symlinks Whether links are listed, as their resolved targets. Links are skipped otherwise.
     * @return The paths of the entries in no particular order, or nothing if the directory cannot be read.
     */
    [[nodiscard]] std::vector<std::string> list_all(const std::filesystem::path &path,
                                                    st::bool_t                   follow_symlinks = false) noexcept;
    /** @brief Lists the regular files of a directory. @copydetails list_all */
    [[nodiscard]] std::vector<std::string> list_files(const std::filesystem::path &path,
                                                      st::bool_t                   follow_symlinks = false) noexcept;
    /** @brief Lists the subdirectories of a directory. @copydetails list_all */
    [[nodiscard]] std::vector<std::string> list_subdirs(const std::filesystem::path &path,
                                                        st::bool_t                   follow_symlinks = false) noexcept;

    /**
     * @brief The directory that holds the running executable.
     *
     * Logs and settings are written here, so that the engine keeps everything in its own folder.
     * @return The directory, or the current directory if it cannot be determined.
     */
    [[nodiscard]] std::filesystem::path executable_dir() noexcept;

} // namespace st::io::filesystem

/** @} */

#endif // STICKY_IO_FILESYSTEM_H
