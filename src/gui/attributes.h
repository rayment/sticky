/*
 * attributes.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 20/08/2026
 */

#ifndef STICKY_GUI_ATTRIBUTES_H
#define STICKY_GUI_ATTRIBUTES_H
#include "math/vec2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct st_gui_rect
{
    st_vec2_int32_t pos;
    st_vec2_int32_t size;
} st_gui_rect_t;

typedef struct st_gui_size
{
    st_vec2_int32_t minimum;
    st_vec2_int32_t preferred;
    st_vec2_int32_t maximum;
} st_gui_size_t;

#ifdef __cplusplus
}
#endif

#endif // STICKY_GUI_ATTRIBUTES_H
