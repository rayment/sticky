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
#include "sticky/math/vec4.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/camera.h"

/*
 * See: https://gdbooks.gitbooks.io/legacyopengl/content/Chapter8/frustum.html
 */
void
S_frustum_load(Sfrustum *frustum,
               const Scamera *camera)
{
	Smat4 persp, view;
	Stransform *transform;
	Svec4 r0, r1, r2;

	if (!frustum || !camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_frustum_load");
		return;
	}

	_S_CALL("_S_camera_perspective",
	        _S_camera_perspective(camera, &persp));
	_S_CALL("S_camera_get_transform",
	        transform = S_camera_get_transform(camera));
	_S_CALL("S_transform_get_view_matrix",
	        S_transform_get_view_matrix(transform, &view));
	_S_CALL("S_mat4_multiply", S_mat4_multiply(&persp, &view));

	_S_CALL("S_vec4_set",
		S_vec4_set(&r0, persp.m00, persp.m01, persp.m02, persp.m03));
	_S_CALL("S_vec4_set",
		S_vec4_set(&r1, persp.m10, persp.m11, persp.m12, persp.m13));
	_S_CALL("S_vec4_set",
		S_vec4_set(&r2, persp.m20, persp.m21, persp.m22, persp.m23));

	_S_CALL("S_vec4_set",
		S_vec4_set(*(frustum+S_FRUSTUM_RIGHT),
		           persp.m30, persp.m31, persp.m32, persp.m33));
	_S_CALL("S_vec4_set",
		S_vec4_set(*(frustum+S_FRUSTUM_LEFT),
		           persp.m30, persp.m31, persp.m32, persp.m33));
	_S_CALL("S_vec4_set",
		S_vec4_set(*(frustum+S_FRUSTUM_TOP),
		           persp.m30, persp.m31, persp.m32, persp.m33));
	_S_CALL("S_vec4_set",
		S_vec4_set(*(frustum+S_FRUSTUM_BOTTOM),
		           persp.m30, persp.m31, persp.m32, persp.m33));
	_S_CALL("S_vec4_set",
		S_vec4_set(*(frustum+S_FRUSTUM_FAR),
		           persp.m30, persp.m31, persp.m32, persp.m33));
	_S_CALL("S_vec4_set",
		S_vec4_set(*(frustum+S_FRUSTUM_NEAR),
		           persp.m30, persp.m31, persp.m32, persp.m33));

	_S_CALL("S_vec4_subtract",
		S_vec4_subtract(*(frustum+S_FRUSTUM_RIGHT), &r0));
	_S_CALL("S_vec4_add",
		S_vec4_add(     *(frustum+S_FRUSTUM_LEFT), &r0));
	_S_CALL("S_vec4_subtract",
		S_vec4_subtract(*(frustum+S_FRUSTUM_TOP), &r1));
	_S_CALL("S_vec4_add",
		S_vec4_add(     *(frustum+S_FRUSTUM_BOTTOM), &r1));
	_S_CALL("S_vec4_subtract",
		S_vec4_subtract(*(frustum+S_FRUSTUM_FAR), &r2));
	_S_CALL("S_vec4_add",
		S_vec4_add(     *(frustum+S_FRUSTUM_NEAR), &r2));
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
	for (i = 0; i < 6; ++i)
	{
		dot = (*(frustum+i))->x*point->x +
		      (*(frustum+i))->y*point->y +
		      (*(frustum+i))->z*point->z;
		if (dot + (*(frustum+i))->w < 0.0f)
			return S_FALSE;
	}
	return S_TRUE;
}

Sbool     S_frustum_intersects_bounds(const Sfrustum *,
                                      const Svec3 *, const Svec3 *);
