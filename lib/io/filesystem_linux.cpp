/*
 * filesystem_linux.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <system_error>

#include "io/filesystem.h"

st::bool_t
st::io::filesystem::is_symlink(const std::filesystem::path &path,
                               std::error_code             &ec) noexcept
{
    return std::filesystem::is_symlink(path, ec);
}

std::filesystem::path
st::io::filesystem::resolve(const std::filesystem::path &path,
                            std::error_code             &ec) noexcept
{
    return std::filesystem::canonical(path, ec);
}

std::filesystem::path
st::io::filesystem::executable_dir() noexcept
{
    std::error_code ec;
    const auto      exe = std::filesystem::read_symlink("/proc/self/exe", ec);
    if (ec)
        return ".";
    return exe.parent_path();
}
