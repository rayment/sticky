/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * gamepad.c
 * Gamepad input handler header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 05/09/2021
 */

#include "sticky/common/error.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/input/gamepad.h"

#define NOT_GAMEPAD  0
#define DEFAULT      1

static
struct
gamepad_s
{
	Sint16 axis_left_horiz, axis_left_vert, axis_left_trig,
	       axis_right_horiz, axis_right_vert, axis_right_trig;
	Sbool a_press, a_down,
	      b_press, b_down,
	      x_press, x_down,
	      y_press, y_down,
	      back_press, back_down,
	      guide_press, guide_down,
	      start_press, start_down,
	      left_stick_press, left_stick_down,
	      right_stick_press, right_stick_down,
	      left_trigger_press, left_trigger_down,
	      right_trigger_press, right_trigger_down,
	      dpad_up_press, dpad_up_down,
	      dpad_down_press, dpad_down_down,
	      dpad_left_press, dpad_left_down,
	      dpad_right_press, dpad_right_down;
} g[S_GAMEPAD_MAX_PADS];

static Suint8 status[S_GAMEPAD_MAX_PADS];
static SDL_GameController *pads[S_GAMEPAD_MAX_PADS];
static Sint8 numpads, padidx[S_GAMEPAD_MAX_PADS];
static Sbool init = S_FALSE;

Sbool
S_input_is_gamepad_button_down(Suint8 gamepad,
                               Senum button)
{
	Sint8 idx;

	if (gamepad >= S_GAMEPAD_MAX_PADS ||
	    (idx = padidx[gamepad]) < 0 || idx >= S_GAMEPAD_MAX_PADS)
		_S_SET_ERROR(S_INVALID_INDEX, "S_input_is_gamepad_button_down");

	switch (button)
	{
	case S_GAMEPAD_BUTTON_A:
		return g[idx].a_down;
	case S_GAMEPAD_BUTTON_B:
		return g[idx].b_down;
	case S_GAMEPAD_BUTTON_X:
		return g[idx].x_down;
	case S_GAMEPAD_BUTTON_Y:
		return g[idx].y_down;
	case S_GAMEPAD_BUTTON_BACK:
		return g[idx].back_down;
	case S_GAMEPAD_BUTTON_GUIDE:
		return g[idx].guide_down;
	case S_GAMEPAD_BUTTON_START:
		return g[idx].start_down;
	case S_GAMEPAD_BUTTON_LEFT_STICK:
		return g[idx].left_stick_down;
	case S_GAMEPAD_BUTTON_RIGHT_STICK:
		return g[idx].right_stick_down;
	case S_GAMEPAD_BUTTON_LEFT_TRIGGER:
		return g[idx].left_trigger_down;
	case S_GAMEPAD_BUTTON_RIGHT_TRIGGER:
		return g[idx].right_trigger_down;
	case S_GAMEPAD_BUTTON_DPAD_UP:
		return g[idx].dpad_up_down;
	case S_GAMEPAD_BUTTON_DPAD_DOWN:
		return g[idx].dpad_down_down;
	case S_GAMEPAD_BUTTON_DPAD_LEFT:
		return g[idx].dpad_left_down;
	case S_GAMEPAD_BUTTON_DPAD_RIGHT:
		return g[idx].dpad_right_down;
	default:
		_S_SET_ERROR(S_INVALID_ENUM, "S_input_is_gamepad_button_down");
		return 0; /* unreachable */
	}
}

Sbool
S_input_is_gamepad_button_pressed(Suint8 gamepad,
                                  Senum button)
{
	Sint8 idx;

	if (gamepad >= S_GAMEPAD_MAX_PADS ||
	    (idx = padidx[gamepad]) < 0 || idx >= S_GAMEPAD_MAX_PADS)
		_S_SET_ERROR(S_INVALID_INDEX, "S_input_is_gamepad_button_pressed");

	switch (button)
	{
	case S_GAMEPAD_BUTTON_A:
		return g[idx].a_press;
	case S_GAMEPAD_BUTTON_B:
		return g[idx].b_press;
	case S_GAMEPAD_BUTTON_X:
		return g[idx].x_press;
	case S_GAMEPAD_BUTTON_Y:
		return g[idx].y_press;
	case S_GAMEPAD_BUTTON_BACK:
		return g[idx].back_press;
	case S_GAMEPAD_BUTTON_GUIDE:
		return g[idx].guide_press;
	case S_GAMEPAD_BUTTON_START:
		return g[idx].start_press;
	case S_GAMEPAD_BUTTON_LEFT_STICK:
		return g[idx].left_stick_press;
	case S_GAMEPAD_BUTTON_RIGHT_STICK:
		return g[idx].right_stick_press;
	case S_GAMEPAD_BUTTON_LEFT_TRIGGER:
		return g[idx].left_trigger_press;
	case S_GAMEPAD_BUTTON_RIGHT_TRIGGER:
		return g[idx].right_trigger_press;
	case S_GAMEPAD_BUTTON_DPAD_UP:
		return g[idx].dpad_up_press;
	case S_GAMEPAD_BUTTON_DPAD_DOWN:
		return g[idx].dpad_down_press;
	case S_GAMEPAD_BUTTON_DPAD_LEFT:
		return g[idx].dpad_left_press;
	case S_GAMEPAD_BUTTON_DPAD_RIGHT:
		return g[idx].dpad_right_press;
	default:
		_S_SET_ERROR(S_INVALID_ENUM, "S_input_is_gamepad_button_pressed");
		return 0; /* unreachable */
	}
}

Suint8
S_input_get_num_gamepads(void)
{
	return numpads;
}

Sint16
S_input_get_gamepad_axis(Suint8 gamepad,
                         Senum axis)
{
	Sint8 idx;

	if (gamepad >= S_GAMEPAD_MAX_PADS ||
	    (idx = padidx[gamepad]) < 0 || idx >= S_GAMEPAD_MAX_PADS)
		_S_SET_ERROR(S_INVALID_INDEX, "S_input_get_gamepad_axis");

	switch (axis)
	{
	case S_GAMEPAD_AXIS_LEFT_HORIZONTAL:
		return g[idx].axis_left_horiz;
	case S_GAMEPAD_AXIS_LEFT_VERTICAL:
		return g[idx].axis_left_vert;
	case S_GAMEPAD_AXIS_LEFT_TRIGGER:
		return g[idx].axis_left_trig;
	case S_GAMEPAD_AXIS_RIGHT_HORIZONTAL:
		return g[idx].axis_right_horiz;
	case S_GAMEPAD_AXIS_RIGHT_VERTICAL:
		return g[idx].axis_right_vert;
	case S_GAMEPAD_AXIS_RIGHT_TRIGGER:
		return g[idx].axis_right_trig;
	default:
		_S_SET_ERROR(S_INVALID_ENUM, "S_input_get_gamepad_axis");
		return 0; /* unreachable */
	}
}

void
_S_input_gamepad_reset(void)
{
	Suint8 i;
	/*memset(g, 0,
	       sizeof(struct gamepad_s) * S_GAMEPAD_MAX_PADS);*/
	for (i = 0; i < S_GAMEPAD_MAX_PADS; ++i)
	{
		g[i].a_press = S_FALSE;
		g[i].b_press = S_FALSE;
		g[i].x_press = S_FALSE;
		g[i].y_press = S_FALSE;
		g[i].back_press = S_FALSE;
		g[i].guide_press = S_FALSE;
		g[i].start_press = S_FALSE;
		g[i].left_stick_press = S_FALSE;
		g[i].right_stick_press = S_FALSE;
		g[i].left_trigger_press = S_FALSE;
		g[i].right_trigger_press = S_FALSE;
		g[i].dpad_up_press = S_FALSE;
		g[i].dpad_down_press = S_FALSE;
		g[i].dpad_left_press = S_FALSE;
		g[i].dpad_right_press = S_FALSE;
	}
}

static
Sbool
_S_input_gamepad_scan(Suint8 raw_id)
{
	if (numpads >= S_GAMEPAD_MAX_PADS)
		return S_FALSE; /* can't add more gamepads */
	if (SDL_IsGameController(raw_id))
	{
		S_debug("Gamepad at index #%d connected.\n", i);
		status[numpads] = DEFAULT;
		pads[numpads] = SDL_GameControllerOpen(raw_id);
		if (!pads[numpads])
			_S_error_sdl(_S_ERR_LOC);
		padidx[numpads] = raw_id;
		++numpads;
		return S_TRUE;
	}
	return S_FALSE;
}

static
Sbool
_S_input_gamepad_remove(Suint8 raw_id)
{
	Suint8 i;
	for (i = 0; i < S_GAMEPAD_MAX_PADS; ++i)
	{
		if (padidx[i] != raw_id)
			continue;
		S_debug("Gamepad at index #%d disconnected.\n", i);
		status[i] = NOT_GAMEPAD;
		SDL_GameControllerClose(pads[i]);
		pads[i] = NULL;
		--numpads;
		return S_TRUE;
	}
	return S_FALSE;
}

void
_S_input_gamepad_init(void)
{
	Sint32 i;

	numpads = 0;
	memset(padidx, -1, sizeof(Sint8) * S_GAMEPAD_MAX_PADS);
	memset(g, 0, sizeof(struct gamepad_s) * S_GAMEPAD_MAX_PADS);
	for (i = 0; i < S_GAMEPAD_MAX_PADS; ++i)
	{
		status[i] = NOT_GAMEPAD;
		pads[i] = NULL;
	}
	/*for (i = 0; i < S_GAMEPAD_MAX_PADS; ++i)
	{
		if (SDL_IsGameController(i))
		{
#ifdef DEBUG
			printf("joystick #%d detected as gamepad.\n", i);
#endif // DEBUG
			status[i] = DEFAULT;
			pads[i] = SDL_GameControllerOpen(i);
			if (!pads[i])
				_S_error_sdl(_S_ERR_LOC);
			padidx[numpads] = i;
			++numpads;
		}
		else
		{
			status[i] = NOT_GAMEPAD;
			pads[i] = NULL;
		}
	}*/
	init = S_TRUE;
	_S_CALL("_S_input_gamepad_reset", _S_input_gamepad_reset());
}

void
_S_input_gamepad_event(SDL_Event event)
{
	Suint8 i;
	SDL_JoystickID id, thisid;

	for (i = 0; i < S_GAMEPAD_MAX_PADS; ++i)
	{
		if (event.type == SDL_CONTROLLERAXISMOTION)
			thisid = event.caxis.which;
		else if (event.type == SDL_CONTROLLERBUTTONDOWN ||
		         event.type == SDL_CONTROLLERBUTTONUP)
			thisid = event.cbutton.which;
		id = SDL_JoystickGetDeviceInstanceID(i);
		if (id != thisid)
			continue;
		switch (event.type)
		{
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			switch (event.cbutton.button)
			{
			case SDL_CONTROLLER_BUTTON_A:
				g[i].a_press = g[i].a_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_B:
				g[i].b_press = g[i].b_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				g[i].x_press = g[i].x_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				g[i].y_press = g[i].y_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_BACK:
				g[i].back_press = g[i].back_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_GUIDE:
				g[i].guide_press = g[i].guide_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_START:
				g[i].start_press = g[i].start_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSTICK:
				g[i].left_stick_press = g[i].left_stick_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
				g[i].right_stick_press = g[i].right_stick_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
				g[i].left_trigger_press = g[i].left_trigger_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
				g[i].right_trigger_press = g[i].right_trigger_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				g[i].dpad_up_press = g[i].dpad_up_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				g[i].dpad_down_press = g[i].dpad_down_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				g[i].dpad_left_press = g[i].dpad_left_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				g[i].dpad_right_press = g[i].dpad_right_down =
					event.cbutton.state == SDL_PRESSED;
				break;
			default:
				return;
			}
			break;
		case SDL_CONTROLLERAXISMOTION:
			switch (event.caxis.axis)
			{
			case SDL_CONTROLLER_AXIS_LEFTX:
				g[i].axis_left_horiz = event.caxis.value;
				break;
			case SDL_CONTROLLER_AXIS_LEFTY:
				g[i].axis_left_vert = event.caxis.value;
				break;
			case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
				g[i].axis_left_trig = event.caxis.value;
				break;
			case SDL_CONTROLLER_AXIS_RIGHTX:
				g[i].axis_right_horiz = event.caxis.value;
				break;
			case SDL_CONTROLLER_AXIS_RIGHTY:
				g[i].axis_right_vert = event.caxis.value;
				break;
			case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
				g[i].axis_right_trig = event.caxis.value;
				break;
			}
			break;
		case SDL_CONTROLLERDEVICEADDED:
			_S_input_gamepad_scan(event.cdevice.which);
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			_S_input_gamepad_remove(event.cdevice.which);
			break;
		case SDL_CONTROLLERDEVICEREMAPPED:
			/* TODO */
			S_debug("joystick remap event found\n");
			break;
		}
	}
}

