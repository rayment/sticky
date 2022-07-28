/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * draw.h
 * Common drawing header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 05/03/2022
 */

#ifndef FR_RAYMENT_STICKY_DRAW_H
#define FR_RAYMENT_STICKY_DRAW_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/types.h"
#include "sticky/math/transform.h"
#include "sticky/math/vec3.h"
#include "sticky/math/vec4.h"
#include "sticky/video/camera.h"
#include "sticky/video/mesh.h"
#include "sticky/video/shader.h"
#include "sticky/video/texture.h"

/**
 * @addtogroup draw
 * @{
 */

/**
 * @brief Draw a line in 3D space.
 *
 * Draws a line to a given window in 3D space from a given point to another.
 *
 * The draw colour can be modified by calling the
 * {@link S_draw_set_color(Swindow *, float, float, float, float)} function.
 *
 * @param[in] window The window to draw to.
 * @param[in] from The first point of the line.
 * @param[in] to The second point of the line.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window or 3D vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void S_draw_draw_line_3d(const Swindow *,
                                    const Svec3 *, const Svec3 *);

/**
 * @brief Draw a point in 3D space.
 *
 * Draws a point to a given window in 3D space.
 *
 * The draw colour can be modified by calling the
 * {@link S_draw_set_color(Swindow *, float, float, float, float)} function.
 *
 * @param[in] window The window to draw to.
 * @param[in] point The location of the point in 3D space.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window or 3D vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void S_draw_draw_point_3d(const Swindow *, const Svec3 *);

/**
 * @brief Draw a quad in 2D space.
 *
 * Draws a quad of two triangles to a given window in 2D space. Note that the 2D
 * space is defined by the camera currently attached to the window.
 *
 * The draw colour can be modified by calling the
 * {@link S_draw_set_color(Swindow *, float, float, float, float)} function.
 *
 * The draw texture can be modified by calling the
 * {@link S_draw_set_texture(Swindow *, const Stexture *)} function.
 *
 * @param[in] window The window to draw to.
 * @param[in] from The location of one point of the quad on the screen.
 * @param[in] to The location diagonal to @p from of the quad on the screen.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window or 2D vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void S_draw_draw_quad_2d(const Swindow *,
                                    const Svec2 *, const Svec2 *);

/**
 * @brief Set the colour for future window drawing.
 *
 * Sets the colour that shall be used to draw polygons. This function will not
 * replace the colour of previously drawn polygons. Thanks to this behaviour,
 * this function may be called many times during one frame for various colour
 * outputs.
 *
 * Colour values are between @f$0@f$ and @f$1@f$.
 *
 * @param[in,out] window The window.
 * @param[in] r The red colour component.
 * @param[in] g The green colour component.
 * @param[in] b The blue colour component.
 * @param[in] a The alpha colour component.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void S_draw_set_color(Swindow *, float, float, float, float);

/**
 * @brief Set the texture for future window drawing.
 *
 * Sets the texture that shall be uesd to draw polygons. This function will not
 * replace the texture of previously drawn polygons. Thanks to this behaviour,
 * this function may be called many times during one frame for various texture
 * outputs.
 *
 * If a <c>NULL</c> texture is provided, then no texture will be used when
 * drawing.
 *
 * Note that textures are only used for drawing shapes, not lines or points.
 *
 * @param[in,out] window The window.
 * @param[in] tex The texture to draw with.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid window is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void S_draw_set_texture(Swindow *, const Stexture *);

void _S_draw_init(Suint8, Suint8);
void _S_draw_free(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_DRAW_H */

