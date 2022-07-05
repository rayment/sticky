/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * socket_posix.h
 * Network socket header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 06/10/2021
 */

#ifndef FR_RAYMENT_STICKY_SOCKET_H
#define FR_RAYMENT_STICKY_SOCKET_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/collections/linkedlist.h"
#include "sticky/common/defines.h"
#include "sticky/common/types.h"

#include <errno.h>

#define S_FAMILY_IPV4         0x4
#define S_FAMILY_IPV6         0x6
#define S_FAMILY_ANY          0xf

#define S_SOCK_READY          1
#define S_SOCK_NODATA         2
#define S_SOCK_DIED           3

#define _S_SOCK_UNDEFINED     1
#define _S_SOCK_CLIENT_LOCAL  2
#define _S_SOCK_CLIENT_EXTERN 3
#define _S_SOCK_SERVER_LOCAL  4
#define _S_SOCK_SERVER_EXTERN 5

/* either EAGAIN or EWOULDBLOCK are possible depending on the implementation */
#define _S_SOCK_BLOCKED(x) ((x) == EAGAIN || (x) == EWOULDBLOCK)
#define _S_SOCK_DIED(x) ((x) == ECONNRESET || (x) == ETIMEDOUT)

#if defined(STICKY_POSIX)
#include <poll.h>
#elif defined(STICKY_WINDOWS) /* STICKY_POSIX */
#include <winsock2.h>
#endif /* STICKY_POSIX */

typedef
struct
Ssocket_s
{
	int fd;
	Senum type, family, state;
	Sbool blocking;
	Ssize_t poll_len;
	struct pollfd *polls;
	struct Ssocket_s *parent;
	Slinkedlist *children;
} Ssocket;

Sssize_t  S_socket_recvfrom(Ssocket *, Schar *, Ssize_t, Senum *);
Sssize_t  S_socket_sendto(Ssocket *, const Schar *, Ssize_t, Senum *);
void      S_socket_close(Ssocket *);
Sbool     S_socket_poll(Ssocket *, Sint32);

void _S_socket_init(void);
void _S_socket_free(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_SOCKET_H */

