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

STICKY_API Ssocket  *S_tcp_bind(Senum, Suint16, Sbool);
STICKY_API Ssocket  *S_tcp_bind_local(Senum, Suint16, Sbool);
STICKY_API void      S_tcp_listen(Ssocket *, Sint32);
STICKY_API Ssocket  *S_tcp_accept(Ssocket *);
STICKY_API Ssocket  *S_tcp_connect(Senum, const Schar *, Suint16, Sbool);
STICKY_API Sssize_t  S_tcp_recv(Ssocket *, Schar *, Ssize_t);
STICKY_API Sssize_t  S_tcp_send(Ssocket *, const Schar *, Ssize_t);
STICKY_API Sbool     S_tcp_poll(Ssocket *, Sint32);
STICKY_API void      S_tcp_close(Ssocket *);
STICKY_API Senum     S_tcp_get_state(Ssocket *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_TCP_H */

