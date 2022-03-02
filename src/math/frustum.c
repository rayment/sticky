/*
 * frustum.c
 * Culling frustum math source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 27/02/2022
 */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/frustum.h"
#include "sticky/math/math.h"
#include "sticky/math/vec3.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/camera.h"

#define NUM_SIDES 6

static
void
_S_frustum_normalize(Sfrustum *frustum,
                     Senum side)
{
	Sfloat mag;
	Svec3 zero;
	_S_CALL("S_vec3_zero", S_vec3_zero(&zero));
	_S_CALL("S_vec3_distance",
	        mag = S_vec3_distance(frustum->plane+side, &zero));
	frustum->plane[side].x /= mag;
	frustum->plane[side].y /= mag;
	frustum->plane[side].z /= mag;
	frustum->dist[side] /= mag;
}

/*
 * See: https://github.com/gametutorials/tutorials
 */
void
S_frustum_load(Sfrustum *frustum,
               const Scamera *camera)
{
	Smat4 perspective, view;
	Stransform *transform;

	if (!frustum || !camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_frustum_load");
		return;
	}

	_S_CALL("_S_camera_perspective",
	        _S_camera_perspective(camera, &perspective));
	_S_CALL("S_camera_get_transform",
	        transform = S_camera_get_transform(camera));
	_S_CALL("S_transform_get_view_matrix",
	        S_transform_get_transformation_matrix(transform, &view));
	_S_CALL("S_mat4_multiply", S_mat4_multiply(&view, &perspective));

	frustum->plane[S_FRUSTUM_RIGHT] .x    = view.m03 - view.m00;
	frustum->plane[S_FRUSTUM_RIGHT] .y    = view.m13 - view.m10;
	frustum->plane[S_FRUSTUM_RIGHT] .z    = view.m23 - view.m20;
	frustum->plane[S_FRUSTUM_LEFT]  .x    = view.m03 + view.m00;
	frustum->plane[S_FRUSTUM_LEFT]  .y    = view.m13 + view.m10;
	frustum->plane[S_FRUSTUM_LEFT]  .z    = view.m23 + view.m20;
	frustum->plane[S_FRUSTUM_TOP]   .x    = view.m03 - view.m01;
	frustum->plane[S_FRUSTUM_TOP]   .y    = view.m13 - view.m11;
	frustum->plane[S_FRUSTUM_TOP]   .z    = view.m23 - view.m21;
	frustum->plane[S_FRUSTUM_BOTTOM].x    = view.m03 + view.m01;
	frustum->plane[S_FRUSTUM_BOTTOM].y    = view.m13 + view.m11;
	frustum->plane[S_FRUSTUM_BOTTOM].z    = view.m23 + view.m21;
	frustum->plane[S_FRUSTUM_BACK]  .x    = view.m03 - view.m02;
	frustum->plane[S_FRUSTUM_BACK]  .y    = view.m13 - view.m12;
	frustum->plane[S_FRUSTUM_BACK]  .z    = view.m23 - view.m22;
	frustum->plane[S_FRUSTUM_FRONT] .x    = view.m03 + view.m02;
	frustum->plane[S_FRUSTUM_FRONT] .y    = view.m13 + view.m12;
	frustum->plane[S_FRUSTUM_FRONT] .z    = view.m23 + view.m22;
	frustum->dist[S_FRUSTUM_RIGHT]        = view.m33 - view.m30;
	frustum->dist[S_FRUSTUM_LEFT]         = view.m33 + view.m30;
	frustum->dist[S_FRUSTUM_TOP]          = view.m33 - view.m31;
	frustum->dist[S_FRUSTUM_BOTTOM]       = view.m33 + view.m31;
	frustum->dist[S_FRUSTUM_BACK]         = view.m33 - view.m32;
	frustum->dist[S_FRUSTUM_FRONT]        = view.m33 + view.m32;
	/* normalise planes */
	_S_frustum_normalize(frustum, S_FRUSTUM_RIGHT);
	_S_frustum_normalize(frustum, S_FRUSTUM_LEFT);
	_S_frustum_normalize(frustum, S_FRUSTUM_TOP);
	_S_frustum_normalize(frustum, S_FRUSTUM_BOTTOM);
	_S_frustum_normalize(frustum, S_FRUSTUM_BACK);
	_S_frustum_normalize(frustum, S_FRUSTUM_FRONT);
}

Sbool
S_frustum_intersects_point(const Sfrustum *frustum,
                           const Svec3 *point)
{
	Suint8 i;
	Sfloat dot;
	if (!frustum || !point)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_frustum_intersects_point");
		return S_FALSE;
	}
	for (i = 0; i < NUM_SIDES; ++i)
	{
		_S_CALL("S_vec3_dot", dot = S_vec3_dot(frustum->plane+i, point));
		if (dot + *(frustum->dist+i) <= 0)
			return S_FALSE;
	}
	return S_TRUE;
}

Sbool     S_frustum_intersects_bounds(const Sfrustum *,
                                      const Svec3 *, const Svec3 *);

