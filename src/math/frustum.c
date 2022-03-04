/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

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

static
void
_S_frustum_normalize(Sfrustum *frustum,
                     Senum side)
{
	Sfloat mag;
	mag = S_sqrt((*(frustum+side))->x*(*(frustum+side))->x +
	      (*(frustum+side))->y*(*(frustum+side))->y +
	      (*(frustum+side))->z*(*(frustum+side))->z);
	(*(frustum+side))->x /= mag;
	(*(frustum+side))->y /= mag;
	(*(frustum+side))->z /= mag;
	(*(frustum+side))->w /= mag;
}

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

	_S_frustum_normalize(frustum, S_FRUSTUM_RIGHT);
	_S_frustum_normalize(frustum, S_FRUSTUM_LEFT);
	_S_frustum_normalize(frustum, S_FRUSTUM_TOP);
	_S_frustum_normalize(frustum, S_FRUSTUM_BOTTOM);
	_S_frustum_normalize(frustum, S_FRUSTUM_FAR);
	_S_frustum_normalize(frustum, S_FRUSTUM_NEAR);
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

Sbool
S_frustum_intersects_sphere(const Sfrustum *frustum,
                            const Svec3 *point,
                            Sfloat radius)
{
	Suint8 i;
	Sfloat dot;
	if (!frustum || !point || radius < 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_frustum_intersects_sphere");
		return S_FALSE;
	}
	for (i = 0; i < 6; ++i)
	{
		dot = (*(frustum+i))->x*point->x +
		      (*(frustum+i))->y*point->y +
		      (*(frustum+i))->z*point->z;
		printf("%d %f\n", i, dot + (*(frustum+i))->w);
		if (dot + (*(frustum+i))->w < -radius)
			return S_FALSE;
	}
	return S_TRUE;
}

Sbool
S_frustum_intersects_bounds(const Sfrustum *frustum,
                            const Svec3 *min,
                            const Svec3 *max)
{
	Suint8 i;
	Sfloat dot;
	Svec3 a, b, c, d, e, f, g, h;
	if (!frustum || !min || !max)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_frustum_intersects_bounds");
		return S_FALSE;
	}
	a.x = min->x; a.y = min->y; a.z=min->z;
	b.x = min->x; b.y = min->y; b.z=max->z;
	c.x = min->x; c.y = max->y; c.z=min->z;
	d.x = min->x; d.y = max->y; d.z=max->z;
	e.x = max->x; e.y = min->y; e.z=min->z;
	f.x = max->x; f.y = min->y; f.z=max->z;
	g.x = max->x; g.y = max->y; g.z=min->z;
	h.x = max->x; h.y = max->y; h.z=max->z;
	for (i = 0; i < 6; ++i)
	{
		dot = (*(frustum+i))->x*a.x +
		      (*(frustum+i))->y*a.y +
		      (*(frustum+i))->z*a.z;
		if (dot + (*(frustum+i))->w >= 0)
			continue;
		dot = (*(frustum+i))->x*b.x +
		      (*(frustum+i))->y*b.y +
		      (*(frustum+i))->z*b.z;
		if (dot + (*(frustum+i))->w >= 0)
			continue;
		dot = (*(frustum+i))->x*c.x +
		      (*(frustum+i))->y*c.y +
		      (*(frustum+i))->z*c.z;
		if (dot + (*(frustum+i))->w >= 0)
			continue;
		dot = (*(frustum+i))->x*d.x +
		      (*(frustum+i))->y*d.y +
		      (*(frustum+i))->z*d.z;
		if (dot + (*(frustum+i))->w >= 0)
			continue;
		dot = (*(frustum+i))->x*e.x +
		      (*(frustum+i))->y*e.y +
		      (*(frustum+i))->z*e.z;
		if (dot + (*(frustum+i))->w >= 0)
			continue;
		dot = (*(frustum+i))->x*f.x +
		      (*(frustum+i))->y*f.y +
		      (*(frustum+i))->z*f.z;
		if (dot + (*(frustum+i))->w >= 0)
			continue;
		dot = (*(frustum+i))->x*g.x +
		      (*(frustum+i))->y*g.y +
		      (*(frustum+i))->z*g.z;
		if (dot + (*(frustum+i))->w >= 0)
			continue;
		dot = (*(frustum+i))->x*h.x +
		      (*(frustum+i))->y*h.y +
		      (*(frustum+i))->z*h.z;
		if (dot + (*(frustum+i))->w >= 0)
			continue;
		return S_FALSE;
	}
	return S_TRUE;
}

