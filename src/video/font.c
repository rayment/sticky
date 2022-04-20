/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * font.c
 * FreeType font source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 19/04/2021
 */

#include <stdlib.h>
#include <string.h>

#include "sticky/common/error.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/math/math.h"
#include "sticky/math/vec2.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/camera.h"
#include "sticky/video/font.h"
#include "sticky/video/shader.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

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
"uniform vec3 u_color;                                                     "\
"void main()                                                               "\
"{                                                                         "\
"	vec4 v_sample = vec4(1.0, 1.0, 1.0, texture(u_text, g_texcoord).r);    "\
"	o_color = vec4(u_color, 1.0) * v_sample;                               "\
"}"

typedef struct _Stexture_node_s
{
	Schar id;
	Suint32 xorigin, yorigin, xsize, ysize;
	Sbool empty;
	struct _Stexture_node_s *left, *right;
} _Stexture_node;

static FT_Library font_library;
static Sfloat font_buffer[S_GLYPH_BUFFER_SIZE*6*4];

static Sshader *font_shader;

void
_S_font_init(void)
{
	Svec3 default_color;
	if (FT_Init_FreeType(&font_library) != 0)
		_S_error_freetype("Failed to init FreeType.");
	/* generate shader */
	_S_CALL("S_shader_new",
	        font_shader = S_shader_new(FONT_VERTEX_SOURCE,
	                                   strlen(FONT_VERTEX_SOURCE),
	                                   FONT_FRAGMENT_SOURCE,
	                                   strlen(FONT_FRAGMENT_SOURCE)));
	_S_CALL("S_vec3_set", S_vec3_set(&default_color, 1.0f, 1.0f, 1.0f));
	_S_CALL("S_shader_set_uniform_vec3",
	        S_shader_set_uniform_vec3(font_shader, "u_color", &default_color));
}

void
_S_font_free(void)
{
	FT_Done_FreeType(font_library);
	_S_CALL("S_shader_delete", S_shader_delete(font_shader));
}

static
int
_S_font_comparator(const void *p1,
                   const void *p2)
{
	_Sglyph *a, *b;
	Suint32 acond, bcond;

	a = (_Sglyph *) p1;
	b = (_Sglyph *) p2;
	acond = a->xtexsize * a->ytexsize;
	bcond = b->xtexsize * b->ytexsize;

	if (acond < bcond)
		return 1;
	else if (acond == bcond)
		return 0;
	else
		return -1;
}

static
void
_S_font_destroy_packing_tree(_Stexture_node *root)
{
	if (!root)
		return;
	_S_CALL("_S_font_destroy_packing_tree",
	        _S_font_destroy_packing_tree(root->left));
	_S_CALL("_S_font_destroy_packing_tree",
	        _S_font_destroy_packing_tree(root->right));
	S_memory_delete(root);
}

static
_Stexture_node *
_S_font_find_glyph(_Stexture_node *root,
                   Schar id)
{
	_Stexture_node *child;
	if (!root)
		return NULL;
	if (root->id == id)
		return root;
	_S_CALL("_S_font_find_glyph", child = _S_font_find_glyph(root->left, id));
	if (!child)
	{
		_S_CALL("_S_font_find_glyph",
		        child = _S_font_find_glyph(root->right, id));
	}
	return child;
}

static
_Stexture_node *
_S_font_pack_texture(_Stexture_node *node,
                     Schar id,
                     Suint32 xsize,
                     Suint32 ysize,
                     Suint32 xtexsize,
                     Suint32 ytexsize)
{
	_Stexture_node *n, *left, *right, *tmp;
	Suint32 xs, ys;
	Sint32 rx, ry;
	Sbool vsplit;

	if (!node || !node->empty)
	{
		/* node already full, do nothing */
		return NULL;
	}
	else if (node->left && node->right)
	{
		/* not a leaf, try left insert, then right */
		_S_CALL("_S_font_pack_texture",
		n = _S_font_pack_texture(node->left, id,
		                         xsize, ysize,
		                         xtexsize, ytexsize));
		if (n)
			return n;
		_S_CALL("_S_font_pack_texture",
		tmp =  _S_font_pack_texture(node->right, id,
		                            xsize, ysize,
		                            xtexsize, ytexsize));
		return tmp;
	}
	else
	{
		/* leaf ready to be packed */
		xs = node->xsize;
		ys = node->ysize;

		/* if on a boundary, calculate the actual size */
		if (node->xorigin + node->xsize == S_UINT32_MAX)
			xs = xtexsize - node->xorigin;
		if (node->yorigin + node->ysize == S_UINT32_MAX)
			ys = ytexsize - node->yorigin;

		if (node->xsize == xsize && node->ysize == ysize)
		{
			/* size is exact, pack directly */
			node->empty = S_FALSE;
			return node;
		}
		else if (xs < xsize || ys < ysize)
		{
			/* node not big enough */
			return NULL;
		}
		else
		{
			/* node is large enough but needs splitting */
			rx = xs - xsize;
			ry = ys - ysize;
			vsplit = rx < ry;

			/* split whichever way leaves the most room */
			if (rx == 0 && ry == 0)
				vsplit = node->xsize <= node->ysize;

			left = (_Stexture_node *) S_memory_new(sizeof(_Stexture_node));
			right = (_Stexture_node *) S_memory_new(sizeof(_Stexture_node));
			left->id = 0;
			left->empty = S_TRUE;
			left->left = NULL;
			left->right = NULL;
			right->id = 0;
			right->empty = S_TRUE;
			right->left = NULL;
			right->right = NULL;
			if (vsplit)
			{
				/* split vertically (left is top) */
				left->xorigin = node->xorigin;
				left->yorigin = node->yorigin;
				left->xsize = node->xsize;
				left->ysize = ysize;
				right->xorigin = node->xorigin;
				right->yorigin = node->yorigin + ysize;
				right->xsize = node->xsize;
				right->ysize = node->ysize - ysize;
			}
			else
			{
				/* split horizontally */
				left->xorigin = node->xorigin;
				left->yorigin = node->yorigin;
				left->xsize = xsize;
				left->ysize = node->ysize;
				right->xorigin = node->xorigin + xsize;
				right->yorigin = node->yorigin;
				right->xsize = node->xsize - xsize;
				right->ysize = node->ysize;
			}
			node->left = left;
			node->right = right;
			node->id = id;
			_S_CALL("_S_font_pack_texture",
			tmp = _S_font_pack_texture(node->left, id,
			                           xsize, ysize,
			                           xtexsize, ytexsize));
			return tmp;
		}
	}
}

Sfont *
S_font_load(const Schar *filename,
            float pixel_size)
{
	Suint32 width, height, resizes;
	Sint16 c;
	_Sglyph *glyph, glyphs[S_GLYPH_NUM];
	_Stexture_node *root, *node;
	Suint32 xglyph_size, yglyph_size, xtexsize, ytexsize;
	FT_Face face;
	Sfont *font;

	if (!filename || pixel_size <= 0.0f)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_font_load");
		return NULL;
	}

	if (FT_New_Face(font_library, filename, 0, &face) != 0)
		_S_error_freetype("Failed to create font face.");
	if (FT_Set_Char_Size(face, 0, 64 * pixel_size, 0, 0) != 0)
		_S_error_freetype("Failed to set font size.");

	font = (Sfont *) S_memory_new(sizeof(Sfont));
	//memset(font->glyphs, 0, S_GLYPH_NUM * sizeof(_Sglyph));

	root = (_Stexture_node *) S_memory_new(sizeof(_Stexture_node));
	root->left = NULL;
	root->right = NULL;
	root->id = -1;
	root->xorigin = 0;
	root->yorigin = 0;
	root->xsize = S_UINT32_MAX;
	root->ysize = S_UINT32_MAX;
	root->empty = S_TRUE;

	xtexsize = pixel_size;
	ytexsize = pixel_size;
	node = NULL;

	/* load glyph sizes and sort */
	for (c = S_GLYPH_BEGIN; c < S_GLYPH_END; ++c)
	{
		glyph = glyphs+c-S_GLYPH_BEGIN;
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			S_warning("Failed to load glyph '%c'.", c);
			glyph->fail = S_TRUE;
			continue;
		}
		glyph->id = c;
		glyph->xtexsize = face->glyph->bitmap.width;
		glyph->ytexsize = face->glyph->bitmap.rows;
		glyph->fail = S_FALSE;
	}
	qsort(glyphs, S_GLYPH_NUM, sizeof(_Sglyph), _S_font_comparator);

	width = height = 0;
	/* pre-pack glyph positions */
	for (c = S_GLYPH_BEGIN; c < S_GLYPH_END; ++c)
	{
		glyph = glyphs+c-S_GLYPH_BEGIN;
		if (glyphs->fail)
			continue;
		xglyph_size = glyph->xtexsize + 1;
		yglyph_size = glyph->ytexsize + 1;
		resizes = 0;
		while (resizes * 2 < pixel_size)
		{
			if (xglyph_size == 0 || yglyph_size == 0)
				break;
			_S_CALL("_S_font_pack_texture",
			node = _S_font_pack_texture(root, glyph->id,
			                            xglyph_size, yglyph_size,
			                            xtexsize, ytexsize));
			if (node)
				break;
			xtexsize = xtexsize + (pixel_size/2);
			ytexsize = ytexsize + (pixel_size/2);
			++resizes;
		}
		if (node)
		{
			width = S_imax(width, node->xorigin + node->xsize);
			height = S_imax(height, node->yorigin + node->ysize);
		}
	}

	font->width = width;
	font->height = height;
	_S_CALL("S_vec3_fill", S_vec3_fill(&font->color, 1.0f));
	font->camera = NULL;

	/* generate vbo and vao */
	_S_GL(glGenVertexArrays(1, &font->vao));
	_S_GL(glGenBuffers(1, &font->vbo));

	/* generate the texture atlas */
	_S_GL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	_S_GL(glGenTextures(1, &font->texture));
	_S_GL(glBindTexture(GL_TEXTURE_2D, font->texture));
	_S_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	_S_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	_S_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	_S_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	_S_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height,
	                   0, GL_RED, GL_UNSIGNED_BYTE, 0));

	/* fill the atlas with glyphs */
	for (c = S_GLYPH_BEGIN; c < S_GLYPH_END; ++c)
	{
		glyph = glyphs+c-S_GLYPH_BEGIN;
		if (glyph->fail)
			continue;
		if (FT_Load_Char(face, glyph->id, FT_LOAD_RENDER) != 0)
		{
			S_warning("Failed to load glyph '%c'.", glyph->id);
			_S_CALL("S_vec2_zero", S_vec2_zero(&glyph->size));
			glyph->xbearing = 0;
			glyph->ybearing = 0;
			glyph->xadvance = 0;
			glyph->yadvance = 0;
			glyph->fail = 1;
			font->glyphs[glyph->id-S_GLYPH_BEGIN] = *glyph;
			continue;
		}

		_S_CALL("S_vec2_set", S_vec2_set(&glyph->size,
		                                 face->glyph->bitmap.width,
		                                 face->glyph->bitmap.rows));
		glyph->xbearing = face->glyph->bitmap_left;
		glyph->ybearing = face->glyph->bitmap_top;
		glyph->xadvance = face->glyph->advance.x >> 6;
		glyph->yadvance = face->glyph->advance.y >> 6;
		glyph->fail = 0;

		_S_CALL("_S_font_find_glyph",
		        node = _S_font_find_glyph(root, glyph->id));
		if (!node)
		{
			S_warning("Expected glyph '%c' but was not found.", glyph->id);
			continue;
		}
		S_vec2_set(&glyph->offset,
		           node->xorigin / (Sfloat) width,
		           node->yorigin / (Sfloat) height);
		_S_GL(
		glTexSubImage2D(GL_TEXTURE_2D, 0, node->xorigin, node->yorigin,
		                glyph->xtexsize, glyph->ytexsize,
		                GL_RED, GL_UNSIGNED_BYTE,
		                face->glyph->bitmap.buffer)
		);
		font->glyphs[glyph->id-S_GLYPH_BEGIN] = *glyph;
	}

	_S_CALL("_S_font_destroy_packing_tree", _S_font_destroy_packing_tree(root));

	/* quick and dirty texture extraction */
	/*
	_S_GL(glBindTexture(GL_TEXTURE_2D, font->texture));
	GLubyte *data = (GLubyte *) S_memory_new(sizeof(GLubyte) * width*height*4);
	_S_GL(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	stbi_write_png("texatlas.png", width, height, 4, data, width*4);
	*/

	FT_Done_Face(face);

	_S_GL(glBindVertexArray(font->vao));
	_S_GL(glBindBuffer(GL_ARRAY_BUFFER, font->vbo));
	_S_GL(glBufferData(GL_ARRAY_BUFFER,
	                   S_GLYPH_BUFFER_SIZE * 6 * 4 * sizeof(Sfloat),
	                   NULL, GL_DYNAMIC_DRAW));
	_S_GL(glEnableVertexAttribArray(0));
	_S_GL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
	                            4 * sizeof(Sfloat), 0));
	_S_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	_S_GL(glBindVertexArray(0));
	_S_GL(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));

	return font;
}

void
S_font_delete(Sfont *font)
{
	if (!font)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_font_delete");
		return;
	}
	_S_GL(glDeleteTextures(1, &font->texture));
	_S_GL(glDeleteVertexArrays(1, &font->vao));
	_S_GL(glDeleteBuffers(1, &font->vbo));
	S_memory_delete(font);
}

void
S_font_draw_text(const Sfont *font,
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

	if (!font || !text || scale <= 0.0f)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_font_draw_text");
		return;
	}

	if (len == 0 || !font->camera)
		return;

	_S_CALL("_S_shader_attach", _S_shader_attach(font_shader));
	_S_CALL("S_shader_set_uniform_vec3",
	        S_shader_set_uniform_vec3(font_shader, "u_color", &font->color));
	/* TODO: Dirty check the camera. */
	_S_CALL("S_camera_get_orthographic_matrix",
	        S_camera_get_orthographic_matrix(font->camera, &projection));
	_S_CALL("S_shader_set_uniform_mat4",
	        S_shader_set_uniform_mat4(font_shader, "u_projection",
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
S_font_set_camera(Sfont *font,
                  const Scamera *camera)
{
	if (!font || !camera)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_font_set_camera");
		return;
	}
	font->camera = camera;
}

void
S_font_set_color(Sfont *font,
                 Sfloat r,
                 Sfloat g,
                 Sfloat b)
{
	if (!font)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_font_set_color");
		return;
	}
	r = S_clamp(r, 0.0f, 1.0f);
	g = S_clamp(g, 0.0f, 1.0f);
	b = S_clamp(b, 0.0f, 1.0f);
	_S_CALL("S_vec3_set", S_vec3_set(&font->color, r, g, b));
}

