/*
 * popup.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/08/2026
 */

#include "os/popup.h"
#include "log/log.h"
#include "st_macros.h"

#ifdef _WIN32
#include <windows.h>

#define TYPE_INFO MB_ICONINFORMATION
#define TYPE_WARNING MB_ICONWARNING
#define TYPE_ERROR MB_ICONERROR

static st_bool
popup_impl(st_string_t title,
           st_string_t msg,
           st_enum type)
{
    // TODO: Unsafe. Add string length checks.
    return MessageBoxA(
        nullptr,
        ST_CAST(LPCSTR, msg.data),
        ST_CAST(LPCSTR, title.data),
        ST_CAST(UINT, type | MB_OK)
    ) != 0;
}
#endif
#ifdef __linux__
#include <SDL3/SDL.h>

#define TYPE_INFO SDL_MESSAGEBOX_INFORMATION
#define TYPE_WARNING SDL_MESSAGEBOX_WARNING
#define TYPE_ERROR SDL_MESSAGEBOX_ERROR

static st_bool
popup_impl(st_string_t title,
           st_string_t msg,
           st_enum type)
{
    // TODO: Unsafe. Add string length checks.
    return SDL_ShowSimpleMessageBox(
        ST_CAST(SDL_MessageBoxFlags, type),
        ST_CAST(const char *, title.data),
        ST_CAST(const char *, msg.data),
        nullptr
    );
}
#endif

st_bool
st_popup_info(st_string_t title,
              st_string_t msg)
{
    st_log_error("popup_info: (%.*s) %.*s", title.len, title.data, msg.len, msg.data);
    return popup_impl(title, msg, TYPE_INFO);
}

st_bool
st_popup_warn(st_string_t title,
              st_string_t msg)
{
    st_log_error("popup_warn: (%.*s) %.*s", title.len, title.data, msg.len, msg.data);
    return popup_impl(title, msg, TYPE_WARNING);
}

st_bool
st_popup_error(st_string_t title,
               st_string_t msg)
{
    st_log_error("popup_error: (%.*s) %.*s", title.len, title.data, msg.len, msg.data);
    return popup_impl(title, msg, TYPE_ERROR);
}
