/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * window.h
 * Video window header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_WINDOW_WINDOW_H
#define FR_RAYMENT_WINDOW_WINDOW_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/concurrency/mutex.h"
#include "sticky/concurrency/thread.h"

#define S_WINDOWED         (SDL_WINDOW_OPENGL)
#define S_BORDERLESS       (SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP)
#define S_FULLSCREEN       (SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN)

#define S_GL_COMPATABILITY (SDL_GL_CONTEXT_PROFILE_COMPATIBILITY)
#define S_GL_CORE          (SDL_GL_CONTEXT_PROFILE_CORE)

typedef struct
Swindow_s
{
	SDL_Window *window;
	SDL_GLContext *context;
	SDL_Surface *icon;
	Senum display_mode, gl_profile;
	Suint8 gl_major, gl_minor;
	Suint16 width, height;
	Svec4 clear_color;
	Sbool centered, capture_mouse, vsync, hwaccel, doublebuf;
	Schar title[64];
	Sbool running;
	Suint16 ticks, tick_limit, skip_ticks, next_tick;
	Suint32 delta_time, current_frame, last_frame;
} Swindow;

Swindow *S_window_new(void);
void     S_window_delete(Swindow *);
void     S_window_apply(Swindow *);
void     S_window_close(Swindow *);
void     S_window_clear(Swindow *);
void     S_window_swap(Swindow *);
void     S_window_tick(Swindow *);

void     S_window_set_title(Swindow *, const Schar *);
void     S_window_set_size(Swindow *, Suint16, Suint16);
void     S_window_set_centered(Swindow *, Sbool);
void     S_window_set_display_mode(Swindow *, Senum);
void     S_window_set_gl_profile(Swindow *, Senum);
void     S_window_set_gl_version(Swindow *, Suint8, Suint8);
void     S_window_set_clear_color(Swindow *, Sfloat, Sfloat, Sfloat, Sfloat);
void     S_window_set_hardware_acceleration(Swindow *, Sbool);
void     S_window_set_double_buffer(Swindow *, Sbool);
void     S_window_set_ticks_per_second(Swindow *, Suint16);

void     S_window_get_size(const Swindow *, Suint16 *, Suint16 *);
Sfloat   S_window_get_delta_time(Swindow *);
Sbool    S_window_is_running(Swindow *);
Sbool    S_window_is_tick(Swindow *);

void *  _S_window_thread(void *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_WINDOW_WINDOW_H */

