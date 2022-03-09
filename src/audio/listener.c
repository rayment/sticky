/*
 * listener.c
 * AL listener source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 04/05/2021
 */

#include "sticky/audio/listener.h"
#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/quat.h"
#include "sticky/math/vec3.h"
#include "sticky/memory/allocator.h"

static Slistener *active = NULL;

Slistener *
S_listener_new(void)
{
	Slistener *listener;
	listener = (Slistener *) S_memory_new(sizeof(Slistener));
	_S_CALL("S_vec3_zero", S_vec3_zero(&listener->pos));
	_S_CALL("S_vec3_zero", S_vec3_zero(&listener->vel));
	_S_CALL("S_quat_identity", S_quat_identity(&listener->rot));
	return listener;
}

void
S_listener_delete(Slistener *listener)
{
	if (!listener)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_listener_delete");
		return;
	}
	if (active == listener)
		active = NULL;
	S_memory_delete(listener);
}

static inline
void
_S_listener_update_pos(const Slistener *listener)
{
	_S_AL(alListener3f(AL_POSITION,
	                   listener->pos.x, listener->pos.y, listener->pos.z));
}

static inline
void
_S_listener_update_vel(const Slistener *listener)
{
	_S_AL(alListener3f(AL_VELOCITY,
	                   listener->vel.x, listener->vel.y, listener->vel.z));
}

static inline
void
_S_listener_update_rot(const Slistener *listener)
{
	Svec3 forward, up;
	Sfloat orientation[6];
	_S_CALL("S_quat_forward", S_quat_forward(&forward, &listener->rot));
	_S_CALL("S_quat_up", S_quat_up(&up, &listener->rot));
	*(orientation+0) = -forward.x;
	*(orientation+1) =  forward.y;
	*(orientation+2) =  forward.z;
	*(orientation+3) =  up.x;
	*(orientation+4) =  up.y;
	*(orientation+5) =  up.z;
	_S_AL(alListenerfv(AL_ORIENTATION, orientation));
}

void
S_listener_set_active(Slistener *listener)
{
	if (!listener)
	{
		active = NULL;
		return;
	}
	if (active != listener)
	{
		active = listener;
		_S_CALL("_S_listener_update_pos", _S_listener_update_pos(listener));
		_S_CALL("_S_listener_update_vel", _S_listener_update_vel(listener));
		_S_CALL("_S_listener_update_rot", _S_listener_update_rot(listener));
	}
}

void
S_listener_set_rot(Slistener *listener,
                   const Squat *rot)
{
	if (!listener || !rot)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_listener_set_rot");
		return;
	}
	_S_CALL("S_quat_copy", S_quat_copy(&listener->rot, rot));
	if (active == listener)
		_S_CALL("_S_listener_update_rot", _S_listener_update_rot(listener));
}

void
S_listener_add_rot(Slistener *listener,
                   const Squat *rot)
{
	if (!listener || !rot)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_listener_add_rot");
		return;
	}
	_S_CALL("S_quat_multiply", S_quat_multiply(&listener->rot, rot));
	if (active == listener)
		_S_CALL("_S_listener_update_rot", _S_listener_update_rot(listener));
}

void
S_listener_set_pos(Slistener *listener,
                   const Svec3 *pos)
{
	if (!listener || !pos)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_listener_set_pos");
		return;
	}
	_S_CALL("S_vec3_copy", S_vec3_copy(&listener->pos, pos));
	if (active == listener)
		_S_CALL("_S_listener_update_pos", _S_listener_update_pos(listener));
}

void
S_listener_add_pos(Slistener *listener,
                   const Svec3 *pos)
{
	if (!listener || !pos)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_listener_add_pos");
		return;
	}
	_S_CALL("S_vec3_add", S_vec3_add(&listener->pos, pos));
	if (active == listener)
		_S_CALL("_S_listener_update_pos", _S_listener_update_pos(listener));
}

void
S_listener_set_vel(Slistener *listener,
                   const Svec3 *vel)
{
	if (!listener || !vel)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_listener_set_vel");
		return;
	}
	_S_CALL("S_vec3_copy", S_vec3_copy(&listener->vel, vel));
	if (active == listener)
		_S_CALL("_S_listener_update_vel", _S_listener_update_vel(listener));
}

void
S_listener_add_vel(Slistener *listener,
                   const Svec3 *vel)
{
	if (!listener || !vel)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_listener_add_vel");
		return;
	}
	_S_CALL("S_vec3_add", S_vec3_add(&listener->vel, vel));
	if (active == listener)
		_S_CALL("_S_listener_update_vel", _S_listener_update_vel(listener));
}

Sbool
S_listener_is_active(const Slistener *listener)
{
	if (!listener)
		return active == NULL;
	return active == listener;
}

void
S_listener_get_pos(const Slistener *listener,
                   Svec3 *dest)
{
	if (!listener || !dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_listener_get_pos");
		return;
	}
	_S_CALL("S_vec3_copy", S_vec3_copy(dest, &listener->pos));
}

void
S_listener_get_vel(const Slistener *listener,
                   Svec3 *dest)
{
	if (!listener || !dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_listener_get_vel");
		return;
	}
	_S_CALL("S_vec3_copy", S_vec3_copy(dest, &listener->vel));
}

void
S_listener_get_rot(const Slistener *listener,
                   Squat *dest)
{
	if (!listener || !dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_listener_get_rot");
		return;
	}
	_S_CALL("S_quat_copy", S_quat_copy(dest, &listener->rot));
}

