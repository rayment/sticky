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
	Svec3 color;
	const Scamera *camera;
} Sfont;

/**
 * @brief Load a font from file.
 *
 * Loads a font from a file path at a certain pixel-size and rasterises it into
 * a texture atlas ready for rendering. The higher the pixel-size, the higher
 * the quality of the resulting image, however it comes at the cost of a larger
 * texture and possible artifacts if rendering at small sizes.
 *
 * Note that {@link S_font_set_camera(Sfont *, const Scamera *)} must be called
 * to provide a camera to the font so it knows the projection matrix for
 * rendering.
 *
 * @param[in] filename The file path to the font to be loaded.
 * @param[in] pixel_size The size of the font for generation.
 * @return A pointer to a newly allocated and loaded font.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid file path is provided
 * to the function, or if @p pixel_size is less than or equal to @f$0@f$.
 * @since 1.0.0
 */
Sfont *S_font_load(const Schar *, float);

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
void  S_font_delete(Sfont *);

/**
 * @brief Draws text to the screen.
 *
 * Using the generated texture atlas, one quad (two triangles) per character of
 * a given font will be drawn to the screen at a given position and at a given
 * scale.
 *
 * @param[in] font The font to draw to the screen.
 * @param[in] text The text to be drawn.
 * @param[in] len The number of chars in @p text to be drawn.
 * @param[in] x The @f$x@f$ position on the screen at which to draw the text.
 * @param[in] y The @f$y@f$ position on the screen at which to draw the text.
 * @param[in] scale The scale of the drawn text.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid font or string is
 * provided to the function.
 * @since 1.0.0
 */
void  S_font_draw_text(const Sfont *, const Schar *, Ssize_t,
                       float, float, float);

/**
 * @brief Set the camera of a font.
 *
 * A font requires a camera for drawing within 2D space as it uses the
 * orthgraphic projection to determine where on the screen text should be drawn.
 *
 * If a camera other than the camera that is currently being drawn to is passed
 * in as a parameter, then it is undefined behaviour.
 *
 * @param[in,out] font The font.
 * @param[in] camera The camera in which any drawn text should appear.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid font or camera is
 * provided to the function.
 * @since 1.0.0
 */
void  S_font_set_camera(Sfont *, const Scamera *);

/**
 * @brief Set the colour of a font.
 *
 * Sets the colour that a font should draw at. This can be called multiple times
 * in a draw function to change the colour for different blocks of text.
 *
 * Colour values are between @f$0@f$ and @f$1@f$.
 *
 * @param[in,out] font The font to set the colour for.
 * @param[in] r The red colour component.
 * @param[in] g The green colour component.
 * @param[in] b The blue colour component.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid font is provided to
 * the function.
 * @since 1.0.0
 */
void  S_font_set_color(Sfont *, float, float, float);

void _S_font_init(Suint8, Suint8);
void _S_font_free(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_FONT_H */

