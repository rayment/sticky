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
	mag = S_sqrt((frustum->p+side)->x*(frustum->p+side)->x +
	      (frustum->p+side)->y*(frustum->p+side)->y +
	      (frustum->p+side)->z*(frustum->p+side)->z);
	(frustum->p+side)->x /= mag;
	(frustum->p+side)->y /= mag;
	(frustum->p+side)->z /= mag;
	(frustum->p+side)->w /= mag;
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
		S_vec4_set(frustum->p+S_FRUSTUM_RIGHT,
		           persp.m30, persp.m31, persp.m32, persp.m33));
	_S_CALL("S_vec4_set",
		S_vec4_set(frustum->p+S_FRUSTUM_LEFT,
		           persp.m30, persp.m31, persp.m32, persp.m33));
	_S_CALL("S_vec4_set",
		S_vec4_set(frustum->p+S_FRUSTUM_TOP,
		           persp.m30, persp.m31, persp.m32, persp.m33));
	_S_CALL("S_vec4_set",
		S_vec4_set(frustum->p+S_FRUSTUM_BOTTOM,
		           persp.m30, persp.m31, persp.m32, persp.m33));
	_S_CALL("S_vec4_set",
		S_vec4_set(frustum->p+S_FRUSTUM_FAR,
		           persp.m30, persp.m31, persp.m32, persp.m33));
	_S_CALL("S_vec4_set",
		S_vec4_set(frustum->p+S_FRUSTUM_NEAR,
		           persp.m30, persp.m31, persp.m32, persp.m33));

	_S_CALL("S_vec4_subtract",
		S_vec4_subtract(frustum->p+S_FRUSTUM_RIGHT,  &r0));
	_S_CALL("S_vec4_add",
		S_vec4_add(     frustum->p+S_FRUSTUM_LEFT,   &r0));
	_S_CALL("S_vec4_subtract",
		S_vec4_subtract(frustum->p+S_FRUSTUM_TOP,    &r1));
	_S_CALL("S_vec4_add",
		S_vec4_add(     frustum->p+S_FRUSTUM_BOTTOM, &r1));
	_S_CALL("S_vec4_subtract",
		S_vec4_subtract(frustum->p+S_FRUSTUM_FAR,    &r2));
	_S_CALL("S_vec4_add",
		S_vec4_add(     frustum->p+S_FRUSTUM_NEAR,   &r2));

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
		dot = (frustum->p+i)->x*point->x +
		      (frustum->p+i)->y*point->y +
		      (frustum->p+i)->z*point->z;
		if (dot + (frustum->p+i)->w < 0.0f)
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
		dot = (frustum->p+i)->x*point->x +
		      (frustum->p+i)->y*point->y +
		      (frustum->p+i)->z*point->z;
		if (dot + (frustum->p+i)->w < -radius)
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
		dot = (frustum->p+i)->x*a.x +
		      (frustum->p+i)->y*a.y +
		      (frustum->p+i)->z*a.z;
		if (dot + (frustum->p+i)->w >= 0)
			continue;
		dot = (frustum->p+i)->x*b.x +
		      (frustum->p+i)->y*b.y +
		      (frustum->p+i)->z*b.z;
		if (dot + (frustum->p+i)->w >= 0)
			continue;
		dot = (frustum->p+i)->x*c.x +
		      (frustum->p+i)->y*c.y +
		      (frustum->p+i)->z*c.z;
		if (dot + (frustum->p+i)->w >= 0)
			continue;
		dot = (frustum->p+i)->x*d.x +
		      (frustum->p+i)->y*d.y +
		      (frustum->p+i)->z*d.z;
		if (dot + (frustum->p+i)->w >= 0)
			continue;
		dot = (frustum->p+i)->x*e.x +
		      (frustum->p+i)->y*e.y +
		      (frustum->p+i)->z*e.z;
		if (dot + (frustum->p+i)->w >= 0)
			continue;
		dot = (frustum->p+i)->x*f.x +
		      (frustum->p+i)->y*f.y +
		      (frustum->p+i)->z*f.z;
		if (dot + (frustum->p+i)->w >= 0)
			continue;
		dot = (frustum->p+i)->x*g.x +
		      (frustum->p+i)->y*g.y +
		      (frustum->p+i)->z*g.z;
		if (dot + (frustum->p+i)->w >= 0)
			continue;
		dot = (frustum->p+i)->x*h.x +
		      (frustum->p+i)->y*h.y +
		      (frustum->p+i)->z*h.z;
		if (dot + (frustum->p+i)->w >= 0)
			continue;
		return S_FALSE;
	}
	return S_TRUE;
}

