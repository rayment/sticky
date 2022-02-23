/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mouse.c
 * Mouse input handler header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 05/09/2021
 */

#include "sticky/common/error.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/input/mouse.h"

static Sint32 dx, dy, xpos, ypos, dxwheel, dywheel;
static Sbool ldown, mdown, rdown, ex1down, ex2down,
             lclick, mclick, rclick, ex1click, ex2click;

void
S_input_get_mouse_motion(Sint32 *x,
                         Sint32 *y)
{
	if (x)
		*x = dx;
	if (y)
		*y = dy;
}

void
S_input_get_mouse_wheel_motion(Sint32 *x,
                               Sint32 *y)
{
	if (x)
		*x = dxwheel;
	if (y)
		*y = dywheel;
}

void
S_input_get_mouse_pos(Sint32 *x,
                      Sint32 *y)
{
	if (x)
		*x = xpos;
	if (y)
		*y = ypos;
}

Sbool
S_input_is_button_down(Senum button)
{
	switch (button)
	{
	case S_MOUSE_LEFT:
		return ldown;
	case S_MOUSE_MIDDLE:
		return mdown;
	case S_MOUSE_RIGHT:
		return rdown;
	case S_MOUSE_EXTRA1:
		return ex1down;
	case S_MOUSE_EXTRA2:
		return ex2down;
	case S_MOUSE_ANY:
		return ldown || mdown || rdown || ex1down || ex2down;
	default:
		_S_SET_ERROR(S_INVALID_ENUM, "S_input_is_button_down");
		return S_FALSE;
	}
}

Sbool
S_input_is_button_pressed(Senum button)
{
	switch (button)
	{
	case S_MOUSE_LEFT:
		return lclick;
	case S_MOUSE_MIDDLE:
		return mclick;
	case S_MOUSE_RIGHT:
		return rclick;
	case S_MOUSE_EXTRA1:
		return ex1click;
	case S_MOUSE_EXTRA2:
		return ex2click;
	case S_MOUSE_ANY:
		return lclick || mclick || rclick || ex1click || ex2click;
	default:
		_S_SET_ERROR(S_INVALID_ENUM, "S_input_is_button_pressed");
		return S_FALSE;
	}
}

void
_S_input_mouse_reset(void)
{
	dx = dy = dxwheel = dywheel = 0;
    lclick = mclick = rclick = ex1click = ex2click = S_FALSE;
}

void
_S_input_mouse_init(void)
{
	xpos = ypos = 0;
	ldown = mdown = rdown = ex1down = ex2down = S_FALSE;
	_S_CALL("_S_input_mouse_reset", _S_input_mouse_reset());
}

void
_S_input_mouse_event(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_MOUSEMOTION:
		dx = e.motion.xrel;
		dy = e.motion.yrel;
		xpos = e.motion.x;
		ypos = e.motion.y;
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		xpos = e.button.x;
		ypos = e.button.y;
		switch (e.button.button)
		{
		case SDL_BUTTON_LEFT:
			ldown = lclick = e.button.state == SDL_PRESSED;
			break;
		case SDL_BUTTON_MIDDLE:
			mdown = mclick = e.button.state == SDL_PRESSED;
			break;
		case SDL_BUTTON_RIGHT:
			rdown = rclick = e.button.state == SDL_PRESSED;
			break;
		case SDL_BUTTON_X1:
			ex1down = ex1click = e.button.state == SDL_PRESSED;
			break;
		case SDL_BUTTON_X2:
			ex2down = ex2click = e.button.state == SDL_PRESSED;
			break;
		}
		break;
	case SDL_MOUSEWHEEL:
		dxwheel = e.wheel.x;
		dywheel = e.wheel.y;
		break;
	}
}

