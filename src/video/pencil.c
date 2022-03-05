/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * pencil.c
 * Primitive drawing source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 05/03/2022
 */

#include <string.h>

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/quat.h"
#include "sticky/math/transform.h"
#include "sticky/math/vec3.h"
#include "sticky/math/vec4.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/camera.h"
#include "sticky/video/pencil.h"
#include "sticky/video/shader.h"

#define DRAW_VERTEX_SOURCE                                                  \
"#version 330\n                                                            "\
"layout (location = 0) in vec3 i_position;                                 "\
"uniform mat4 u_model;                                                     "\
"uniform mat4 u_view;                                                      "\
"uniform mat4 u_projection;                                                "\
"void main()                                                               "\
"{                                                                         "\
"	gl_Position = u_projection * u_view * u_model * vec4(i_position, 1.0); "\
"}"

#define DRAW_FRAGMENT_SOURCE                                                \
"#version 330\n                                                            "\
"out vec4 o_color;                                                         "\
"uniform vec3 u_color;                                                     "\
"void main()                                                               "\
"{                                                                         "\
"	o_color = vec4(u_color, 1.0);                                          "\
"}"

#define POINT_SIZE 0.005f

static
Sfloat vertices[6] =
{
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f
};

Spencil *
S_pencil_new(void)
{
	Spencil *pencil;
	Svec3 color;

	pencil = S_memory_new(sizeof(Spencil));

	_S_CALL("S_shader_new",
	        pencil->shader = S_shader_new(DRAW_VERTEX_SOURCE,
	                                    strlen(DRAW_VERTEX_SOURCE),
	                                    DRAW_FRAGMENT_SOURCE,
	                                    strlen(DRAW_FRAGMENT_SOURCE)));

	_S_GL(glGenVertexArrays(1, &pencil->vao));
	_S_GL(glGenBuffers(1, &pencil->vbo));

	_S_GL(glBindVertexArray(pencil->vao));
	_S_GL(glBindBuffer(GL_ARRAY_BUFFER, pencil->vbo));
	_S_GL(glBufferData(GL_ARRAY_BUFFER,
	                   sizeof(vertices),
	                   vertices,
	                   GL_STATIC_DRAW));
	_S_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
	                            3 * sizeof(float), (void *) 0));
	_S_GL(glEnableVertexAttribArray(0));

	pencil->camera = NULL;

	_S_CALL("S_transform_new", pencil->transform = S_transform_new());

	_S_CALL("S_vec3_fill", S_vec3_fill(&color, 1.0f));
	_S_CALL("S_pencil_set_color", S_pencil_set_color(pencil, &color));

	return pencil;
}

void
S_pencil_delete(Spencil *pencil)
{
	if (!pencil)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_pencil_delete");
		return;
	}
	_S_CALL("S_shader_delete", S_shader_delete(pencil->shader));
	_S_CALL("S_transform_delete", S_transform_delete(pencil->transform));
	S_memory_delete(pencil);
}

void
S_pencil_draw_line(const Spencil *pencil,
                   const Svec3 *from,
                   const Svec3 *to)
{
	Smat4 projection, view, model;
	Svec3 scale;
	Squat quat;
	Sfloat dist;
	if (!pencil || !from || !to)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_pencil_line");
		return;
	}
	if (!pencil->camera)
		return;
	/* scale and rotation */
	_S_CALL("S_quat_lookpoint", S_quat_lookpoint(&quat, from, to));
	_S_CALL("S_vec3_distance", dist = S_vec3_distance(from, to));
	_S_CALL("S_vec3_fill", S_vec3_fill(&scale, dist));
	/* create model transform */
	_S_CALL("S_transform_set_pos",
	        S_transform_set_pos(pencil->transform, from));
	_S_CALL("S_transform_set_scale",
	        S_transform_set_scale(pencil->transform, &scale));
	_S_CALL("S_transform_set_rot",
	        S_transform_set_rot(pencil->transform, &quat));
	/* get matrices */
	_S_CALL("S_camera_get_projection_matrix",
	        S_camera_get_projection_matrix(pencil->camera, &projection));
	_S_CALL("S_camera_get_view_matrix",
	        S_camera_get_view_matrix(pencil->camera, &view));
	_S_CALL("S_transform_get_transformation_matrix",
	        S_transform_get_transformation_matrix(pencil->transform, &model));
	/* set shader uniforms */
	_S_CALL("_S_shader_attach", _S_shader_attach(pencil->shader));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(pencil->shader,
	                                  "u_projection", &projection));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(pencil->shader,
	                                  "u_view", &view));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(pencil->shader,
	                                  "u_model", &model));
	_S_GL(glBindVertexArray(pencil->vao));
	_S_GL(glDrawArrays(GL_LINES, 0, 2));
	/* TODO: Unbind necessary? */
	_S_GL(glBindVertexArray(0));
}

void
S_pencil_draw_point(const Spencil *pencil,
                    const Svec3 *point)
{
	Svec3 a, b, c, d, e, f;
	if (!pencil || !point)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_pencil_draw_point");
		return;
	}
	a.x = point->x - POINT_SIZE; a.y = point->y; a.z = point->z;
	b.x = point->x + POINT_SIZE; b.y = point->y; b.z = point->z;
	c.x = point->x; c.y = point->y - POINT_SIZE; c.z = point->z;
	d.x = point->x; d.y = point->y + POINT_SIZE; d.z = point->z;
	e.x = point->x; e.y = point->y; e.z = point->z - POINT_SIZE;
	f.x = point->x; f.y = point->y; f.z = point->z + POINT_SIZE;
	_S_CALL("S_pencil_draw_line", S_pencil_draw_line(pencil, &a, &b));
	_S_CALL("S_pencil_draw_line", S_pencil_draw_line(pencil, &c, &d));
	_S_CALL("S_pencil_draw_line", S_pencil_draw_line(pencil, &e, &f));
}

void
S_pencil_set_camera(Spencil *pencil,
                  const Scamera *camera)
{
	if (!pencil || !camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_pencil_set_camera");
		return;
	}
	pencil->camera = camera;
}

void
S_pencil_set_color(Spencil *pencil,
                 const Svec3 *color)
{
	if (!pencil || !color)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_pencil_set_color");
		return;
	}
	_S_CALL("_S_shader_attach", _S_shader_attach(pencil->shader));
	_S_CALL("S_shader_set_uniform_vec3",
	        S_shader_set_uniform_vec3(pencil->shader, "u_color", color));
}

