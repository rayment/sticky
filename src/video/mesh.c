/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * mesh.c
 * Graphical mesh header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

/*
typedef struct
Smesh_s
{
	Suint32 vbo, vao, ebo, normals, uv;
	Ssize_t vlen, ilen, uvlen;
	Sbool use_indices;
	Svec3 bounds_min, bounds_max;
} Smesh;
*/

#include "sticky/common/error.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/math/vec3.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/mesh.h"

static
Suint32
_S_mesh_gen_attribute(Suint32 attrib,
                      const Sfloat *data,
                      Ssize_t len,
                      Suint32 dimensions)
{
	Suint32 id;
	_S_GL(glGenBuffers(1, &id));
	_S_GL(glBindBuffer(GL_ARRAY_BUFFER, id));
	_S_GL(glBufferData(GL_ARRAY_BUFFER, len, data, GL_STATIC_DRAW));
	_S_GL(glVertexAttribPointer(attrib, dimensions, GL_FLOAT, GL_FALSE,
	                            dimensions * sizeof(Sfloat), (void *) 0));
	_S_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	_S_GL(glEnableVertexAttribArray(attrib));
	return id;
}

Smesh *
S_mesh_new(const Sfloat *vertices, Ssize_t vlen,
           const Suint32 *indices, Ssize_t ilen,
           const Sfloat *normals, Ssize_t nlen,
           const Sfloat *uv, Ssize_t uvlen)
{
	Smesh *mesh;
	Sfloat f, minx, miny, minz, maxx, maxy, maxz;
	Sbool xset, yset, zset;
	Ssize_t i, j;

	if (!vertices || vlen == 0 ||
	    (!indices && ilen > 0) ||
	    (!normals && nlen > 0) ||
	    (!uv && uvlen > 0))
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mesh_new");
		return NULL;
	}

	mesh = (Smesh *) S_memory_new(sizeof(Smesh));
	_S_GL(glGenVertexArrays(1, &mesh->vao));
	_S_GL(glBindVertexArray(mesh->vao));

	xset = yset = zset = S_FALSE;
	minx = miny = minz = maxx = maxy = maxz = 0.0f;
	/* TODO: Parallelise. */
	for (i = 0; i < vlen / 3; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			f = *(vertices+(i*3)+j);
			switch (j)
			{
			case 0:
				/* x vertex */
				if (!xset || f < minx)
					minx = f;
				if (!xset || f > maxx)
					maxx = f;
				xset = S_TRUE;
				break;
			case 1:
				/* y vertex */
				if (!yset || f < miny)
					miny = f;
				if (!yset || f > maxy)
					maxy = f;
				yset = S_TRUE;
				break;
			case 2:
				/* z vertex */
				if (!zset || f < minz)
					minz = f;
				if (!zset || f > maxz)
					maxz = f;
				zset = S_TRUE;
				break;
			}
		}
	}
	_S_CALL("S_vec3_set", S_vec3_set(&mesh->bounds_min, minx, miny, minz));
	_S_CALL("S_vec3_set", S_vec3_set(&mesh->bounds_max, maxx, maxy, maxz));

	/* multiply the buffer counts by their respective sizes before buffering
	   into the vbo and vao */
	vlen *= sizeof(Sfloat);
	ilen *= sizeof(Suint32);
	nlen *= sizeof(Sfloat);
	uvlen *= sizeof(Sfloat);

	/* buffer everything and setup the attribs */
	mesh->use_indices = S_FALSE;
	if (indices && ilen > 0)
	{
		mesh->use_indices = S_TRUE;
		_S_GL(glGenBuffers(1, &mesh->ebo));
		_S_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo));
		_S_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, ilen, indices,
		                   GL_STATIC_DRAW));
	}
	_S_CALL("_S_mesh_gen_attribute",
	        mesh->vbo = _S_mesh_gen_attribute(0, vertices, vlen, 3));
	if (normals && nlen > 0)
	{
		_S_CALL("_S_mesh_gen_attribute",
		        mesh->normals = _S_mesh_gen_attribute(1, normals, nlen, 3));
	}
	if (uv && uvlen > 0)
	{
		_S_CALL("_S_mesh_gen_attribute",
		        mesh->uv = _S_mesh_gen_attribute(2, uv, uvlen, 2));
	}
	_S_GL(glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo));
	_S_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
	                            3 * sizeof(Sfloat), (void *) 0));
	_S_GL(glEnableVertexAttribArray(0));

	mesh->vlen = vlen;
	mesh->ilen = ilen;
	mesh->uvlen = uvlen;

	return mesh;
}

void
S_mesh_delete(Smesh *mesh)
{
	if (!mesh)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mesh_delete");
		return;
	}
	_S_GL(glDeleteVertexArrays(1, &mesh->vao));
	if (mesh->uvlen > 0)
	{
		_S_GL(glDeleteVertexArrays(1, &mesh->uv));
	}
	_S_GL(glDeleteBuffers(1, &mesh->vbo));
	S_memory_delete(mesh);
}

/* TODO: Redo when framebuffers are implemented? */
void
_S_mesh_draw(const Smesh *mesh)
{
	if (!mesh)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_mesh_draw");
		return;
	}
	_S_GL(glBindVertexArray(mesh->vao));
	if (mesh->use_indices)
	{
		_S_GL(glDrawElements(GL_TRIANGLES, mesh->ilen, GL_UNSIGNED_INT, 0));
	}
	else
	{
		_S_GL(glDrawArrays(GL_TRIANGLES, 0, mesh->vlen / sizeof(Sfloat) / 3));
	}
	_S_GL(glBindVertexArray(0));
}

