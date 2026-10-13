/*
 * symlink.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

/**
 * @file
 * @brief Creation and removal of symbolic links for the filesystem tests.
 */

#ifndef STICKY_TEST_SYMLINK_H
#define STICKY_TEST_SYMLINK_H

#include <filesystem>
#include <system_error>

#ifdef _WIN32
#include <windows.h>
#endif

namespace st::test
{
    /**
     * @brief Creates a symbolic link. libstdc++ on MinGW does not implement std::filesystem::create_symlink.
     * @param target What the link points to.
     * @param link Path of the new link.
     * @param directory Whether the target is a directory.
     * @param ec Set on failure, such as when the user may not create links.
     */
    inline void make_symlink(const std::filesystem::path &target,
                             const std::filesystem::path &link,
                             bool                         directory,
                             std::error_code             &ec)
    {
#ifdef _WIN32
        DWORD flags = SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;
        if (directory)
            flags |= SYMBOLIC_LINK_FLAG_DIRECTORY;
        if (CreateSymbolicLinkW(link.c_str(), target.c_str(), flags) != 0)
            ec.clear();
        else
            ec.assign(static_cast<int>(GetLastError()), std::system_category());
#else
        if (directory)
            std::filesystem::create_directory_symlink(target, link, ec);
        else
            std::filesystem::create_symlink(target, link, ec);
#endif
    }

    /** @brief Removes a link created by make_symlink(), leaving its target alone. @param link Path of the link. */
    inline void remove_link(const std::filesystem::path &link)
    {
#ifdef _WIN32
        if (DeleteFileW(link.c_str()) == 0)
            RemoveDirectoryW(link.c_str());
#else
        std::error_code ec;
        std::filesystem::remove(link, ec);
#endif
    }
} // namespace st::test

#endif // STICKY_TEST_SYMLINK_H
