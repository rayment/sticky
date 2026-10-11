/*
 * filesystem.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 09/10/2026
 */

#include <system_error>

#include "io/filesystem.h"

st::bool_t
st::io::filesystem::exists(const std::filesystem::path &path) noexcept
{
    return std::filesystem::exists(path);
}

st::bool_t
st::io::filesystem::is_directory(const std::filesystem::path &path) noexcept
{
    return std::filesystem::is_directory(path);
}

st::bool_t
st::io::filesystem::is_file(const std::filesystem::path &path) noexcept
{
    return std::filesystem::is_regular_file(path);
}

std::string
st::io::filesystem::basename(const std::filesystem::path &path) noexcept
{
    return path.filename().string();
}

std::string
st::io::filesystem::dirname(const std::filesystem::path &path) noexcept
{
    return path.parent_path().string();
}

std::string
st::io::filesystem::extension(const std::filesystem::path &path) noexcept
{
    return path.extension().string();
}

std::string
st::io::filesystem::stem(const std::filesystem::path &path) noexcept
{
    return path.stem().string();
}

std::vector<std::string>
st::io::filesystem::list_all(const std::filesystem::path &path,
                             st::bool_t follow_symlinks) noexcept
{
    std::vector<std::string> entries;

    std::error_code ec;
    for (std::filesystem::directory_iterator it(path, ec), end;
         !ec && it != end;
         it.increment(ec))
    {
        const std::filesystem::path entry = it->path();

        std::error_code type_ec;
        const bool is_symlink = it->is_symlink(type_ec);

        if (type_ec)
            continue;

        if (is_symlink)
        {
            if (!follow_symlinks)
                continue;

            std::error_code canonical_ec;
            std::filesystem::path resolved = std::filesystem::canonical(entry, canonical_ec);

            if (canonical_ec)
                continue;

            entries.push_back(resolved.string());
        }
        else
        {
            entries.push_back(entry.string());
        }
    }

    return entries;
}

std::vector<std::string>
st::io::filesystem::list_files(const std::filesystem::path &path,
                               st::bool_t follow_symlinks) noexcept
{
    std::vector<std::string> files;

    std::error_code ec;
    for (std::filesystem::directory_iterator it(path, ec), end; !ec && it != end; it.increment(ec))
    {
        std::error_code type_ec;
        const bool is_symlink = it->is_symlink(type_ec);

        if (type_ec || (is_symlink && !follow_symlinks))
            continue;

        if (is_symlink)
        {
            std::error_code canonical_ec;
            std::filesystem::path resolved = std::filesystem::canonical(it->path(), canonical_ec);

            if (canonical_ec)
                continue;

            std::error_code status_ec;
            if (std::filesystem::is_regular_file(resolved, status_ec) && !status_ec)
                files.push_back(resolved.string());
        }
        else if (it->is_regular_file(type_ec) && !type_ec)
        {
            files.push_back(it->path().string());
        }
    }

    return files;
}

std::vector<std::string>
st::io::filesystem::list_subdirs(const std::filesystem::path &path,
                                 st::bool_t follow_symlinks) noexcept
{
    std::vector<std::string> dirs;

    std::error_code ec;
    for (std::filesystem::directory_iterator it(path, ec), end; !ec && it != end; it.increment(ec))
    {
        std::error_code type_ec;
        const bool is_symlink = it->is_symlink(type_ec);

        if (type_ec || (is_symlink && !follow_symlinks))
            continue;

        if (is_symlink)
        {
            std::error_code canonical_ec;
            std::filesystem::path resolved = std::filesystem::canonical(it->path(), canonical_ec);

            if (canonical_ec)
                continue;

            std::error_code status_ec;
            if (std::filesystem::is_directory(resolved, status_ec) && !status_ec)
                dirs.push_back(resolved.string());
        }
        else if (it->is_directory(type_ec) && !type_ec)
        {
            dirs.push_back(it->path().string());
        }
    }

    return dirs;
}
