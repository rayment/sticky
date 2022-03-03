/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * camera.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/mat4.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/camera.h"

static inline
void
_S_camera_set_perspective(Scamera *camera)
{
	_S_CALL("S_mat4_perspective", S_mat4_perspective(&camera->perspective,
	                                                 camera->fov,
	                                                 camera->aspect,
	                                                 camera->near,
	                                                 camera->far));
}

Scamera *
S_camera_new(Suint32 width,
             Suint32 height)
{
	Scamera *camera;
	camera = S_memory_new(sizeof(Scamera));
	_S_CALL("S_transform_new", camera->transform = S_transform_new());
	camera->near = 1.0f;
	camera->far = 100.0f;
	camera->fov = 60.0f;
	camera->aspect = ((Sfloat) width) / ((Sfloat) height);
	_S_CALL("_S_camera_set_perspective", _S_camera_set_perspective(camera));
	return camera;
}

void
S_camera_delete(Scamera *camera)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_delete");
		return;
	}
	_S_CALL("S_transform_delete", S_transform_delete(camera->transform));
	S_memory_delete(camera);
}

void
S_camera_set_near_plane(Scamera *camera,
                        Sfloat near)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_set_near_plane");
		return;
	}
	camera->near = near;
	_S_CALL("_S_camera_set_perspective", _S_camera_set_perspective(camera));
}

void
S_camera_set_far_plane(Scamera *camera,
                       Sfloat far)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_set_far_plane");
		return;
	}
	camera->far = far;
	_S_CALL("_S_camera_set_perspective", _S_camera_set_perspective(camera));
}

void
S_camera_set_field_of_view(Scamera *camera,
                           Sfloat fov)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_set_field_of_view");
		return;
	}
	camera->fov = fov;
	_S_CALL("_S_camera_set_perspective", _S_camera_set_perspective(camera));
}

void
S_camera_set_aspect_ratio(Scamera *camera,
                          Sfloat aspect)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_set_aspect_ratio");
		return;
	}
	camera->aspect = aspect;
	_S_CALL("_S_camera_set_perspective", _S_camera_set_perspective(camera));
}

Sfloat
S_camera_get_near_plane(const Scamera *camera)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_get_near_plane");
		return 0.0f;
	}
	return camera->near;
}

Sfloat
S_camera_get_far_plane(const Scamera *camera)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_get_far_plane");
		return 0.0f;
	}
	return camera->far;
}

Sfloat
S_camera_get_field_of_view(const Scamera *camera)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_get_field_of_view");
		return 0.0f;
	}
	return camera->fov;
}

Sfloat
S_camera_get_aspect_ratio(const Scamera *camera)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_get_aspect_ratio");
		return 0.0f;
	}
	return camera->aspect;
}

Stransform *
S_camera_get_transform(const Scamera *camera)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_get_transform");
		return NULL;
	}
	return camera->transform;
}

void
_S_camera_perspective(const Scamera *camera,
                      Smat4 *dest)
{
	if (!camera || !dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_camera_perspective");
		return;
	}
	_S_CALL("S_mat4_copy", S_mat4_copy(dest, &camera->perspective));
}

