/*
 * popup.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#ifndef STICKY_OS_POPUP_H
#define STICKY_OS_POPUP_H

#include "st_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void st_os_popup_info (const st_char *title, const st_char *message);
void st_os_popup_warn (const st_char *title, const st_char *message);
void st_os_popup_error(const st_char *title, const st_char *message);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <string>

namespace st::os::popup
{
    void info (const std::string &title, const std::string &message) noexcept;
    void warn (const std::string &title, const std::string &message) noexcept;
    void error(const std::string &title, const std::string &message) noexcept;

    void info (const std::string &message) noexcept;
    void warn (const std::string &message) noexcept;
    void error(const std::string &message) noexcept;
}
#endif // __cplusplus

#endif // STICKY_OS_POPUP_H
