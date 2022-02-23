/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mesh.h
 * Graphical mesh header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_MESH_H
#define FR_RAYMENT_STICKY_MESH_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/types.h"
#include "sticky/math/vec3.h"

/**
 * @addtogroup mesh
 * @{
 */

typedef struct
Smesh_s
{
	Suint32 vbo, vao, ebo, normals, uv;
	Ssize_t vlen, ilen, uvlen;
	Sbool use_indices;
	Svec3 bounds_min, bounds_max;
} Smesh;

Smesh *S_mesh_new(const Sfloat *, Ssize_t,
                  const Suint32 *, Ssize_t,
                  const Sfloat *, Ssize_t,
                  const Sfloat *, Ssize_t);

void   S_mesh_delete(Smesh *);

void  _S_mesh_draw(const Smesh *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MESH_H */

