/*
 * gui_backend.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 20/08/2026
 */

#ifndef STICKY_GUI_BACKEND_GUI_IMGUI_HPP
#define STICKY_GUI_BACKEND_GUI_IMGUI_HPP

#include "st_includes.h"
#include "video/window.h"

#ifdef __cplusplus
extern "C" {
#endif

st_bool st_gui_backend_init(void);
st_bool st_gui_backend_free(void);

st_bool st_gui_backend_create(SDL_Window *win, SDL_GLContext gl_context);
st_bool st_gui_backend_destroy(void);

void st_gui_backend_new_frame(void);
void st_gui_backend_render_frame(void);
void st_gui_backend_swap_frame(void);

// TODO: Replace SDL_Event with st_event_t.
void st_gui_backend_process_event(SDL_Event e);

#ifdef __cplusplus
}
#endif

#endif // STICKY_GUI_BACKEND_GUI_IMGUI_HPP
