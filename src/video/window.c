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

#include "sticky/audio/sound.h"
#include "sticky/common/error.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/concurrency/mutex.h"
#include "sticky/concurrency/thread.h"
#include "sticky/input/gamepad.h"
#include "sticky/input/keyboard.h"
#include "sticky/input/mouse.h"
#include "sticky/math/math.h"
#include "sticky/math/vec4.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/draw.h"
#include "sticky/video/font.h"
#include "sticky/video/texture.h"
#include "sticky/video/window.h"

static Sbool init = S_FALSE;
static Sbool exists = S_FALSE;

Swindow *
S_window_new(void)
{
	Swindow *window;
	if (!init)
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
			_S_error_sdl("S_window_new");
		_S_CALL("_S_input_gamepad_init", _S_input_gamepad_init());
		_S_CALL("_S_input_keyboard_init", _S_input_keyboard_init());
		_S_CALL("_S_input_mouse_init", _S_input_mouse_init());
		_S_CALL("_S_sound_init", _S_sound_init());
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
	window->cam = NULL;
	window->camresize = S_TRUE;
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
	window->resizable = S_FALSE;
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

	window->dshader = NULL;
	window->dtex = NULL;
	_S_CALL("S_vec4_fill", S_vec4_fill(&window->dcolor, 1.0f));
	_S_CALL("S_transform_new", window->dtrans = S_transform_new());

	window->on_exit = NULL;
	window->on_resize = NULL;

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
	if (window->cam)
	{
		_S_CALL("S_camera_attach", S_camera_attach(window->cam, NULL, S_FALSE));
	}
	_S_CALL("S_transform_delete", S_transform_delete(window->dtrans));
	_S_CALL("_S_font_free", _S_font_free());
	_S_CALL("_S_draw_free", _S_draw_free());
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

static inline
void
_S_window_recalculate_viewport(Swindow *window)
{
	_S_GL(glViewport(0, 0, window->width, window->height));
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
		/* other init that requires GL */
		_S_CALL("_S_texture_init", _S_texture_init());
		_S_CALL("_S_draw_init",
		        _S_draw_init(window->gl_major, window->gl_minor));
		_S_CALL("_S_font_init",
		        _S_font_init(window->gl_major, window->gl_minor));
		window->running = S_TRUE;
	}
	/* general settings that can be applied at any time */
	/* window size */
	SDL_SetWindowSize(window->window, window->width, window->height);
	/* resizing */
	SDL_SetWindowResizable(window->window, window->resizable);
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
	_S_CALL("_S_window_recalculate_viewport",
	        _S_window_recalculate_viewport(window));
	_S_GL(glEnable(GL_DEPTH_TEST));
	_S_GL(glEnable(GL_BLEND));
	_S_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
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
	if ((window->input_mode & S_GAMEPAD) == S_GAMEPAD)
		_S_input_gamepad_reset();
	if ((window->input_mode & S_KEYBOARD) == S_KEYBOARD)
		_S_input_keyboard_reset();
	if ((window->input_mode & S_MOUSE) == S_MOUSE)
		_S_input_mouse_reset();
	while (SDL_PollEvent(&e) != 0)
	{
		/* TODO: Implement input and event handler. */
		switch (e.type)
		{
		case SDL_QUIT:
			/* window or program exit */
			if (window->on_exit)
			{
				_S_CALL("HOOK (Swindow::on_exit)", window->on_exit(window));
			}
			else
			{
				/* if no callback method was registered, then just close */
				_S_CALL("S_window_close", S_window_close(window));
			}
			break;
		case SDL_WINDOWEVENT:
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				/* window resize */
				window->width = (Suint16) e.window.data1;
				window->height = (Suint16) e.window.data2;
				_S_CALL("_S_window_recalculate_viewport",
				        _S_window_recalculate_viewport(window));
				if (window->on_resize)
				{
					_S_CALL("HOOK (Swindow::on_resize)",
					        window->on_resize(window));
				}
				if (window->cam && window->camresize)
				{
					_S_CALL("_S_camera_resize_hook",
					        _S_camera_resize_hook(window->cam));
				}
				break;
			}
			break;
		case SDL_CONTROLLERAXISMOTION:
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMOVED:
		case SDL_CONTROLLERDEVICEREMAPPED:
			if ((window->input_mode & S_GAMEPAD) == S_GAMEPAD)
			{
				_S_CALL("_S_input_gamepad_event", _S_input_gamepad_event(e));
			}
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if ((window->input_mode & S_KEYBOARD) == S_KEYBOARD)
			{
				_S_CALL("_S_input_keyboard_event", _S_input_keyboard_event(e));
			}
			break;
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEWHEEL:
			if ((window->input_mode & S_MOUSE) == S_MOUSE)
			{
				_S_CALL("_S_input_mouse_event", _S_input_mouse_event(e));
			}
			break;
		}
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
		len = strlen(title);
		if (len > 63)
			len = 63;
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
S_window_set_resizable(Swindow *window,
                       Sbool resizable)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_resizable");
		return;
	}
	window->resizable = resizable;
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
	else if (mode != S_WINDOWED && mode != S_BORDERLESS && mode != S_FULLSCREEN)
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
	else if (profile != S_GL_COMPATIBILITY && profile != S_GL_CORE)
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
S_window_set_callback_on_exit(Swindow *window,
                              Swindow_callback callback)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_callback_on_exit");
		return;
	}
	window->on_exit = callback;
}

void
S_window_set_callback_on_resize(Swindow *window,
                                Swindow_callback callback)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_set_callback_on_resize");
		return;
	}
	window->on_resize = callback;
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
	if (width)
		*width = window->width;
	if (height)
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

Suint8
S_window_get_num_displays(void)
{
	Sint32 num;
	if ((num = SDL_GetNumVideoDisplays() < 1))
		_S_error_sdl("S_window_get_num_displays");
	return (Suint8) num;
}

void
S_window_get_display_info(Suint8 id,
                          Sint32 *w,
						  Sint32 *h,
						  Sint32 *refresh)
{
	SDL_DisplayMode dm;
	if ((SDL_GetDesktopDisplayMode(id, &dm)) != 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_window_get_display_info");
		return;
	}
	if (w)
		*w = dm.w;
	if (h)
		*h = dm.h;
	if (refresh)
		*refresh = dm.refresh_rate;
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

Sbool
_S_window_is_init(void)
{
	return init;
}

