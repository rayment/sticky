/*
 * button.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 20/08/2026
 */

#ifndef STICKY_BUTTON_H
#define STICKY_BUTTON_H

#include "type/string.h"
#include "gui/widget.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct st_gui_button
{
    st_gui_widget_t *parent;

    st_string_t text;

    void (*on_click)(struct st_gui_button *);
} st_gui_button_t;

#ifdef __cplusplus
}
#endif

#endif // STICKY_BUTTON_H
