/*
 * popup.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/08/2026
 */

#ifndef STICKY_OS_POPUP_H
#define STICKY_OS_POPUP_H

#include "st_primitives.h"
#include "type/string.h"

#ifdef __cplusplus
extern "C" {
#endif

st_bool st_popup_info (st_string_t title, st_string_t msg);
st_bool st_popup_warn (st_string_t title, st_string_t msg);
st_bool st_popup_error(st_string_t title, st_string_t msg);

#ifdef __cplusplus
}
#endif

#endif // STICKY_OS_POPUP_H
