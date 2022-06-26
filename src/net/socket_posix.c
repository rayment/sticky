/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * socket_posix.c
 * POSIX socket function implementation.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 26/06/2022
 */

#include "sticky/net/socket.h"

#ifndef STICKY_POSIX
#error This source file cannot be compiled on non-POSIX systems.
#endif /* STICKY_POSIX */

void
_S_socket_init(void)
{}

void
_S_socket_free(void)
{}

