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

/**
 * @brief Collection of 3D vertices as visual objects.
 *
 * Meshes are the visual geometry of any given object that is rendered in a
 * {@link Swindow}. Each mesh is made up of multiple vertices from which faces
 * are created and rendered on the screen. Meshes may additionally make use of a
 * UV map to map textures onto models giving photographic detail to a given
 * model.
 *
 * @since 1.0.0
 */
typedef struct
Smesh_s
{
	Suint32 vbo, vao, ebo, normals, uv;
	Ssize_t vlen, ilen, uvlen;
	Sbool use_indices;
	Svec3 bounds_min, bounds_max;
} Smesh;

/**
 * @brief Generate a new mesh from components.
 *
 * Creates a new mesh from individual vertex, indice, normal and UV components.
 *
 * Note that rendering will be performed using an element buffer object if the
 * indices array is not <c>NULL</c> which may lead to better rendering
 * performance than drawing via. the vertex array object.
 *
 * Note that the @p indices, @p normals and @p uv pointers may be <c>NULL</c>,
 * in which case their respective lengths must also be equal to 0 to avoid an
 * error.
 *
 * @param[in] vertices The array of vertices (3 per point) from which to create
 * a mesh.
 * @param[in] vlen The number of elements in the @p vertices array.
 * @param[in] indices The array of indices (3 per point) from which to create a
 * mesh.
 * @param[in] ilen The number of elements in the @p indices array.
 * @param[in] normals The array of normals (3 per point) from which to create a
 * mesh.
 * @param[in] nlen The number of elements in the @p normals array.
 * @param[in] uv The array of UV points (2 per point) from which to create a
 * mesh.
 * @param[in] uvlen The number of elements in the @p uv array.
 * @return A given mesh allocated in memory.
 * @exception S_INVALID_VALUE If @p vertices is <c>NULL</c>, @p vlen is equal to
 * 0, or any of @p indices, @p normals or @p uv are <c>NULL</c> but their
 * lengths are not 0.
 * @since 1.0.0
 */
Smesh *S_mesh_new(const Sfloat *, Ssize_t,
                  const Suint32 *, Ssize_t,
                  const Sfloat *, Ssize_t,
                  const Sfloat *, Ssize_t);

/**
 * @brief Free a mesh from memory.
 *
 * Once this function is called for a given mesh, that mesh becomes invalid and
 * may not be used again in any other mesh or rendering function.
 *
 * @param[in,out] mesh The mesh to be free'd.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid mesh is provided to
 * the function.
 * @since 1.0.0
 */
void   S_mesh_delete(Smesh *);

void  _S_mesh_draw(const Smesh *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_MESH_H */

