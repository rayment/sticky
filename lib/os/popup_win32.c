/*
 * popup_win32.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/08/2026
 */

#include <stdlib.h>

#include <windows.h>

#include "os/popup.h"
#include "st_macros.h"

#define TYPE_INFO MB_ICONINFORMATION
#define TYPE_WARNING MB_ICONWARNING
#define TYPE_ERROR MB_ICONERROR

/* Converts UTF-8 to a newly allocated UTF-16 string, or returns null. */
static wchar_t *
_st_os_popup_widen(const st_char *text)
{
    const int length = MultiByteToWideChar(CP_UTF8, 0, text, -1, nullptr, 0);
    if (length <= 0)
        return nullptr;

    wchar_t *wide = (wchar_t *) malloc((size_t) length * sizeof(wchar_t));
    if (wide == nullptr)
        return nullptr;

    if (MultiByteToWideChar(CP_UTF8, 0, text, -1, wide, length) != length)
    {
        free(wide);
        return nullptr;
    }
    return wide;
}

static st_bool
_st_os_popup_impl(const st_char *title,
                  const st_char *message,
                  st_uint32      type)
{
    wchar_t *wide_title = _st_os_popup_widen(title);
    wchar_t *wide_message = _st_os_popup_widen(message);
    st_bool  shown = false;

    if (wide_title != nullptr && wide_message != nullptr)
        shown = MessageBoxW(nullptr, wide_message, wide_title, (UINT) (type | MB_OK)) != 0;

    free(wide_title);
    free(wide_message);
    return shown;
}

ST_EXTERN_PROTOTYPE(st_bool _st_os_popup_impl_info(const st_char *title,
                                                   const st_char *message))
{
    return _st_os_popup_impl(title, message, TYPE_INFO);
}

ST_EXTERN_PROTOTYPE(st_bool _st_os_popup_impl_warn(const st_char *title,
                                                   const st_char *message))
{
    return _st_os_popup_impl(title, message, TYPE_WARNING);
}

ST_EXTERN_PROTOTYPE(st_bool _st_os_popup_impl_error(const st_char *title,
                                                    const st_char *message))
{
    return _st_os_popup_impl(title, message, TYPE_ERROR);
}
