/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * font.h
 * FreeType font header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 19/04/2021
 */

#ifndef FR_RAYMENT_STICKY_FONT_H
#define FR_RAYMENT_STICKY_FONT_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/math/math.h"
#include "sticky/video/camera.h"
#include "sticky/video/shader.h"

/**
 * @addtogroup font
 * @{
 */

#define S_GLYPH_BEGIN       32  /* first glyph to load */
#define S_GLYPH_END         128 /* last glyph to load */
#define S_GLYPH_NUM (S_GLYPH_END - S_GLYPH_BEGIN)
#define S_GLYPH_BUFFER_SIZE 128 /* buffer size for draw call batching */

typedef struct _Sglyph_s
{
	Svec2 size, offset;
	Sint32 xbearing, ybearing, xadvance, yadvance;
	Suint32 xtexsize, ytexsize;
	Schar id;
	Sbool fail;
} _Sglyph;

/**
 * @brief Font data structure.
 *
 * A font defines a graphical type of a specific face and size. Fonts in
 * <b>sticky</b> are loaded using the FreeType library which is capable of
 * processing TrueType and PostScript fonts among others whereby the font is
 * rasterised into a compact GL texture to be displayed on the screen.
 *
 * @warning This module uses a built-in shader compiled for OpenGL 3.3 Core.
 * Ensure a compatible profile is used before attempting to call
 * {@link S_font_load(const Schar *, float)} or else all behaviour shall be
 * undefined.
 * @since 1.0.0
 */
typedef struct Sfont_s
{
	GLuint texture;
	Suint32 width, height;
	_Sglyph glyphs[S_GLYPH_NUM];
	GLuint vao, vbo;
} Sfont;

/**
 * @brief Load a font from file.
 *
 * Loads a font from a file path at a certain pixel-size and rasterises it into
 * a texture atlas ready for rendering. The higher the pixel-size, the higher
 * the quality of the resulting image, however it comes at the cost of a larger
 * texture and possible artifacts if rendering at small sizes.
 *
 * @param[in] filename The file path to the font to be loaded.
 * @param[in] pixel_size The size of the font for generation.
 * @return A pointer to a newly allocated and loaded font.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid file path is provided
 * to the function, or if @p pixel_size is less than or equal to @f$0@f$.
 * @since 1.0.0
 */
STICKY_API Sfont *S_font_load(const Schar *, Sfloat);

/**
 * @brief Free a font from memory.
 *
 * Once this function is called for a given font, that font becomes invalid and
 * may not be used again in any other font function.
 *
 * @param[in,out] font The font to delete.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid font is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void  S_font_delete(Sfont *);

void _S_font_init(Suint8, Suint8);
void _S_font_free(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_FONT_H */

