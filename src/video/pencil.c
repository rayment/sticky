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
#include "sticky/video/mesh.h"
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
"   gl_PointSize = 8.0f;                                                   "\
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
Sfloat line_vertices[6] =
{
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f
};

Spencil *
S_pencil_new(void)
{
	Spencil *pencil;
	Svec3 color;

	pencil = (Spencil *) S_memory_new(sizeof(Spencil));

	_S_GL(glEnable(GL_PROGRAM_POINT_SIZE));
	_S_CALL("S_shader_new",
	        pencil->shader = S_shader_new(DRAW_VERTEX_SOURCE,
	                                      strlen(DRAW_VERTEX_SOURCE),
	                                      DRAW_FRAGMENT_SOURCE,
	                                      strlen(DRAW_FRAGMENT_SOURCE)));

	_S_CALL("S_mesh_new",
	        pencil->line_mesh = S_mesh_new(line_vertices, 6,
	                                       NULL, 0,
	                                       NULL, 0,
	                                       NULL, 0));

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
	_S_CALL("S_mesh_delete", S_mesh_delete(pencil->line_mesh));
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
	/* TODO: Dirty check the camera. */
	_S_CALL("S_camera_get_perspective_matrix",
	        S_camera_get_perspective_matrix(pencil->camera, &projection));
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
	/* draw line */
	_S_CALL("_S_mesh_draw", _S_mesh_draw(pencil->line_mesh, S_MESH_LINES));
}

void
S_pencil_draw_point(const Spencil *pencil,
                    const Svec3 *point)
{
	Smat4 projection, view, model;
	if (!pencil || !point)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_pencil_draw_point");
		return;
	}
	/* create model transform */
	_S_CALL("S_transform_set_pos",
	        S_transform_set_pos(pencil->transform, point));
	/* get matrices */
	/* TODO: Dirty check the camera. */
	_S_CALL("S_camera_get_perspective_matrix",
	        S_camera_get_perspective_matrix(pencil->camera, &projection));
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
	_S_CALL("_S_mesh_draw_count",
	        _S_mesh_draw_count(pencil->line_mesh, S_MESH_POINTS, 1));
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

