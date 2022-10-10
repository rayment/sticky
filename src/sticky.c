/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * sticky.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/02/2022
 */

#include <stdlib.h>
#include <time.h>

#include "sticky.h"

/* single definitions */
Senum  _S_error;
Schar *_S_error_loc;

void
S_sticky_init(void)
{
	/* error handler init */
	SERRNO = S_NO_ERROR;
	SERRLOC = "null";
#ifdef DEBUG
	/* this must come before other _S_CALL calls! */
	_S_memtrace_init();
#endif /* DEBUG */
	/* random number generator init */
	_S_CALL("S_random_set_seed", S_random_set_seed(time(NULL)));
	/* network stack */
	_S_CALL("_S_socket_init", _S_socket_init());
}

void
S_sticky_free(void)
{
	Sbool init;
	_S_CALL("_S_window_is_init", init = _S_window_is_init());
	if (init)
	{
		/* free audio-visual stuff */
		SDL_Quit();
		_S_CALL("_S_sound_free", _S_sound_free());
	}
	_S_CALL("_S_socket_free", _S_socket_free());
#ifdef DEBUG
	_S_memtrace_free();
	if (!_S_memtrace_all_free())
		exit(EXIT_FAILURE);
#endif /* DEBUG */
}

const Schar *
S_sticky_get_version(void)
{
	return VERSION;
}

