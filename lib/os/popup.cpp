/*
 * popup.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include "st_macros.h"
#include "io/log.h"
#include "os/popup.h"

extern "C" st_bool _st_os_popup_impl_info (const st_char *, const st_char *) noexcept;
extern "C" st_bool _st_os_popup_impl_warn (const st_char *, const st_char *) noexcept;
extern "C" st_bool _st_os_popup_impl_error(const st_char *, const st_char *) noexcept;

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
st::os::popup::info(const std::string &message) noexcept
{
    info(PROGRAM_TITLE, message);
}

void
st::os::popup::warn(const std::string &message) noexcept
{
    warn(PROGRAM_TITLE, message);
}

void
st::os::popup::error(const std::string &message) noexcept
{
    error(PROGRAM_TITLE, message);
}

void
st::os::popup::info(const std::string &title,
                    const std::string &message) noexcept
{
    st::io::log::info(message);
    ST_ASSERT(_st_os_popup_impl_info(title.c_str(), message.c_str()));
}

void
st::os::popup::warn(const std::string &title,
                    const std::string &message) noexcept
{
    st::io::log::warn(message);
    ST_ASSERT(_st_os_popup_impl_warn(title.c_str(), message.c_str()));
}

void
st::os::popup::error(const std::string &title,
                     const std::string &message) noexcept
{
    st::io::log::error(message);
    ST_ASSERT(_st_os_popup_impl_error(title.c_str(), message.c_str()));
}
