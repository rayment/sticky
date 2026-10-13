/*
 * filesystem_win32.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/10/2026
 */

#include <string>
#include <string_view>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include "io/filesystem.h"

namespace
{
    constexpr std::wstring_view verbatim_prefix = L"\\\\?\\";
    constexpr std::wstring_view unc_prefix = L"\\\\?\\UNC\\";
} // namespace

st::bool_t
st::io::filesystem::is_symlink(const std::filesystem::path &path,
                               std::error_code             &ec) noexcept
{
    const DWORD attributes = GetFileAttributesW(path.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
    {
        ec.assign(static_cast<int>(GetLastError()), std::system_category());
        return false;
    }

    ec.clear();
    return (attributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0;
}

std::filesystem::path
st::io::filesystem::resolve(const std::filesystem::path &path,
                            std::error_code             &ec) noexcept
{
    const HANDLE handle = CreateFileW(path.c_str(), 0, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
                                      OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);
    if (handle == INVALID_HANDLE_VALUE)
    {
        ec.assign(static_cast<int>(GetLastError()), std::system_category());
        return {};
    }

    std::wstring resolved(MAX_PATH, L'\0');
    DWORD        length =
        GetFinalPathNameByHandleW(handle, resolved.data(), static_cast<DWORD>(resolved.size()), FILE_NAME_NORMALIZED);
    if (length >= resolved.size())
    {
        resolved.resize(length);
        length = GetFinalPathNameByHandleW(handle, resolved.data(), static_cast<DWORD>(resolved.size()),
                                           FILE_NAME_NORMALIZED);
    }
    const DWORD error = GetLastError();
    CloseHandle(handle);

    if (length == 0 || length >= resolved.size())
    {
        ec.assign(static_cast<int>(error), std::system_category());
        return {};
    }
    resolved.resize(length);

    if (resolved.compare(0, unc_prefix.size(), unc_prefix) == 0)
        resolved = L"\\\\" + resolved.substr(unc_prefix.size());
    else if (resolved.compare(0, verbatim_prefix.size(), verbatim_prefix) == 0)
        resolved.erase(0, verbatim_prefix.size());

    ec.clear();
    return std::filesystem::path(resolved);
}

std::filesystem::path
st::io::filesystem::executable_dir() noexcept
{
    std::wstring buffer(MAX_PATH, L'\0');
    for (;;)
    {
        const DWORD length = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
        if (length == 0)
            return ".";
        if (length < buffer.size())
        {
            buffer.resize(length);
            return std::filesystem::path(buffer).parent_path();
        }
        buffer.resize(buffer.size() * 2);
    }
}
