/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * texture.h
 * OpenGL texture header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 18/04/2021
 */

#ifndef FR_RAYMENT_STICKY_TEXTURE_H
#define FR_RAYMENT_STICKY_TEXTURE_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"

/**
 * @addtogroup texture
 * @{
 */

/**
 * @brief Linear filtering mode.
 * @hideinitializer
 *
 * The linear filtering mode makes use of interpolation to smoothly filter a
 * texture.
 *
 * @since 1.0.0
 */
#define S_TEXTURE_LINEAR  GL_LINEAR
/**
 * @brief Nearest-neighbour filtering mode.
 * @hideinitializer
 *
 * The nearest-neighbour filtering mode takes the nearest representation of a
 * given pixel leading to a more sharp and pixelated effect, but more accurate
 * textures.
 *
 * @since 1.0.0
 */
#define S_TEXTURE_NEAREST GL_NEAREST

/**
 * @brief Clamped wrapping mode.
 * @hideinitializer
 *
 * Clamp a texture and repeat the border pixels across a mesh or surface.
 *
 * @since 1.0.0
 */
#define S_TEXTURE_CLAMP   GL_CLAMP_TO_EDGE
/**
 * @brief Repeating wrapping mode.
 * @hideinitializer
 *
 * Repeat a texture across a mesh or surface.
 *
 * @since 1.0.0
 */
#define S_TEXTURE_REPEAT  GL_REPEAT

/**
 * @brief Texture for use with OpenGL rendering.
 *
 * Textures are images that have been loaded into the GPU memory to be rendered
 * through the use of shaders. Each texture contains many texels that are
 * rendered across a 2D or 3D mesh to give colour and construct a visual image.
 *
 * Cubemaps are a special case for textures, in that the texture has 6 sides,
 * which makes it useful for rendering on cubes, such as the skybox of a 3D
 * world.
 *
 * @since 1.0.0
 */
typedef struct
Stexture_s
{
	GLuint tex;
	Sbool cubemap;
	Senum filter, wrap;
} Stexture;

/**
 * @brief Load a 2D texture into memory.
 *
 * Loads a single 2D texture that can be used in shaders.
 *
 * @param[in] filename The filename of the image to load.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid filename is provided
 * to the function.
 * @exception S_INVALID_CHANNELS If a given texture does not have three or four
 * channels (RGB and RGBA respectively).
 * @exception S_INVALID_FORMAT If a given texture does not have an even number
 * of pixels in either width or height.
 * @since 1.0.0
 */
STICKY_API Stexture *S_texture_load(const Schar *);

/**
 * @brief Load a cubemap texture into memory.
 *
 * A cubemap is defined as a 6-sided texture and can be used for rendering
 * skyboxes or other cube-like meshes.
 *
 * @param[in] px_filename The filename of the image for the @f$+X@f$ face.
 * @param[in] nx_filename The filename of the image for the @f$-X@f$ face.
 * @param[in] py_filename The filename of the image for the @f$+Y@f$ face.
 * @param[in] ny_filename The filename of the image for the @f$-Y@f$ face.
 * @param[in] pz_filename The filename of the image for the @f$+Z@f$ face.
 * @param[in] nz_filename The filename of the image for the @f$-Z@f$ face.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid filename is provided
 * to the function.
 * @exception S_INVALID_CHANNELS If any given texture does not have three or
 * four channels (RGB and RGBA respectively).
 * @exception S_INVALID_FORMAT If any given texture does not have an even number
 * of pixels in either width or height.
 * @since 1.0.0
 */
STICKY_API Stexture *S_texture_load_cubemap(const Schar *, const Schar *,
                                            const Schar *, const Schar *,
                                            const Schar *, const Schar *);

/**
 * @brief Free a texture from memory.
 *
 * One this function is called for a given texture, that texture becomes invalid
 * and may not be used in any other texture, shader or rendering function.
 *
 * @param[in,out] texture The texture to free from memory.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid texture is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void      S_texture_delete(Stexture *);

/**
 * @brief Set the filter mode for a texture.
 *
 * Sets the mode in which the texture should be filtered. The
 * {@link S_TEXTURE_NEAREST} filter will use the nearest-neighbour filter which
 * gives a sharper and more pixelated look. {@link S_TEXTURE_LINEAR} will smooth
 * the image by using an interpolation algorithm at the cost of sharpness.
 *
 * @param[in,out] texture The texture to set the wrap mode for.
 * @param[in] filter The filter mode. Either one of {@link S_TEXTURE_LINEAR} or
 * {@link S_TEXTURE_NEAREST}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid texture is provided to
 * the function.
 * @exception S_INVALID_ENUM If an invalid filter mode not specified above is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void      S_texture_set_filter(Stexture *, Senum);

/**
 * @brief Set the wrapping mode for a texture.
 *
 * Sets the mode in which the texture should wrap around whichever mesh it is
 * applied to. Using the {@link S_TEXTURE_CLAMP} mode will force the texture to
 * occupy only as much space as necessary to draw the texture, and the rest of
 * the mesh will be a repetition of the edge pixels of the texture. Using the
 * {@link S_TEXTURE_REPEAT} mode will repeatedly tile the texture across the
 * mesh.
 *
 * @param[in,out] texture The texture to set the wrap mode for.
 * @param[in] wrap The wrapping mode. Either one of {@link S_TEXTURE_CLAMP} or
 * {@link S_TEXTURE_REPEAT}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid texture is provided to
 * the function.
 * @exception S_INVALID_ENUM If an invalid wrapping mode not specified above is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void      S_texture_set_wrap(Stexture *, Senum);

void _S_texture_init(void);
void _S_texture_attach_raw(GLuint, Suint32, Sbool);
void _S_texture_attach(const Stexture *, Suint32);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_TEXTURE_H */

