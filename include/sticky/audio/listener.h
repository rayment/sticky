/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * listener.c
 * AL listener header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 04/05/2021
 */

#ifndef FR_RAYMENT_STICKY_LISTENER_H
#define FR_RAYMENT_STICKY_LISTENER_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/quat.h"
#include "sticky/math/vec3.h"

typedef struct
Slistener_s
{
	Svec3 pos, vel;
	Squat rot;
	Sbool active;
} Slistener;

Slistener *S_listener_new(void);
void       S_listener_delete(Slistener *);

void       S_listener_set_active(Slistener *);
void       S_listener_set_rot(Slistener *, const Squat *);
void       S_listener_set_pos(Slistener *, const Svec3 *);
void       S_listener_set_vel(Slistener *, const Svec3 *);
void       S_listener_add_rot(Slistener *, const Squat *);
void       S_listener_add_pos(Slistener *, const Svec3 *);
void       S_listener_add_vel(Slistener *, const Svec3 *);

Sbool      S_listener_is_active(const Slistener *);
void       S_listener_get_pos(const Slistener *, Svec3 *);
void       S_listener_get_vel(const Slistener *, Svec3 *);
void       S_listener_get_rot(const Slistener *, Squat *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_LISTENER_H */

