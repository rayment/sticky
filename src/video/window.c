/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * window.c
 * Video window source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#include <string.h>

#include "sticky/common/error.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/concurrency/mutex.h"
#include "sticky/concurrency/thread.h"
#include "sticky/math/math.h"
#include "sticky/math/vec4.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/window.h"

static Sbool init = S_FALSE;
static Sbool exists = S_FALSE;

Swindow *
S_window_new(void)
{
	Swindow *window;
	if (!init)
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
			_S_error_sdl("S_window_new");
		init = S_TRUE;
	}
	if (exists)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_window_new");
		return NULL;
	}
	window = (Swindow *) S_memory_new(sizeof(Swindow));
	window->window = NULL;
	window->context = NULL;
	window->icon = NULL;
	window->display_mode = S_WINDOWED;
	window->vsync = S_FALSE;
	window->gl_profile = S_GL_CORE;
	window->gl_major = 3;
	window->gl_minor = 2;
	window->width = 800;
	window->height = 600;
	_S_CALL("S_vec4_set",
	        S_vec4_set(&(window->clear_color), 0.0f, 0.0f, 0.0f, 1.0f));
	window->hwaccel = S_TRUE;
	window->doublebuf = S_FALSE;
	window->capture_mouse = S_FALSE;
	window->centered = S_FALSE;
	memset(window->title, 0, 64 * sizeof(Schar));
	window->input_mode = S_KEYBOARD | S_MOUSE;
	/* tick init */
	window->delta_time = 0;
	window->current_frame = 0;
	window->last_frame = 0;
	window->tick_limit = 30;
	window->skip_ticks = 1000;
	window->next_tick = 0;
	window->ticks = 0;

	window->running = S_FALSE;
	exists = S_TRUE;
	return window;
}

void
S_window_delete(Swindow *window)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_delete");
		return;
	}
	SDL_GL_DeleteContext(window->context);
	SDL_DestroyWindow(window->window);
	S_memory_delete(window);
	exists = S_FALSE;
}

void
S_window_close(Swindow *window)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_close");
		return;
	}
	window->running = S_FALSE;
}

static inline
void
_S_window_set_attrib(SDL_GLattr attrib,
                     Sint32 val)
{
	if (SDL_GL_SetAttribute(attrib, val) != 0)
		_S_error_sdl("_S_window_set_attrib");
}

void
S_window_apply(Swindow *window)
{
	Suint32 glew;
	Senum windowpos;
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_apply");
		return;
	}
	/* settings and contexts that need be called only once */
	if (!window->running)
	{
		/* GL context init */
		_S_CALL("_S_window_set_attrib",
		        _S_window_set_attrib(SDL_GL_CONTEXT_MAJOR_VERSION,
		                             window->gl_major));
		_S_CALL("_S_window_set_attrib",
		        _S_window_set_attrib(SDL_GL_CONTEXT_MINOR_VERSION,
		                             window->gl_minor));
		_S_CALL("_S_window_set_attrib",
		        _S_window_set_attrib(SDL_GL_CONTEXT_PROFILE_MASK,
		                             window->gl_profile));
		_S_CALL("_S_window_set_attrib",
		        _S_window_set_attrib(SDL_GL_DOUBLEBUFFER,
		                             window->doublebuf));
		_S_CALL("_S_window_set_attrib",
		        _S_window_set_attrib(SDL_GL_ACCELERATED_VISUAL,
		                             window->hwaccel));
		/* create window */
		if (window->centered)
			windowpos = SDL_WINDOWPOS_CENTERED;
		else
			windowpos = SDL_WINDOWPOS_UNDEFINED;
		window->window =
			SDL_CreateWindow(window->title, windowpos, windowpos,
			                 window->width, window->height, SDL_WINDOW_OPENGL);
		if (!window->window)
			_S_error_sdl("S_window_apply");
		/* create GL render context */
		window->context = SDL_GL_CreateContext(window->window);
		if (!window->context)
			_S_error_sdl("S_window_apply");
		/* set window icon */
		if (window->icon)
		{
			/* TODO: Implement S_window_set_icon. */
			SDL_SetWindowIcon(window->window, window->icon);
			SDL_FreeSurface(window->icon);
		}
		/* init GLEW */
		if ((glew = glewInit()) != GLEW_OK)
			_S_error_glew("S_sticky_init", glew);
		/* disable resizing */
		SDL_SetWindowResizable(window->window, SDL_FALSE);
		window->running = S_TRUE;
	}
	/* general settings that can be applied at any time */
	/* window size */
	SDL_SetWindowSize(window->window, window->width, window->height);
	/* display mode */
	SDL_SetWindowFullscreen(window->window, window->display_mode);
	/* mouse capture */
	SDL_SetRelativeMouseMode(window->capture_mouse);
	/* vsync */
	if (window->vsync)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);
	/* clear color */
	_S_GL(glClearColor(window->clear_color.x,
	                   window->clear_color.y,
	                   window->clear_color.z,
	                   window->clear_color.w));
	/* viewport */
	_S_GL(glViewport(0, 0, window->width, window->height));
	_S_GL(glEnable(GL_DEPTH_TEST));
	/* ticks */
	window->skip_ticks = 1000 / window->tick_limit;
	window->next_tick = SDL_GetTicks();
	window->ticks = 0;
}

void
S_window_clear(Swindow *window)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_clear");
		return;
	}
	window->last_frame = window->current_frame;
	window->current_frame = SDL_GetTicks();
	window->delta_time = window->current_frame - window->last_frame;
	window->ticks = 0;
	/* TODO: Allow clearing manually? */
	_S_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void
S_window_poll(Swindow *window)
{
	SDL_Event e;
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_poll");
		return;
	}
	while (SDL_PollEvent(&e) != 0)
	{
		/* TODO: Implement input and event handler. */
	}
}

void
S_window_swap(Swindow *window)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_swap");
		return;
	}
	SDL_GL_SwapWindow(window->window);
}

void
S_window_tick(Swindow *window)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_tick");
		return;
	}
	window->next_tick += window->skip_ticks;
	++window->ticks;
}

void
S_window_set_title(Swindow *window,
                  const Schar *title)
{
	Ssize_t len;
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_title");
		return;
	}
	if (title)
	{
		len = S_min(strlen(title), 63);
		if (len > 0)
		{
			memcpy(window->title, title, len * sizeof(Schar));
			*(window->title+len) = '\0'; /* null terminate */
			return;
		}
	}
	*(window->title) = '\0';
}

void
S_window_set_size(Swindow *window,
                 Suint16 width,
                 Suint16 height)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_size");
		return;
	}
	window->width = width;
	window->height = height;
}

void
S_window_set_capture_mouse(Swindow *window,
                           Sbool capture)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_capture_mouse");
		return;
	}
	window->capture_mouse = capture;
}

void
S_window_set_centered(Swindow *window,
                     Sbool centered)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_centered");
		return;
	}
	window->centered = centered;
}

void
S_window_set_display_mode(Swindow *window,
                          Senum mode)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_display_mode");
		return;
	}
	else if (mode != S_WINDOWED || mode != S_BORDERLESS || mode != S_FULLSCREEN)
	{
		_S_SET_ERROR(S_INVALID_ENUM, "S_window_set_display_mode");
		return;
	}
	window->display_mode = mode;
}

void
S_window_set_vsync(Swindow *window,
                   Sbool vsync)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_vsync");
		return;
	}
	window->vsync = vsync;
}

void
S_window_set_gl_profile(Swindow *window,
                        Senum profile)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_display_mode");
		return;
	}
	else if (profile != S_GL_COMPATIBILITY || profile != S_GL_CORE)
	{
		_S_SET_ERROR(S_INVALID_ENUM, "S_window_set_gl_profile");
		return;
	}
	window->gl_profile = profile;
}

void
S_window_set_gl_version(Swindow *window,
                        Suint8 major,
                        Suint8 minor)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_gl_version");
		return;
	}
	window->gl_major = major;
	window->gl_minor = minor;
}

void
S_window_set_clear_color(Swindow *window,
                         Sfloat r,
                         Sfloat g,
                         Sfloat b,
                         Sfloat a)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_clear_color");
		return;
	}
	_S_CALL("S_vec4_set", S_vec4_set(&(window->clear_color), r, g, b, a));
}

void
S_window_set_hardware_acceleration(Swindow *window,
                                   Sbool hwaccel)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_hardware_acceleration");
		return;
	}
	window->hwaccel = hwaccel;
}

void
S_window_set_double_buffer(Swindow *window,
                           Sbool doublebuf)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_double_buffer");
		return;
	}
	window->doublebuf = doublebuf;
}

void
S_window_set_ticks_per_second(Swindow *window,
                              Suint16 ticks)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_ticks_per_second");
		return;
	}
	if (ticks == 0 || ticks > 500)
		ticks = 30;
	window->tick_limit = ticks;
}

void
S_window_set_input_mode(Swindow *window,
                        Senum mode)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_input_mode");
		return;
	}
	window->input_mode = mode;
}

void
S_window_get_size(const Swindow *window,
                  Suint16 *width,
                  Suint16 *height)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_get_size");
		return;
	}
	*width = window->width;
	*height = window->height;
}

Sfloat
S_window_get_delta_time(const Swindow *window)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_get_delta_time");
		return 0.0f;
	}
	return (Sfloat) window->delta_time / 1000.0f;
}

Sbool
S_window_is_running(const Swindow *window)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_get_running");
		return S_FALSE;
	}
	return window->running;
}

Sbool
S_window_is_tick(const Swindow *window)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_get_ticking");
		return S_FALSE;
	}
	return SDL_GetTicks() > window->next_tick &&
	       window->ticks < window->tick_limit;
}

