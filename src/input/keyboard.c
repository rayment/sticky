/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * keyboard.c
 * Keyboard input handler source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 05/09/2021
 */

#include "sticky/common/error.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/input/keyboard.h"

/* max number of keys that can be held down at the same time and registered */
#define MAX_CONCURRENT_KEYS 8

static SDL_Keycode keys[MAX_CONCURRENT_KEYS];
static SDL_Keycode keypresses[MAX_CONCURRENT_KEYS];

void
_S_input_keyboard_reset(void)
{
	memset(keypresses, -1, sizeof(SDL_Keycode) * MAX_CONCURRENT_KEYS);
}

void
_S_input_keyboard_init(void)
{
	memset(keys, -1, sizeof(SDL_Keycode) * MAX_CONCURRENT_KEYS);
	_S_CALL("_S_input_keyboard_reset", _S_input_keyboard_reset());
}

void
_S_input_keyboard_event(SDL_Event e)
{
	SDL_Keycode key, *tmp;
	Ssize_t i;

	switch (e.type)
	{
	case SDL_KEYDOWN:
		key = e.key.keysym.sym;
		tmp = NULL;
		for (i = 0; i < MAX_CONCURRENT_KEYS; ++i)
		{
			if (*(keys+i) == -1)
				tmp = keys+i;
			if (*(keys+i) == key)
			{
				tmp = NULL;
				break;
			}
		}
		if (tmp)
			*tmp = key;
		for (i = 0; i < MAX_CONCURRENT_KEYS; ++i)
		{
			if (*(keypresses+i) == -1)
			{
				*(keypresses+i) = key;
				break;
			}
		}
		break;
	case SDL_KEYUP:
		key = e.key.keysym.sym;
		for (i = 0; i < MAX_CONCURRENT_KEYS; ++i)
		{
			if (*(keys+i) == key)
			{
				*(keys+i) = -1;
				break;
			}
		}
		break;
	}
}

Sbool
S_input_is_key_down(Skey key)
{
	Ssize_t i;
	for (i = 0; i < MAX_CONCURRENT_KEYS; ++i)
	{
		if (*(keys+i) == key ||
		    (*(keys+i) != -1 && key == S_KEY_ANY))
			return S_TRUE;
	}
	return S_FALSE;
}

Sbool
S_input_is_key_pressed(Skey key)
{
	Ssize_t i;
	for (i = 0; i < MAX_CONCURRENT_KEYS; ++i)
	{
		if (*(keypresses+i) == key ||
		    (*(keypresses+i) != -1 && key == S_KEY_ANY))
			return S_TRUE;
	}
	return S_FALSE;
}

