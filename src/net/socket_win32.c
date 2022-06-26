/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * socket.c
 * Win32 socket function implementation.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 26/06/2022
 */

#include "sticky/common/error.h"
#include "sticky/net/socket.h"

#ifndef STICKY_WINDOWS
#error This source file cannot be compiled on non-Windows systems.
#endif /* STICKY_WINDOWS */

#include <windows.h>
#include <winsock2.h>

#define WINSOCK_VER_MAJ 2
#define WINSOCK_VER_MIN 2

static WSADATA wsa;

void
_S_socket_init(void)
{
	if (WSAStartup(MAKEWORD(WINSOCK_VER_MAJ, WINSOCK_VER_MIN), &wsa) != 0)
	{
		_S_error_other("WSA", "Failed to init WinSock: %d\n",
		               WSAGetLastError());
	}
}

void
_S_socket_free(void)
{
	WSACleanup();
}

