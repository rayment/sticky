/*
 * popup.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <cstdio>
#include <cstdlib>

#include <fmt/format.h>

#include "io/log.h"
#include "os/popup.h"
#include "st_macros.h"

extern "C" st_bool _st_os_popup_impl_info(const st_char *,
                                          const st_char *) noexcept;
extern "C" st_bool _st_os_popup_impl_warn(const st_char *,
                                          const st_char *) noexcept;
extern "C" st_bool _st_os_popup_impl_error(const st_char *,
                                           const st_char *) noexcept;

namespace
{
    st::bool_t popups_enabled = true;

    void show(st::bool_t (*impl)(const st::char_t *,
                                 const st::char_t *) noexcept,
              const std::string &title,
              const std::string &message) noexcept
    {
        if (!popups_enabled)
            return;
        if (!impl(title.c_str(), message.c_str()))
            st::io::log::error(fmt::format("Could not show a message box titled '{}'.", title));
    }
} // namespace

void
st_os_popup_info(const st_char *title,
                 const st_char *message)
{
    st::os::popup::info(title, message);
}

void
st_os_popup_warn(const st_char *title,
                 const st_char *message)
{
    st::os::popup::warn(title, message);
}

void
st_os_popup_error(const st_char *title,
                  const st_char *message)
{
    st::os::popup::error(title, message);
}

void
st_assert_fail(const st_char *expression,
               const st_char *file,
               st_int32       line)
{
    const std::string message = fmt::format("Assertion failed in {} on line {}:\n\n{}", file, line, expression);
    std::fputs(message.c_str(), stderr);
    std::fputc('\n', stderr);
    st::os::popup::error("Assertion failed", message);
    std::abort();
}

void
st::os::popup::set_enabled(st::bool_t enabled) noexcept
{
    popups_enabled = enabled;
}

st::bool_t
st::os::popup::is_enabled() noexcept
{
    return popups_enabled;
}

void
st::os::popup::info(const std::string &message) noexcept
{
    info("libsticky", message);
}

void
st::os::popup::warn(const std::string &message) noexcept
{
    warn("libsticky", message);
}

void
st::os::popup::error(const std::string &message) noexcept
{
    error("libsticky", message);
}

void
st::os::popup::info(const std::string &title,
                    const std::string &message) noexcept
{
    st::io::log::info(message);
    show(_st_os_popup_impl_info, title, message);
}

void
st::os::popup::warn(const std::string &title,
                    const std::string &message) noexcept
{
    st::io::log::warn(message);
    show(_st_os_popup_impl_warn, title, message);
}

void
st::os::popup::error(const std::string &title,
                     const std::string &message) noexcept
{
    st::io::log::error(message);
    show(_st_os_popup_impl_error, title, message);
}
