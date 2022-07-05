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

#include "sticky/common/types.h"
#include "sticky/net/socket.h"

Ssocket  *S_tcp_bind(Senum, Suint16, Sbool);
Ssocket  *S_tcp_bind_local(Senum, Suint16, Sbool);
void      S_tcp_listen(Ssocket *, Sint32);
Ssocket  *S_tcp_accept(Ssocket *);
Ssocket  *S_tcp_connect(Senum, const Schar *, Suint16, Sbool);
Sssize_t  S_tcp_recv(Ssocket *, Schar *, Ssize_t);
Sssize_t  S_tcp_send(Ssocket *, const Schar *, Ssize_t);
Sbool     S_tcp_poll(Ssocket *, Sint32);
void      S_tcp_close(Ssocket *);
Senum     S_tcp_get_state(Ssocket *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_TCP_H */

