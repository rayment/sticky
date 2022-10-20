/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * tcp.h
 * TCP networking header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 06/10/2021
 */

#ifndef FR_RAYMENT_STICKY_TCP_H
#define FR_RAYMENT_STICKY_TCP_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/types.h"
#include "sticky/net/socket.h"

/**
 * @brief Binds a new socket to a port.
 *
 * Binds a new socket to a given port and allocates memory for the socket on the
 * heap.
 *
 * Socket bindings may be synchronous and block each time data is received, or
 * asynchronous whereby data may or not be received in a given request.
 *
 * To only allow loopback connections, see
 * {@link S_tcp_bind(Senum, Suint16, Sbool)}.
 *
 * @param family {@link S_FAMILY_IPV4} or {@link S_FAMILY_IPV6}.
 * @param port The port to bind the socket to.
 * @param blocking Whether or not the socket should block.
 * @return A new socket allocated to the heap, or <c>NULL</c> in case of an
 * error.
 * @exception S_INVALID_ENUM If an invalid family is provided to the function.
 * @since 1.0.0
 */
STICKY_API Ssocket  *S_tcp_bind(Senum, Suint16, Sbool);

/**
 * @brief Binds a new loopback socket to a port.
 *
 * Binds a new socket to a given port and allocates memory for the socket on the
 * heap. The socket will only accept connections from the current machine.
 *
 * Socket bindings may be synchronous and block each time data is received, or
 * asynchronous whereby data may or not be received in a given request.
 *
 * To also accept connections from outside the system, see
 * {@link S_tcp_bind_local(Senum, Suint16, Sbool)}.
 *
 * @param family {@link S_FAMILY_IPV4} or {@link S_FAMILY_IPV6}.
 * @param port The port to bind the socket to.
 * @param blocking Whether or not the socket should block.
 * @return A new socket allocated to the heap, or <c>NULL</c> in case of an
 * error.
 * @exception S_INVALID_ENUM If an invalid family is provided to the function.
 * @since 1.0.0
 */
STICKY_API Ssocket  *S_tcp_bind_local(Senum, Suint16, Sbool);

/**
 * @brief Begin listening for incoming connections on a socket.
 *
 * Takes a socket and marks it as a passive socket so that it may accept
 * connections from other clients.
 *
 * This must be called after binding a socket to a port.
 *
 * @param sock The socket to listen to.
 * @param backlog The maximum number of incoming connections that may be queued
 * at any given point.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid socket is passed to
 * the function.
 * @since 1.0.0
 */
STICKY_API void      S_tcp_listen(Ssocket *, Sint32);

/**
 * @brief Accept an incoming socket connection.
 *
 * Note that the new client socket must be closed like server sockets by calling
 * {@link S_tcp_close(Ssocket *)} to safely terminate the connection and free
 * the socket from memory.
 *
 * @param sock The server socket from which the client socket is accepted.
 * @return A new client socket allocated on the heap.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid socket is passed to
 * the function.
 * @since 1.0.0
 */
STICKY_API Ssocket  *S_tcp_accept(Ssocket *);

STICKY_API Ssocket  *S_tcp_connect(Senum, const Schar *, Suint16, Sbool);

STICKY_API Sssize_t  S_tcp_recv(Ssocket *, Schar *, Ssize_t);

STICKY_API Sssize_t  S_tcp_send(Ssocket *, const Schar *, Ssize_t);

STICKY_API Sbool     S_tcp_poll(Ssocket *, Sint32);

/**
 * @brief Closes a given socket and its connection.
 *
 * Attempts to close a socket, its connection and free it from memory. Any
 * connection on the other end will be terminated once a send is attempted, or
 * after a timeout on receive.
 *
 * Once this function is called for a given TCP socket, that socket becomes
 * invalid and may not be used again in any other function.
 *
 * @param[in,out] sock The socket to free from memory.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid TCP socket is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void      S_tcp_close(Ssocket *);

/**
 * @brief Returns the current state of a given socket.
 *
 * Takes a TCP socket and determines whether or not it is ready to receive data,
 * it has died due to a terminated or broken connection, or if it is
 * non-blocking and has not yet received data.
 *
 * @param[in] sock The socket to get the state from.
 * @return {@link S_SOCK_READY}, {@link S_SOCK_NODATA} or {@link S_SOCK_DIED}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid TCP socket is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API Senum     S_tcp_get_state(Ssocket *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_TCP_H */

