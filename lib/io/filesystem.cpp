/*
 * filesystem.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 09/10/2026
 */

#include <system_error>

#include <SDL3/SDL.h>

#include "io/filesystem.h"

namespace
{
    // Some standard libraries stat the link itself rather than its target.
    st::bool_t is_dangling_symlink(const std::filesystem::path &path) noexcept
    {
        std::error_code ec;
        if (!st::io::filesystem::is_symlink(path, ec) || ec)
            return false;

        return st::io::filesystem::resolve(path, ec).empty();
    }

    st::bool_t is_within_directory(const std::filesystem::path &root,
                                   const std::filesystem::path &candidate) noexcept
    {
        auto root_it = root.begin();
        auto candidate_it = candidate.begin();

        for (; root_it != root.end(); ++root_it, ++candidate_it)
        {
            if (candidate_it == candidate.end() || *root_it != *candidate_it)
                return false;
        }

        return true;
    }
} // namespace

st::bool_t
st::io::filesystem::exists(const std::filesystem::path &path) noexcept
{
    if (is_dangling_symlink(path))
        return false;

    std::error_code ec;
    return std::filesystem::exists(path, ec) && !ec;
}

st::bool_t
st::io::filesystem::is_directory(const std::filesystem::path &path) noexcept
{
    if (is_dangling_symlink(path))
        return false;

    std::error_code ec;
    return std::filesystem::is_directory(path, ec) && !ec;
}

st::bool_t
st::io::filesystem::is_file(const std::filesystem::path &path) noexcept
{
    if (is_dangling_symlink(path))
        return false;

    std::error_code ec;
    return std::filesystem::is_regular_file(path, ec) && !ec;
}

std::string
st::io::filesystem::basename(const std::filesystem::path &path) noexcept
{
    return path.filename().u8string();
}

std::string
st::io::filesystem::dirname(const std::filesystem::path &path) noexcept
{
    return path.parent_path().u8string();
}

std::string
st::io::filesystem::extension(const std::filesystem::path &path) noexcept
{
    return path.extension().u8string();
}

std::string
st::io::filesystem::stem(const std::filesystem::path &path) noexcept
{
    return path.stem().u8string();
}

std::vector<std::string>
st::io::filesystem::list_all(const std::filesystem::path &path,
                             st::bool_t                   follow_symlinks) noexcept
{
    std::vector<std::string> entries;

    std::error_code ec;
    for (std::filesystem::directory_iterator it(path, ec), end; !ec && it != end; it.increment(ec))
    {
        const std::filesystem::path entry = it->path();

        std::error_code  type_ec;
        const st::bool_t symlink = st::io::filesystem::is_symlink(it->path(), type_ec);

        if (type_ec)
            continue;

        if (symlink)
        {
            if (!follow_symlinks)
                continue;

            std::error_code       canonical_ec;
            std::filesystem::path resolved = st::io::filesystem::resolve(entry, canonical_ec);

            if (canonical_ec)
                continue;

            entries.push_back(resolved.u8string());
        }
        else
        {
            entries.push_back(entry.u8string());
        }
    }

    return entries;
}

std::vector<std::string>
st::io::filesystem::list_files(const std::filesystem::path &path,
                               st::bool_t                   follow_symlinks) noexcept
{
    std::vector<std::string> files;

    std::error_code ec;
    for (std::filesystem::directory_iterator it(path, ec), end; !ec && it != end; it.increment(ec))
    {
        std::error_code  type_ec;
        const st::bool_t symlink = st::io::filesystem::is_symlink(it->path(), type_ec);

        if (type_ec || (symlink && !follow_symlinks))
            continue;

        if (symlink)
        {
            std::error_code       canonical_ec;
            std::filesystem::path resolved = st::io::filesystem::resolve(it->path(), canonical_ec);

            if (canonical_ec)
                continue;

            std::error_code status_ec;
            if (std::filesystem::is_regular_file(resolved, status_ec) && !status_ec)
                files.push_back(resolved.u8string());
        }
        else if (it->is_regular_file(type_ec) && !type_ec)
        {
            files.push_back(it->path().u8string());
        }
    }

    return files;
}

std::vector<std::string>
st::io::filesystem::list_subdirs(const std::filesystem::path &path,
                                 st::bool_t                   follow_symlinks) noexcept
{
    std::vector<std::string> dirs;

    std::error_code ec;
    for (std::filesystem::directory_iterator it(path, ec), end; !ec && it != end; it.increment(ec))
    {
        std::error_code  type_ec;
        const st::bool_t symlink = st::io::filesystem::is_symlink(it->path(), type_ec);

        if (type_ec || (symlink && !follow_symlinks))
            continue;

        if (symlink)
        {
            std::error_code       canonical_ec;
            std::filesystem::path resolved = st::io::filesystem::resolve(it->path(), canonical_ec);

            if (canonical_ec)
                continue;

            std::error_code status_ec;
            if (std::filesystem::is_directory(resolved, status_ec) && !status_ec)
                dirs.push_back(resolved.u8string());
        }
        else if (it->is_directory(type_ec) && !type_ec)
        {
            dirs.push_back(it->path().u8string());
        }
    }

    return dirs;
}

st::bool_t
// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
st::io::filesystem::resolve_file(const std::filesystem::path &root,
                                 const std::filesystem::path &requested,
                                 std::filesystem::path       &resolved) noexcept
{
    std::error_code ec;

    const auto canonical_root = resolve(root, ec);
    if (ec)
        return false;

    const auto candidate = requested.is_absolute() ? requested : canonical_root / requested;

    resolved = resolve(candidate, ec);
    if (ec || !is_within_directory(canonical_root, resolved))
        return false;

    return std::filesystem::is_regular_file(resolved, ec) && !ec;
}

std::filesystem::path
st::io::filesystem::executable_dir() noexcept
{
    // SDL caches the result and owns the string. It ends with a separator.
    const char *base = SDL_GetBasePath();
    if (base == nullptr)
        return ".";
    return std::filesystem::u8path(base);
}
