/*
 * popup_linux.cpp
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#if ST_MODULE_VIDEO
#include <SDL3/SDL.h>
#endif

#include "os/popup.h"
#include "st_macros.h"

#if ST_MODULE_VIDEO
#define TYPE_INFO SDL_MESSAGEBOX_INFORMATION
#define TYPE_WARNING SDL_MESSAGEBOX_WARNING
#define TYPE_ERROR SDL_MESSAGEBOX_ERROR

static st_bool
_st_os_popup_impl(const st_char *title,
                  const st_char *message,
                  st_enum        type)
{
    return ST_LIKELY(
        SDL_ShowSimpleMessageBox((SDL_MessageBoxFlags) type, (const char *) title, (const char *) message, nullptr));
}
#else
#define TYPE_INFO 0
#define TYPE_WARNING 1
#define TYPE_ERROR 2

static st_bool
_st_os_popup_impl(const st_char *title,
                  const st_char *message,
                  st_enum        type)
{
    (void) title;
    (void) message;
    (void) type;
    return 1;
}
#endif // ST_MODULE_VIDEO

st_bool
_st_os_popup_impl_info(const st_char *title,
                       const st_char *message)
{
    return _st_os_popup_impl(title, message, TYPE_INFO);
}

st_bool
_st_os_popup_impl_warn(const st_char *title,
                       const st_char *message)
{
    return _st_os_popup_impl(title, message, TYPE_WARNING);
}

st_bool
_st_os_popup_impl_error(const st_char *title,
                        const st_char *message)
{
    return _st_os_popup_impl(title, message, TYPE_ERROR);
}
