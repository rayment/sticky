/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * pencil.h
 * Primitive drawing header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 05/03/2022
 */

#ifndef FR_RAYMENT_STICKY_PENCIL_H
#define FR_RAYMENT_STICKY_PENCIL_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/types.h"
#include "sticky/math/transform.h"
#include "sticky/math/vec3.h"
#include "sticky/math/vec4.h"
#include "sticky/video/camera.h"
#include "sticky/video/shader.h"

/**
 * @addtogroup pencil
 * @{
 */

/**
 * @brief Primitive pencil struct.
 *
 * Primitive pencils allow for drawing lines and other primitive shapes on the
 * screen for debugging purposes.
 *
 * @warning This module uses a built-in shader compiled for OpenGL 3.3 Core.
 * Ensure a compatible profile is used before attempting to call
 * {@link S_pencil_new(void)} or else all behaviour shall be undefined.
 * @since 1.0.0
 */
typedef struct
Spencil_s
{
	Suint32 vbo, vao;
	Sshader *shader;
	Stransform *transform;
	const Scamera *camera;
} Spencil;

/**
 * @brief Create a new primitive pencil.
 *
 * Allocates a new pencil along with the required OpenGL shader, VAOs and VBOs
 * for drawing primitive shapes on the screen.
 *
 * Note that {@link S_pencil_set_camera(Spencil *, const Scamera *)} must be
 * called to provide a camera to the pencil so it knows the projection and view
 * matrices for rendering.
 *
 * @return A pointer to a newly allocated pencil struct.
 * @since 1.0.0
 */
Spencil *S_pencil_new(void);

/**
 * @brief Free a primitive pencil from memory.
 *
 * The OpenGL shader, VAOs and VBOs are also free'd when this function is
 * called.
 *
 * Once this function is called for a given pencil, that pencil becomes invalid
 * and may not be used again in any other draw function.
 *
 * @param[in,out] pencil The pencil to delete.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid pencil is provided to
 * the function.
 * @since 1.0.0
 */
void   S_pencil_delete(Spencil *);

/**
 * @brief Draw a line in 3D space.
 *
 * Draws a line for a given camera in 3D space from a given point to another.
 *
 * The draw colour can be modified by calling the
 * {@link S_pencil_set_camera(Spencil *, const Scamera *)} function.
 *
 * @param[in] pencil The pencil to draw with.
 * @param[in] from The first point of the line.
 * @param[in] to The second point of the line.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid pencil is provided to
 * the function.
 * @since 1.0.0
 */
void   S_pencil_draw_line(const Spencil *, const Svec3 *, const Svec3 *);

/**
 * @brief Set the camera of a pencil.
 *
 * A pencil requires a camera for drawing within 3D space as it uses the
 * projection and view matrices to determine where on the screen a shape should
 * be drawn.
 *
 * If a camera other than the camera that is currently being drawn to is passed
 * in as a parameter, then it is undefined behaviour.
 *
 * @param[in,out] pencil The pencil.
 * @param[in] camera The camera in which the lines should appear.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid pencil or camera is
 * provided to the function.
 * @since 1.0.0
 */
void   S_pencil_set_camera(Spencil *, const Scamera *);

/**
 * @brief Set the colour of a pencil.
 *
 * Sets the colour that a pencil should draw its shapes with. This function will
 * not replace the colour of previously drawn lines, as it only affects the
 * following draw calls. Because of this, this function may be called many times
 * during one frame for various colour outputs.
 *
 * @param[in,out] pencil The pencil.
 * @param[in] color The colour to draw with.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid pencil or 3D vector is
 * provided to the function.
 * @since 1.0.0
 */
void   S_pencil_set_color(Spencil *, const Svec3 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_PENCIL_H */

