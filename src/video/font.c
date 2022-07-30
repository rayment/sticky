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
#include "sticky/video/font.h"

/*#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>*/

typedef struct _Stexture_node_s
{
	Schar id;
	Suint32 xorigin, yorigin, xsize, ysize;
	Sbool empty;
	struct _Stexture_node_s *left, *right;
} _Stexture_node;

static FT_Library font_library;
static Sbool init;

void
_S_font_init(Suint8 gl_maj,
             Suint8 gl_min)
{
	Sint32 i;
	if (gl_maj < 3 || (gl_maj == 3 && gl_min < 3))
	{
		/* cannot generate the font shader on old GL versions */
		return;
	}
	init = S_TRUE;
	if ((i = FT_Init_FreeType(&font_library)) != 0)
		_S_error_freetype("Failed to init FreeType.", i);
}

void
_S_font_free(void)
{
	if (init)
		FT_Done_FreeType(font_library);
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
            Sfloat pixel_size)
{
	Suint32 width, height, resizes;
	Sint16 c;
	_Sglyph *glyph, glyphs[S_GLYPH_NUM];
	_Stexture_node *root, *node;
	Suint32 xglyph_size, yglyph_size, xtexsize, ytexsize;
	FT_Face face;
	Sfont *font;
	Sint32 i;

	if (!filename || pixel_size <= 0.0f)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_font_load");
		return NULL;
	}

	if ((i = FT_New_Face(font_library, filename, 0, &face)) != 0)
		_S_error_freetype("Failed to create font face.", i);
	if ((i = FT_Set_Char_Size(face, 0, 64 * pixel_size, 0, 0)) != 0)
		_S_error_freetype("Failed to set font size.", i);

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
	                            4 * sizeof(Sfloat), (void *) 0));
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
S_font_get_extents(const Sfont *font,
                   const Schar *text,
				   Ssize_t len,
                   Sfloat scale,
				   Sfloat *xoff,
				   Sfloat *yoff,
				   Sfloat *w,
				   Sfloat *h)
{
	Schar c;
	Ssize_t i;
	const _Sglyph *glyph;
	Sfloat x, y, xp, yp, gw, gh, tw, th, ox, oy;
	Sint32 chars;

	if (!font || !text || scale <= 0.0f)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_font_get_extents");
		return;
	}

	chars = 0;
	x = y = xp = yp = gw = gh = tw = th = 0.0f;
	ox = oy = 99999.0f; /* big number so S_min works */
	for (i = 0; i < len; ++i)
	{
		c = *(text+i);
		glyph = font->glyphs+c-S_GLYPH_BEGIN;

		gw = glyph->size.x * scale;
		gh = S_max(glyph->size.y * scale, gh);

		xp = x + glyph->xbearing * scale;
		yp = y - (glyph->size.y - glyph->ybearing) * scale;

		x += glyph->xadvance * scale;
		y += glyph->yadvance * scale;

		if (gw == 0 || gh == 0)
			continue; /* skip empty glyphs */

		ox = S_min(xp, ox);
		oy = S_min(yp, oy);
		tw = S_abs(xp) + S_abs(gw);
		th = S_abs(oy) + S_abs(gh);

		++chars;
		if (chars >= S_GLYPH_BUFFER_SIZE)
			break;
	}
	if (xoff)
		*xoff = ox;
	if (yoff)
		*yoff = oy;
	if (w)
		*w = tw;
	if (h)
		*h = th;
}

