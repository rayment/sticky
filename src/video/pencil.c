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
#include "sticky/math/vec2.h"
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
"uniform vec4 u_color;                                                     "\
"void main()                                                               "\
"{                                                                         "\
"	o_color = u_color;                                                     "\
"}"

#define DRAW_VERTEX_SOURCE_2D                                               \
"#version 330\n                                                            "\
"layout (location = 0) in vec2 i_vertex;                                   "\
"uniform mat4 u_projection;                                                "\
"void main()                                                               "\
"{                                                                         "\
"	gl_Position = u_projection * vec4(i_vertex.xy, 0.0, 1.0);              "\
"}"

#define DRAW_FRAGMENT_SOURCE_2D                                             \
"#version 330\n                                                            "\
"out vec4 o_color;                                                         "\
"uniform vec4 u_color;                                                     "\
"void main()                                                               "\
"{                                                                         "\
"	o_color = u_color;                                                     "\
"}"

#define DRAW_VERTEX_SOURCE_2DTEX                                            \
"#version 330\n                                                            "\
"layout (location = 0) in vec3 i_vertex;                                   "\
"layout (location = 1) in vec2 i_texcoord;                                 "\
"out vec2 g_texcoord;                                                      "\
"uniform mat4 u_projection;                                                "\
"void main()                                                               "\
"{                                                                         "\
"	g_texcoord = i_texcoord;                                               "\
"	gl_Position = u_projection * vec4(i_vertex.xy, 0.0, 1.0);              "\
"}"

#define DRAW_FRAGMENT_SOURCE_2DTEX                                          \
"#version 330\n                                                            "\
"in vec2 g_texcoord;                                                       "\
"out vec4 o_color;                                                         "\
"uniform vec4 u_color;                                                     "\
"uniform sampler2D u_tex;                                                  "\
"void main()                                                               "\
"{                                                                         "\
"	o_color = texture(u_tex, g_texcoord) * u_color;                        "\
"}"

#define POINT_SIZE 0.005f

static
Sfloat line_vertices[6] =
{
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f
};

static
Sfloat quad_vertices[12] =
{
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};

static
Sfloat quad_texcoords[12] =
{
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};

static Sshader *shader, *shader2d, *shader2dtex;
static Smesh *line_mesh, *quad_mesh;
static GLuint vao2d, vbo2d, vbotex2d;
static Svec2 last2dfrom, last2dto;

void
_S_pencil_init(Suint8 gl_maj,
               Suint8 gl_min)
{
	if (gl_maj < 3 || (gl_maj == 3 && gl_min < 3))
	{
		/* cannot generate the pencil shaders on old GL versions */
		shader = NULL;
		shader2d = NULL;
		shader2dtex = NULL;
		line_mesh = NULL;
		quad_mesh = NULL;
		return;
	}
	/* generate shaders */
	_S_CALL("S_shader_new",
		    shader2d = S_shader_new(DRAW_VERTEX_SOURCE_2D,
	                                strlen(DRAW_VERTEX_SOURCE_2D),
	                                DRAW_FRAGMENT_SOURCE_2D,
	                                strlen(DRAW_FRAGMENT_SOURCE_2D)));
	_S_CALL("S_shader_new",
		    shader2dtex = S_shader_new(DRAW_VERTEX_SOURCE_2DTEX,
	                                   strlen(DRAW_VERTEX_SOURCE_2DTEX),
	                                   DRAW_FRAGMENT_SOURCE_2DTEX,
	                                   strlen(DRAW_FRAGMENT_SOURCE_2DTEX)));
	_S_GL(glEnable(GL_PROGRAM_POINT_SIZE));
	_S_CALL("S_shader_new",
	        shader = S_shader_new(DRAW_VERTEX_SOURCE,
	                              strlen(DRAW_VERTEX_SOURCE),
	                              DRAW_FRAGMENT_SOURCE,
	                              strlen(DRAW_FRAGMENT_SOURCE)));
	/* generate vbos and vaos */
	//_S_GL(glGenVertexArrays(1, &vao3d));
	//_S_GL(glGenBuffers(1, &vbo3d));
	_S_GL(glGenVertexArrays(1, &vao2d));
	_S_GL(glGenBuffers(1, &vbo2d));
	_S_GL(glGenBuffers(1, &vbotex2d));

	_S_CALL("S_vec2_fill", S_vec2_fill(&last2dfrom, -1.0f));
	_S_CALL("S_vec2_fill", S_vec2_fill(&last2dto, -1.0f));

	/* generate meshes */
	_S_CALL("S_mesh_new",
	        line_mesh = S_mesh_new(line_vertices, 6,
	                               NULL, 0,
	                               NULL, 0,
	                               NULL, 0));
	_S_CALL("S_mesh_new",
	        quad_mesh = S_mesh_new(quad_vertices, 18,
	                               NULL, 0,
	                               NULL, 0,
	                               quad_texcoords, 12));
}

void
_S_pencil_free(void)
{
	if (shader)
	{
		_S_CALL("S_shader_delete", S_shader_delete(shader));
		_S_CALL("S_shader_delete", S_shader_delete(shader2d));
		_S_CALL("S_shader_delete", S_shader_delete(shader2dtex));
		_S_CALL("S_mesh_delete", S_mesh_delete(line_mesh));
		_S_CALL("S_mesh_delete", S_mesh_delete(quad_mesh));
		_S_GL(glDeleteBuffers(1, &vbo2d));
		_S_GL(glDeleteBuffers(1, &vbotex2d));
		_S_GL(glDeleteVertexArrays(1, &vao2d));
	}
}

Spencil *
S_pencil_new(void)
{
	Spencil *pencil;

	pencil = (Spencil *) S_memory_new(sizeof(Spencil));

	pencil->camera = NULL;
	pencil->texture = NULL;
	_S_CALL("S_vec4_fill", S_vec4_fill(&pencil->color, 1.0f));

	_S_CALL("S_transform_new", pencil->transform = S_transform_new());

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
	_S_CALL("S_transform_delete", S_transform_delete(pencil->transform));
	S_memory_delete(pencil);
}

void
S_pencil_draw_line_3d(const Spencil *pencil,
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
	_S_CALL("_S_shader_attach", _S_shader_attach(shader));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(shader,
	                                  "u_projection", &projection));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(shader,
	                                  "u_view", &view));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(shader,
	                                  "u_model", &model));
	_S_CALL("S_shader_set_uniform_vec4",
	        S_shader_set_uniform_vec4(shader, "u_color", &pencil->color));
	/* draw line */
	_S_CALL("_S_mesh_draw", _S_mesh_draw(line_mesh, S_MESH_LINES));
}

void
S_pencil_draw_point_3d(const Spencil *pencil,
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
	_S_CALL("_S_shader_attach", _S_shader_attach(shader));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(shader,
	                                  "u_projection", &projection));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(shader,
	                                  "u_view", &view));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(shader,
	                                  "u_model", &model));
	_S_CALL("S_shader_set_uniform_vec4",
	        S_shader_set_uniform_vec4(shader, "u_color", &pencil->color));
	_S_CALL("_S_mesh_draw_count",
	        _S_mesh_draw_count(line_mesh, S_MESH_POINTS, 1));
}

void
S_pencil_draw_quad_2d(const Spencil *pencil,
                      const Svec2 *from,
                      const Svec2 *to)
{
	Smat4 projection;
	Sshader *quadshader;
	Sbool b1, b2;
	if (!pencil || !from || !to)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_pencil_draw_quad");
		return;
	}
	_S_CALL("S_vec2_equals", b1 = S_vec2_equals(S_EPSILON, from, &last2dfrom));
	_S_CALL("S_vec2_equals", b2 = S_vec2_equals(S_EPSILON, to, &last2dto));
	_S_GL(glBindVertexArray(vao2d));
	if (!b1 || !b2)
	{
		/* the quad is not the same as the last drawn quad, so we must
		   recalculate the vertices/texcoords again and buffer them into the
		   vbos */
		/* calculate vertices */
		quad_vertices[(0*2)  ] = from->x;
		quad_vertices[(0*2)+1] = from->y;
		quad_vertices[(1*2)  ] = to->x;
		quad_vertices[(1*2)+1] = to->y;
		quad_vertices[(2*2)  ] = to->x;
		quad_vertices[(2*2)+1] = from->y;
		quad_vertices[(3*2)  ] = to->x;
		quad_vertices[(3*2)+1] = to->y;
		quad_vertices[(4*2)  ] = from->x;
		quad_vertices[(4*2)+1] = from->y;
		quad_vertices[(5*2)  ] = from->x;
		quad_vertices[(5*2)+1] = to->y;

		/* buffer the vertices */
		_S_GL(glBindBuffer(GL_ARRAY_BUFFER, vbo2d));
		_S_GL(glBufferData(GL_ARRAY_BUFFER,
		                   12 * sizeof(Sfloat),
		                   quad_vertices,
		                   GL_STATIC_DRAW));
		_S_GL(glEnableVertexAttribArray(0));
		_S_GL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
		                            2 * sizeof(Sfloat), (void *) 0));
		if (pencil->texture)
		{
			_S_GL(glBindBuffer(GL_ARRAY_BUFFER, vbotex2d));
			/* buffer the tex coords */
			_S_GL(glBufferData(GL_ARRAY_BUFFER,
			                   12 * sizeof(Sfloat),
			                   quad_texcoords,
			                   GL_STATIC_DRAW));
			_S_GL(glEnableVertexAttribArray(1));
			_S_GL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
				                        2 * sizeof(Sfloat), (void *) 0));
			_S_texture_attach(pencil->texture, 0);
		}
		_S_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
	/* get matrix */
	/* TODO: Dirty check the camera. */
	_S_CALL("S_camera_get_orthographic_matrix",
	        S_camera_get_orthographic_matrix(pencil->camera, &projection));
	/* set shader uniforms */
	if (pencil->texture)
		quadshader = shader2dtex;
	else
		quadshader = shader2d;
	_S_CALL("_S_shader_attach", _S_shader_attach(quadshader));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(quadshader,
	                                  "u_projection", &projection));
	_S_CALL("S_shader_set_uniform_vec4",
	        S_shader_set_uniform_vec4(quadshader, "u_color", &pencil->color));
	/* draw */
	_S_GL(glDisable(GL_DEPTH_TEST));
	_S_GL(glDrawArrays(GL_TRIANGLES, 0, 6));
	_S_GL(glBindVertexArray(0));
	_S_GL(glEnable(GL_DEPTH_TEST));
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
                   float r,
                   float g,
                   float b,
				   float a)
{
	if (!pencil)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_pencil_set_color");
		return;
	}
	r = S_clamp(r, 0.0f, 1.0f);
	g = S_clamp(g, 0.0f, 1.0f);
	b = S_clamp(b, 0.0f, 1.0f);
	a = S_clamp(a, 0.0f, 1.0f);
	_S_CALL("S_vec4_set", S_vec4_set(&pencil->color, r, g, b, a));
}

void
S_pencil_set_texture(Spencil *pencil,
                     const Stexture *tex)
{
	if (!pencil)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_pencil_set_texture");
		return;
	}
	pencil->texture = tex;
}

