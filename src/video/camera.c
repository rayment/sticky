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
#include "sticky/memory/allocator.h"
#include "sticky/video/camera.h"

Scamera *
S_camera_new(void)
{
	Scamera *camera;
	camera = S_memory_new(sizeof(Scamera));
	_S_CALL("S_transform_new", camera->transform = S_transform_new());
	camera->near = 1.0f;
	camera->far = 100.0f;
	camera->fov = 60.0f;
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

