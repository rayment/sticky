/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * draw.c
 * Common drawing source.
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
#include "sticky/util/string.h"
#include "sticky/video/camera.h"
#include "sticky/video/mesh.h"
#include "sticky/video/draw.h"
#include "sticky/video/font.h"
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

#define FONT_VERTEX_SOURCE                                                  \
"#version 330\n                                                            "\
"in vec4 i_vertex;                                                         "\
"out vec2 g_texcoord;                                                      "\
"uniform mat4 u_projection;                                                "\
"void main()                                                               "\
"{                                                                         "\
"	g_texcoord = i_vertex.zw;                                              "\
"	gl_Position = u_projection * vec4(i_vertex.xy, 0.0, 1.0);              "\
"}"

#define FONT_FRAGMENT_SOURCE                                                \
"#version 330\n                                                            "\
"in vec2 g_texcoord;                                                       "\
"out vec4 o_color;                                                         "\
"uniform sampler2D u_text;                                                 "\
"uniform vec4 u_color;                                                     "\
"void main()                                                               "\
"{                                                                         "\
"	vec4 v_sample = vec4(1.0, 1.0, 1.0, texture(u_text, g_texcoord).r);    "\
"	o_color = u_color * v_sample;                                          "\
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

static Sshader *shader, *shader2d, *shader2dtex, *shaderfont;
static Smesh *line_mesh, *quad_mesh;
static GLuint vao2d, vbo2d, vbotex2d;
static Svec2 last2dfrom, last2dto;
static Sfloat font_buffer[S_GLYPH_BUFFER_SIZE*6*4];

void
_S_draw_init(Suint8 gl_maj,
             Suint8 gl_min)
{
	if (gl_maj < 3 || (gl_maj == 3 && gl_min < 3))
	{
		/* cannot generate the draw shaders on old GL versions */
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
	_S_GL(glDisable(GL_PROGRAM_POINT_SIZE));
	_S_CALL("S_shader_new",
	        shaderfont = S_shader_new(FONT_VERTEX_SOURCE,
	                                  strlen(FONT_VERTEX_SOURCE),
	                                  FONT_FRAGMENT_SOURCE,
	                                  strlen(FONT_FRAGMENT_SOURCE)));
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
_S_draw_free(void)
{
	if (shader)
	{
		_S_CALL("S_shader_delete", S_shader_delete(shader));
		_S_CALL("S_shader_delete", S_shader_delete(shader2d));
		_S_CALL("S_shader_delete", S_shader_delete(shader2dtex));
		_S_CALL("S_shader_delete", S_shader_delete(shaderfont));
		_S_CALL("S_mesh_delete", S_mesh_delete(line_mesh));
		_S_CALL("S_mesh_delete", S_mesh_delete(quad_mesh));
		_S_GL(glDeleteBuffers(1, &vbo2d));
		_S_GL(glDeleteBuffers(1, &vbotex2d));
		_S_GL(glDeleteVertexArrays(1, &vao2d));
	}
}

void
S_draw_line_3d(const Swindow *window,
               const Svec3 *from,
               const Svec3 *to)
{
	Smat4 projection, view, model;
	Svec3 scale;
	Squat quat;
	Sfloat dist;
	if (!window || !from || !to)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_draw_line_3d");
		return;
	}
	if (!window->cam)
		return;
	/* scale and rotation */
	_S_CALL("S_quat_lookpoint", S_quat_lookpoint(&quat, from, to));
	_S_CALL("S_vec3_distance", dist = S_vec3_distance(from, to));
	_S_CALL("S_vec3_fill", S_vec3_fill(&scale, dist));
	/* create model transform */
	_S_CALL("S_transform_set_pos",
	        S_transform_set_pos(window->dtrans, from));
	_S_CALL("S_transform_set_scale",
	        S_transform_set_scale(window->dtrans, &scale));
	_S_CALL("S_transform_set_rot",
	        S_transform_set_rot(window->dtrans, &quat));
	/* get matrices */
	/* TODO: Dirty check the camera. */
	_S_CALL("S_camera_get_perspective_matrix",
	        S_camera_get_perspective_matrix(window->cam, &projection));
	_S_CALL("S_camera_get_view_matrix",
	        S_camera_get_view_matrix(window->cam, &view));
	_S_CALL("S_transform_get_transformation_matrix",
	        S_transform_get_transformation_matrix(window->dtrans, &model));
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
	        S_shader_set_uniform_vec4(shader,
			                          "u_color", &window->dcolor));
	/* draw line */
	_S_CALL("_S_mesh_draw", _S_mesh_draw(line_mesh, S_MESH_LINES));
}

void
S_draw_point_3d(const Swindow *window,
                const Svec3 *point)
{
	Smat4 projection, view, model;
	if (!window || !point)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_draw_point_3d");
		return;
	}
	if (!window->cam)
		return;
	/* create model transform */
	_S_CALL("S_transform_set_pos",
	        S_transform_set_pos(window->dtrans, point));
	/* get matrices */
	/* TODO: Dirty check the camera. */
	_S_CALL("S_camera_get_perspective_matrix",
	        S_camera_get_perspective_matrix(window->cam, &projection));
	_S_CALL("S_camera_get_view_matrix",
	        S_camera_get_view_matrix(window->cam, &view));
	_S_CALL("S_transform_get_transformation_matrix",
	        S_transform_get_transformation_matrix(window->dtrans, &model));
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
	        S_shader_set_uniform_vec4(shader,
			                          "u_color", &window->dcolor));
	_S_CALL("_S_mesh_draw_count",
	        _S_mesh_draw_count(line_mesh, S_MESH_POINTS, 1));
}

void
S_draw_quad_2d(const Swindow *window,
               const Svec2 *from,
               const Svec2 *to)
{
	Smat4 projection;
	Sshader *quadshader;
	Sbool b1, b2;
	if (!window || !from || !to)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_draw_quad_2d");
		return;
	}
	if (!window->cam)
		return;
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
		if (window->dtex)
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
			_S_CALL("_S_texture_attach", _S_texture_attach(window->dtex, 0));
		}
		_S_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
	/* get matrix */
	/* TODO: Dirty check the camera. */
	_S_CALL("S_camera_get_orthographic_matrix",
	        S_camera_get_orthographic_matrix(window->cam, &projection));
	/* set shader uniforms */
	if (window->dtex)
		quadshader = shader2dtex;
	else
		quadshader = shader2d;
	_S_CALL("_S_shader_attach", _S_shader_attach(quadshader));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(quadshader,
	                                  "u_projection", &projection));
	_S_CALL("S_shader_set_uniform_vec4",
	        S_shader_set_uniform_vec4(quadshader, "u_color", &window->dcolor));
	/* draw */
	_S_GL(glDisable(GL_DEPTH_TEST));
	_S_GL(glDrawArrays(GL_TRIANGLES, 0, 6));
	_S_GL(glBindVertexArray(0));
	_S_GL(glEnable(GL_DEPTH_TEST));
}

void
S_draw_string_2d(const Swindow *window,
                 const Sfont *font,
				 const Sstring *str,
                 Sfloat x,
                 Sfloat y,
                 Sfloat scale)
{
	Ssize_t len;
	const Schar *chr;
	if (!window || !font || !str || scale <= 0.0f)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_draw_string_2d");
		return;
	}
	_S_CALL("S_string_length", len = S_string_length(str));
	_S_CALL("S_string_ptr", chr = S_string_ptr(str));
	_S_CALL("S_draw_text_2d",
	        S_draw_text_2d(window, font, chr, len, x, y, scale));
}

void
S_draw_text_2d(const Swindow *window,
               const Sfont *font,
               const Schar *text,
               Ssize_t len,
               Sfloat x,
               Sfloat y,
               Sfloat scale)
{
	Schar c;
	Ssize_t i;
	const _Sglyph *glyph;
	Sfloat xp, yp, w, h, offx, offy, offw, offh;
	Sint32 tris, chars;
	Smat4 projection;

	if (!window || !font || !text || scale <= 0.0f)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_draw_text_2d");
		return;
	}

	if (!window->cam || len == 0)
		return;

	_S_CALL("_S_shader_attach", _S_shader_attach(shaderfont));
	_S_CALL("S_shader_set_uniform_vec4",
	        S_shader_set_uniform_vec4(shaderfont, "u_color", &window->dcolor));
	/* TODO: Dirty check the camera. */
	_S_CALL("S_camera_get_orthographic_matrix",
	        S_camera_get_orthographic_matrix(window->cam, &projection));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(shaderfont, "u_projection",
	                                  &projection));

	_S_GL(glDisable(GL_DEPTH_TEST));
	_S_GL(glActiveTexture(GL_TEXTURE0));
	_S_GL(glBindTexture(GL_TEXTURE_2D, font->texture));
	_S_GL(glBindVertexArray(font->vao));
	_S_GL(glBindBuffer(GL_ARRAY_BUFFER, font->vbo));

	tris = 0;
	chars = 0;

	while (1)
	{
		for (i = 0; i < len; ++i)
		{
			c = *(text+i);
			glyph = font->glyphs+c-S_GLYPH_BEGIN;
			w = glyph->size.x * scale;
			h = glyph->size.y * scale;

			xp = x + glyph->xbearing * scale;
			yp = y - (glyph->size.y - glyph->ybearing) * scale;

			x += glyph->xadvance * scale;
			y += glyph->yadvance * scale;

			if (w == 0 || h == 0)
				continue; /* skip empty glyphs */

			offx = glyph->offset.x;
			offy = glyph->offset.y;
			offw = glyph->size.x / font->width;
			offh = glyph->size.y / font->height;

			font_buffer[4*(tris+0)+0] = xp;
			font_buffer[4*(tris+0)+1] = yp + h;
			font_buffer[4*(tris+0)+2] = offx;
			font_buffer[4*(tris+0)+3] = offy;
			font_buffer[4*(tris+1)+0] = xp;
			font_buffer[4*(tris+1)+1] = yp;
			font_buffer[4*(tris+1)+2] = offx;
			font_buffer[4*(tris+1)+3] = offy + offh;
			font_buffer[4*(tris+2)+0] = xp + w;
			font_buffer[4*(tris+2)+1] = yp;
			font_buffer[4*(tris+2)+2] = offx + offw;
			font_buffer[4*(tris+2)+3] = offy + offh;
			font_buffer[4*(tris+3)+0] = xp;
			font_buffer[4*(tris+3)+1] = yp + h;
			font_buffer[4*(tris+3)+2] = offx;
			font_buffer[4*(tris+3)+3] = offy;
			font_buffer[4*(tris+4)+0] = xp + w;
			font_buffer[4*(tris+4)+1] = yp;
			font_buffer[4*(tris+4)+2] = offx + offw;
			font_buffer[4*(tris+4)+3] = offy + offh;
			font_buffer[4*(tris+5)+0] = xp + w;
			font_buffer[4*(tris+5)+1] = yp + h;
			font_buffer[4*(tris+5)+2] = offx + offw;
			font_buffer[4*(tris+5)+3] = offy;

			tris += 6;
			++chars;

			if (chars >= S_GLYPH_BUFFER_SIZE)
			{
				++i;
				break;
			}
		}

		if (tris > 0)
		{
			_S_GL(glBufferSubData(GL_ARRAY_BUFFER, 0,
			                      tris * 4 * sizeof(Sfloat), font_buffer));
			_S_GL(glDrawArrays(GL_TRIANGLES, 0, tris));
		}

		if (i >= len)
			break;

		tris = 0;
		chars = 0;
	}

	_S_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	_S_GL(glBindVertexArray(0));
	_S_GL(glBindTexture(GL_TEXTURE_2D, 0));
	_S_GL(glEnable(GL_DEPTH_TEST));
}

void
S_draw_set_color(Swindow *window,
                 Sfloat r,
                 Sfloat g,
                 Sfloat b,
                 Sfloat a)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_draw_set_color");
		return;
	}
	r = S_clamp(r, 0.0f, 1.0f);
	g = S_clamp(g, 0.0f, 1.0f);
	b = S_clamp(b, 0.0f, 1.0f);
	a = S_clamp(a, 0.0f, 1.0f);
	_S_CALL("S_vec4_set", S_vec4_set(&window->dcolor, r, g, b, a));
}

void
S_draw_set_texture(Swindow *window,
                   const Stexture *tex)
{
	if (!window)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_draw_set_texture");
		return;
	}
	window->dtex = tex;
}

