/*
 * filesystem.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 09/10/2026
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

namespace st::io::filesystem
{

    [[nodiscard]] st::bool_t exists(const std::filesystem::path &path) noexcept;
    [[nodiscard]] st::bool_t is_directory(const std::filesystem::path &path) noexcept;
    [[nodiscard]] st::bool_t is_file(const std::filesystem::path &path) noexcept;

    [[nodiscard]] std::string basename(const std::filesystem::path &path) noexcept;
    [[nodiscard]] std::string dirname(const std::filesystem::path &path) noexcept;
    [[nodiscard]] std::string extension(const std::filesystem::path &path) noexcept;
    [[nodiscard]] std::string stem(const std::filesystem::path &path) noexcept;

    [[nodiscard]] st::bool_t            is_symlink(const std::filesystem::path &path,
                                                   std::error_code             &ec) noexcept;
    [[nodiscard]] std::filesystem::path resolve(const std::filesystem::path &path,
                                                std::error_code             &ec) noexcept;
    [[nodiscard]] st::bool_t            resolve_file(const std::filesystem::path &root,
                                                     const std::filesystem::path &requested,
                                                     std::filesystem::path       &resolved) noexcept;

    [[nodiscard]] std::vector<std::string> list_all(const std::filesystem::path &path,
                                                    st::bool_t                   follow_symlinks = false) noexcept;
    [[nodiscard]] std::vector<std::string> list_files(const std::filesystem::path &path,
                                                      st::bool_t                   follow_symlinks = false) noexcept;
    [[nodiscard]] std::vector<std::string> list_subdirs(const std::filesystem::path &path,
                                                        st::bool_t                   follow_symlinks = false) noexcept;

} // namespace st::io::filesystem

#endif // STICKY_FILESYSTEM_H
