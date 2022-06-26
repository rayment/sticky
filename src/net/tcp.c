/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * tcp.c
 * TCP networking implementation.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 06/10/2021
 */

#include "sticky/net/tcp.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#if defined(STICKY_POSIX)
#include <fcntl.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#elif defined(STICKY_WINDOWS)
#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#endif /* STICKY_POSIX */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/memory/allocator.h"

#define TCP_BACKLOG_SIZE 5

static
Ssocket *
_S_tcp_open(Senum family)
{
	Sint32 fd, domain;
	Ssocket *sock;
	if (family == S_FAMILY_IPV6)
		domain = AF_INET6;
	else
		domain = AF_INET;
	fd = socket(domain, SOCK_STREAM, 0);
	if (fd == -1)
	{
		_S_SET_ERROR(S_IO_ERROR, "_S_tcp_open");
		return NULL;
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(Sint32))
	    == -1)
	{
		_S_SET_ERROR(S_IO_ERROR, "_S_tcp_open");
		return NULL;
	}
#ifdef SO_REUSEPORT
	/* port reuse is not always defined */
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(Sint32))
	    == -1) 
	{
		_S_SET_ERROR(S_IO_ERROR, "_S_tcp_open");
		return NULL;
	}
#endif
	sock = (Ssocket *) S_memory_new(sizeof(Ssocket));
	sock->type = _S_SOCK_UNDEFINED;
	sock->family = domain;
	sock->fd = fd;
	sock->poll_len = 0;
	sock->polls = NULL;
	sock->blocking = S_TRUE;
	sock->parent = NULL;
	sock->children = NULL;
	sock->state = S_SOCK_READY;
	return sock;
}

static
Ssocket *
_S_tcp_unblock_socket(Ssocket *sock)
{
#if defined(STICKY_POSIX)
	Sint32 flags;
	flags = fcntl(sock->fd, F_GETFL, 0);
	if (flags == -1)
	{
		_S_SET_ERROR(S_IO_ERROR, "_S_tcp_unblock_socket");
		return NULL;
	}
	flags |= O_NONBLOCK;
	if (fcntl(sock->fd, F_SETFL, flags) != 0)
	{
		_S_SET_ERROR(S_IO_ERROR, "_S_tcp_unblock_socket");
		return NULL;
	}
#elif defined(STICKY_WINDOWS)
	if (ioctlsocket(sock, FIONBIO, &(u_long){1}) != 0)
	{
		_S_SET_ERROR(S_IO_ERROR, "_S_tcp_unblock_socket");
		return NULL;
	}
#endif
	sock->blocking = S_FALSE;
	return sock;
}

static
void
_S_tcp_fail_close(Ssocket *sock)
{
	if (sock && close(sock->fd) == -1)
	{
		S_warning("Double failure on forceful socket close (fd=%d).\n",
		          sock->fd);
	}
}

static
void
_S_tcp_gen_poll(Ssocket *sock,
                Sint32 fd)
{
	if (sock->poll_len++ == 0)
	{
		sock->polls = (struct pollfd *)
			S_memory_new(sizeof(struct pollfd));
	}
	else
	{
		sock->polls = (struct pollfd *)
			S_memory_resize(sock->polls,
			                sizeof(struct pollfd) * sock->poll_len);
	}
	(sock->polls+sock->poll_len-1)->fd = fd;
	/* mark read poll event */
	(sock->polls+sock->poll_len-1)->events = POLLIN;
}

Ssocket *
S_tcp_bind(Senum family,
           Suint16 port,
           Sbool blocking)
{
	Ssocket *sock;
	Ssize_t saddr_len;
	struct sockaddr *saddr;
	struct sockaddr_in saddr4;
	struct sockaddr_in6 saddr6;
	if (family != S_FAMILY_IPV4 && family != S_FAMILY_IPV6)
	{
		_S_SET_ERROR(S_INVALID_ENUM, "S_tcp_bind");
		return NULL;
	}
	_S_CALL("_S_tcp_open", sock = _S_tcp_open(family));
	if (family == S_FAMILY_IPV4)
	{
		memset(&saddr4, 0, sizeof(struct sockaddr_in));
		saddr4.sin_family = sock->family;
		saddr4.sin_port = htons(port);
		saddr4.sin_addr.s_addr = htonl(INADDR_ANY);
		saddr = (struct sockaddr *) &saddr4;
		saddr_len = sizeof(saddr4);
	}
	else
	{
		memset(&saddr6, 0, sizeof(struct sockaddr_in6));
		saddr6.sin6_family = sock->family;
		saddr6.sin6_port = htons(port);
		saddr6.sin6_addr = in6addr_any;
		saddr = (struct sockaddr *) &saddr6;
		saddr_len = sizeof(saddr6);
	}
	if (bind(sock->fd, saddr, saddr_len) == -1)
	{
#if defined(STICKY_POSIX)
		if (errno == EADDRINUSE)
#elif defined(STICKY_WINDOWS)
		if (errno == WSAEADDRINUSE)
#endif /* STICKY_POSIX */
		{
			_S_SET_ERROR(S_PORT_IN_USE, "S_tcp_bind");
		}
#if defined(STICKY_POSIX)
		else if (errno == EACCES)
#elif defined(STICKY_WINDOWS)
		else if (errno == WSAEACCES)
#endif /* STICKY_POSIX */
		{
			_S_SET_ERROR(S_INVALID_ACCESS, "S_tcp_bind");
		}
		else
		{
			_S_SET_ERROR(S_IO_ERROR, "S_tcp_bind");
		}
		_S_CALL("_S_tcp_fail_close", _S_tcp_fail_close(sock));
		return NULL;
	}
	if (!blocking)
	{
		/* bind successful, now block */
		_S_CALL("_S_tcp_unblock_socket", sock = _S_tcp_unblock_socket(sock));
		if (!sock)
		{
			_S_CALL("S_tcp_close", S_tcp_close(sock));
			return NULL;
		}
	}
	sock->type = _S_SOCK_SERVER_LOCAL;
	_S_CALL("S_linkedlist_new", sock->children = S_linkedlist_new());
	_S_CALL("_S_tcp_gen_poll", _S_tcp_gen_poll(sock, sock->fd));
	return sock;
}

void
S_tcp_listen(Ssocket *sock,
             Sint32 backlog)
{
	if (!sock || backlog < 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tcp_listen");
		return;
	}
	else if (sock->type != _S_SOCK_SERVER_LOCAL)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_tcp_listen");
		return;
	}
	else if (listen(sock->fd, TCP_BACKLOG_SIZE) == -1)
	{
#if defined(STICKY_POSIX)
		if (errno == EADDRINUSE)
#elif defined(STICKY_WINDOWS)
		if (errno == WSAEADDRINUSE)
#endif /* STICKY_POSIX */
		{
			_S_SET_ERROR(S_PORT_IN_USE, "S_tcp_listen");
		}
		else
		{
			_S_SET_ERROR(S_IO_ERROR, "S_tcp_listen");
		}
		return;
	}
}

Ssocket *
S_tcp_accept(Ssocket *sock)
{
	Ssocket *client;
	struct sockaddr_in addr;
	socklen_t addrlen;
	Sint32 fd;
	if (!sock)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tcp_accept");
		return NULL;
	}
	else if (sock->type != _S_SOCK_SERVER_LOCAL)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_tcp_accept");
		return NULL;
	}
	addrlen = sizeof(addr);
	if ((fd = accept(sock->fd, (struct sockaddr *) &addr, &addrlen)) == -1)
	{
#if defined(STICKY_POSIX)
		if (errno == EAGAIN)
#elif defined(STICKY_WINDOWS)
		if (errno == WSAEWOULDBLOCK)
#endif /* STICKY_POSIX */
		{
			/* no clients trying to connect */
			_S_SET_ERROR(S_TIMEOUT, "S_tcp_accept");
		}
#ifdef STICKY_POSIX
		else if (errno == ECONNABORTED)
		{
			/* connection was aborted */
			_S_SET_ERROR(S_CONNECTION_ABORTED, "S_tcp_accept");
		}
#endif /* STICKY_POSIX */
		else
		{
			/* unknown error */
			_S_SET_ERROR(S_IO_ERROR, "S_tcp_accept");
		}
		return NULL;
	}
	client = (Ssocket *) S_memory_new(sizeof(Ssocket));
	client->fd = fd;
	if (!sock->blocking)
	{
		/* TODO: Don't all accepted sockets inherit the non-blocking state? */
		_S_CALL("_S_tcp_unblock_socket",
		        client = _S_tcp_unblock_socket(client));
		if (!client)
		{
			_S_CALL("S_tcp_close", S_tcp_close(client));
			return NULL;
		}
	}
	client->type = _S_SOCK_CLIENT_EXTERN;
	client->family = sock->family;
	client->poll_len = 0;
	client->polls = NULL;
	client->blocking = sock->blocking;
	client->parent = sock;
	client->children = NULL;
	client->state = S_SOCK_READY;
	if (sock->children)
	{
		_S_CALL("S_linkedlist_add_head",
		        S_linkedlist_add_head(sock->children, client));
	}
	_S_CALL("_S_tcp_gen_poll", _S_tcp_gen_poll(client, fd));
	_S_CALL("_S_tcp_gen_poll", _S_tcp_gen_poll(sock, fd));
	return client;
}

Sbool
_S_tcp_find_addr(Senum family,
                 Senum *resfamily,
                 struct addrinfo *info,
                 struct addrinfo **res)
{
	while (info)
	{
		if (family == S_FAMILY_ANY ||
		    (family == S_FAMILY_IPV4 && info->ai_family == AF_INET) ||
		    (family == S_FAMILY_IPV6 && info->ai_family == AF_INET6))
		{
			*res = info;
			if (info->ai_family == AF_INET)
				*resfamily = S_FAMILY_IPV4;
			else
				*resfamily = S_FAMILY_IPV6;
			return S_TRUE;
		}
		info = info->ai_next;
	}
	return S_FALSE;
}

Ssocket *
S_tcp_connect(Senum family,
              const Schar *addr,
              Suint16 port,
              Sbool blocking)
{
	Sint32 a;
	Sbool b;
	Senum resfamily;
	Schar portstr[6];
	Ssocket *sock;
	struct addrinfo *info, *resinfo;
	if (!addr)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tcp_connect");
		return NULL;
	}
	else if (family != S_FAMILY_ANY &&
	         family != S_FAMILY_IPV4 &&
	         family != S_FAMILY_IPV6)
	{
		_S_SET_ERROR(S_INVALID_ENUM, "S_tcp_connect");
		return NULL;
	}
	sprintf(portstr, "%d", port);
	if ((a = getaddrinfo(addr, portstr, NULL, &info)) != 0)
	{
#if defined(STICKY_POSIX)
		if (a == EAI_AGAIN)
#elif defined(STICKY_WINDOWS)
		if (a == WSATRY_AGAIN)
#endif /* STICKY_POSIX */
		{
			_S_SET_ERROR(S_NAMERES_FAIL, "S_tcp_connect");
		}
#ifdef STICKY_POSIX
		else if (a == EAI_SYSTEM)
		{
			_S_SET_ERROR(S_UNKNOWN_ERROR, "S_tcp_connect");
		}
#endif /* STICKY_POSIX */
		else
		{
			_S_SET_ERROR(S_NETWORK_ERROR, "S_tcp_connect");
		}
		return NULL;
	}
	/* begin use address */
	_S_CALL("_S_tcp_find_addr",
	        b = _S_tcp_find_addr(family, &resfamily, info, &resinfo));
	if (!b)
	{
		/* the requested family was not found in the list of returned addresses
		   so it should be treated as an error */
		_S_SET_ERROR(S_UNKNOWN_HOST, "S_tcp_connect");
		return NULL;
	}
	_S_CALL("_S_tcp_open", sock = _S_tcp_open(resfamily));
	if (connect(sock->fd, resinfo->ai_addr, resinfo->ai_addrlen) == -1)
	{
		if (errno == ECONNREFUSED)
		{
			_S_SET_ERROR(S_CONNECTION_REFUSED, "S_tcp_connect");
		}
		else
		{
			_S_SET_ERROR(S_IO_ERROR, "S_tcp_connect");
		}
		_S_CALL("_S_tcp_fail_close", _S_tcp_fail_close(sock));
		return NULL;
	}
	/* end use address */
	freeaddrinfo(info);
	if (!blocking)
	{
		/* connection successful, now make it non-blocking */
		_S_CALL("_S_tcp_unblock_socket", sock = _S_tcp_unblock_socket(sock));
		if (!sock)
		{
			_S_CALL("S_tcp_close", S_tcp_close(sock));
			return NULL;
		}
	}
	sock->type = _S_SOCK_CLIENT_LOCAL;
	_S_CALL("_S_tcp_gen_poll", _S_tcp_gen_poll(sock, sock->fd));
	return sock;
}

void
_S_tcp_clear_poll(Ssocket *sock,
                  int fd)
{
	Sbool shuffle;
	Ssize_t i;
	if (!sock)
		return;
	shuffle = S_FALSE;
	for (i = 0; i < sock->poll_len; ++i)
	{
		if ((sock->polls+i)->fd == fd)
			shuffle = S_TRUE;
		else if (shuffle) /* will skip idx 0 */
			(sock->polls+i-1)->fd = (sock->polls+i)->fd;
	}
	if (shuffle)
		--sock->poll_len;
}

void
S_tcp_close(Ssocket *sock)
{
	Sbool b;
	Slinkedlist_iter *iter;
	Ssocket *child;
	if (!sock)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tcp_close");
		return;
	}
	else if (close(sock->fd) == -1)
	{
		_S_SET_ERROR(S_IO_ERROR, "S_tcp_close");
	}
	else
	{
		if (sock->polls)
			S_memory_delete(sock->polls);
		if (sock->parent)
		{
			/* child (client) sockets may have a parent to notify */
			_S_CALL("_S_tcp_clear_poll",
			        _S_tcp_clear_poll(sock->parent, sock->fd));
			if (sock->parent->children)
			{
				_S_CALL("S_linkedlist_remove_ptr",
				        S_linkedlist_remove_ptr(sock->parent->children, sock));
			}
		}
		else if (sock->children)
		{
			/* parent (server) sockets may have child sockets to notify */
			_S_CALL("S_linkedlist_iter_begin",
			        iter = S_linkedlist_iter_begin(sock->children));
			while (1)
			{
				_S_CALL("S_linkedlist_iter_hasnext",
				        b = S_linkedlist_iter_hasnext(iter));
				if (!b)
					break;
				_S_CALL("S_linkedlist_iter_next",
				        child = (Ssocket *) S_linkedlist_iter_next(&iter));
				child->parent = NULL;
			}
			_S_CALL("S_linkedlist_delete",
			        S_linkedlist_delete(sock->children));
		}
	}
	S_memory_delete(sock);
}

Sssize_t
S_tcp_recv(Ssocket *sock,
           Schar *buf,
           Ssize_t len)
{
	Sssize_t rb;
	if (!sock || !buf)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tcp_recv");
		return -1;
	}
	else if (sock->type != _S_SOCK_SERVER_EXTERN &&
	         sock->type != _S_SOCK_CLIENT_EXTERN &&
	         sock->type != _S_SOCK_CLIENT_LOCAL)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_tcp_recv");
		return -1;
	}
	if ((rb = read(sock->fd, buf, len)) == -1)
	{
		if (!sock->blocking)
		{
			if (_S_SOCK_BLOCKED(errno))
			{
				sock->state = S_SOCK_NODATA;
				return 0; /* not an error, there was just no recv */
			}
		}
		if (_S_SOCK_DIED(errno))
		{
			sock->state = S_SOCK_DIED;
			/* remove the dead socket from the polling list */
			_S_CALL("_S_tcp_clear_poll", _S_tcp_clear_poll(sock, sock->fd));
			return 0; /* not an error, the socket just died */
		}
		_S_SET_ERROR(S_IO_ERROR, "S_tcp_recv");
		return -1;
	}
	else if (rb == 0)
	{
		/* socket was closed (return 0), the socket is closed and thus dead */
		sock->state = S_SOCK_DIED;
		return 0;
	}
	return rb;
}

Sssize_t
S_tcp_send(Ssocket *sock,
           const Schar *buf,
           Ssize_t len)
{
	Sssize_t sb;
	if (!sock || !buf)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tcp_send");
		return -1;
	}
	else if (sock->type != _S_SOCK_SERVER_LOCAL &&
	         sock->type != _S_SOCK_CLIENT_LOCAL &&
	         sock->type != _S_SOCK_CLIENT_EXTERN)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_tcp_send");
		return -1;
	}
	if ((sb = send(sock->fd, buf, len, 0)) == -1)
	{
		if (_S_SOCK_DIED(errno))
		{
			sock->state = S_SOCK_DIED;
			return 0; /* not an error, the socket just died */
		}
		else
		{
			_S_SET_ERROR(S_IO_ERROR, "S_tcp_send");
			return -1;
		}
	}
	return sb;
}

Sbool
S_tcp_poll(Ssocket *sock,
           Sint32 timeout)
{
	Sint32 ret;
	if (!sock || sock->poll_len == 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tcp_poll");
		return S_FALSE;
	}
	else if (sock->type != _S_SOCK_SERVER_LOCAL &&
	         sock->type != _S_SOCK_SERVER_EXTERN &&
	         sock->type != _S_SOCK_CLIENT_LOCAL &&
	         sock->type != _S_SOCK_CLIENT_EXTERN)
	{
		_S_SET_ERROR(S_INVALID_OPERATION, "S_tcp_poll");
		return -1;
	}
#if defined(STICKY_POSIX)
	ret = poll(sock->polls, sock->poll_len, timeout);
#elif defined(STICKY_WINDOWS)
	ret = WSAPoll(sock->polls, sock->poll_len, timeout);
#endif /* STICKY_POSIX */
	if (ret == -1)
	{
		_S_SET_ERROR(S_IO_ERROR, "S_tcp_poll");
		return S_FALSE;
	}
	return ret > 0;
}

Senum
S_tcp_get_state(Ssocket *sock)
{
	if (!sock)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tcp_get_state");
		return S_SOCK_DIED;
	}
	return sock->state;
}

