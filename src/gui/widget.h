/*
 * widget.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 20/08/2026
 */

#ifndef STICKY_GUI_WIDGET_H
#define STICKY_GUI_WIDGET_H

#include "gui/attributes.h"
#include "math/vec2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct st_gui_widget
{
    struct st_gui_widget *parent;
    //s_linkedlist children;

    st_gui_size_t size;
    st_gui_rect_t geometry;

    st_uint32 flags;

    void (*layout)(struct st_gui_widget *);
    void (*render)(struct st_gui_widget *);
    void (*event) (struct st_gui_widget *);
} st_gui_widget_t;

#ifdef __cplusplus
}
#endif

#endif // STICKY_GUI_WIDGET_H
