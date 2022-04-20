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
#include "sticky/math/transform.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/camera.h"

static inline
void
_S_camera_set_projection(Scamera *camera)
{
	_S_CALL("S_mat4_perspective", S_mat4_perspective(&camera->perspective,
	                                                 camera->fov,
	                                                 camera->aspect,
	                                                 camera->near,
	                                                 camera->far));
	_S_CALL("S_mat4_orthographic", S_mat4_orthographic(&camera->orthographic,
	                                                   camera->width,
	                                                   camera->height));
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
	camera->width = width;
	camera->height = height;
	_S_CALL("_S_camera_set_projection", _S_camera_set_projection(camera));
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
	_S_CALL("_S_camera_set_projection", _S_camera_set_projection(camera));
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
	_S_CALL("_S_camera_set_projection", _S_camera_set_projection(camera));
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
	_S_CALL("_S_camera_set_projection", _S_camera_set_projection(camera));
}

void
S_camera_set_size(Scamera *camera,
                  Suint32 width,
                  Suint32 height)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_set_size");
		return;
	}
	camera->width = width;
	camera->height = height;
	camera->aspect = (Sfloat) width / (Sfloat) height;
	_S_CALL("_S_camera_set_projection", _S_camera_set_projection(camera));
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

void
S_camera_get_size(const Scamera *camera,
                          Suint32 *width,
                          Suint32 *height)
{
	if (!camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_get_size");
		return;
	}
	if (width)
		*width = camera->width;
	if (height)
		*height = camera->height;
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
S_camera_get_perspective_matrix(const Scamera *camera,
                                Smat4 *dest)
{
	if (!camera || !dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_get_perspective_matrix");
		return;
	}
	_S_CALL("S_mat4_copy", S_mat4_copy(dest, &camera->perspective));
}

void
S_camera_get_orthographic_matrix(const Scamera *camera,
                                 Smat4 *dest)
{
	if (!camera || !dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_get_orthographic_matrix");
		return;
	}
	_S_CALL("S_mat4_copy", S_mat4_copy(dest, &camera->orthographic));
}

void
S_camera_get_view_matrix(const Scamera *camera,
                         Smat4 *dest)
{
	if (!camera || !dest)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_camera_get_view_matrix");
		return;
	}
	_S_CALL("S_transform_get_transformation_matrix",
	        S_transform_get_transformation_matrix(camera->transform, dest));
	_S_CALL("S_mat4_inverse", S_mat4_inverse(dest));
}

