/*
 * popup_win32.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <windows.h>

#include "st_macros.h"
#include "os/popup.h"

#define TYPE_INFO    MB_ICONINFORMATION
#define TYPE_WARNING MB_ICONWARNING
#define TYPE_ERROR   MB_ICONERROR

static st_bool
_st_os_popup_impl(const st_char *title,
                  const st_char *message,
                  st_enum type)
{
    return ST_LIKELY(MessageBoxA(
        nullptr,
        (LPCSTR) message,
        (LPCSTR) title,
        (UINT) (type | MB_OK)
    ) != 0);
}

ST_EXTERN_PROTOTYPE(
st_bool
_st_os_popup_impl_info(const st_char *title,
                       const st_char *message)
)
{
    return _st_os_popup_impl(title, message, TYPE_INFO);
}

ST_EXTERN_PROTOTYPE(
st_bool
_st_os_popup_impl_warn(const st_char *title,
                       const st_char *message)
)
{
    return _st_os_popup_impl(title, message, TYPE_WARNING);
}

ST_EXTERN_PROTOTYPE(
st_bool
_st_os_popup_impl_error(const st_char *title,
                        const st_char *message)
)
{
    return _st_os_popup_impl(title, message, TYPE_ERROR);
}
