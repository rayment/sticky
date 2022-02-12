/*
 * sticky.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 12/02/2022
 */

#include "sticky.h"

void
S_sticky_init()
{
	SERRNO = S_NO_ERROR;
	SERRLOC = "null";
#ifdef DEBUG
	_S_memtrace_init();
#endif
}

void
S_sticky_free()
{
#ifdef DEBUG
	_S_memtrace_free();
#endif
}

