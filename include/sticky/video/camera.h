/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * camera.h
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_CAMERA_H
#define FR_RAYMENT_STICKY_CAMERA_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/types.h"
#include "sticky/math/mat4.h"
#include "sticky/math/transform.h"
#include "sticky/video/window.h"

/**
 * @addtogroup camera
 * @{
 */

/**
 * @brief Display camera.
 *
 * The camera is the primary device through which the world-space is viewed.
 * By manipulating the position and rotation of the camera, the world may be
 * viewed in a unique perspective.
 *
 * @since 1.0.0
 */
typedef struct
Scamera_s
{
	Stransform *transform;
	Sfloat near_plane, far_plane, fov, aspect;
	Suint32 width, height;
	Smat4 perspective, orthographic;
	Swindow *win;
} Scamera;

/**
 * @brief Create a new camera.
 *
 * Allocates a block of memory on the heap for a camera and an internal
 * {@link Stransform} that defines the camera position and rotation. The default
 * near-plane of <c>1.0f</c>, far-plane of <c>100.0f</c> and field-of-view of
 * 60 degrees will be applied to the new camera.
 *
 * @param[in] width The width of the camera frame in pixels.
 * @param[in] height The height of the camera frame in pixels.
 * @return A new camera.
 * @since 1.0.0
 */
STICKY_API Scamera    *S_camera_new(Suint32, Suint32);

/**
 * @brief Free a camera from memory.
 *
 * Once this function is called for a given camera, that camera becomes invalid
 * and may not be used again in any other camera function.
 *
 * @param[in,out] camera The camera to delete.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void        S_camera_delete(Scamera *);

/**
 * @brief Hook a camera to a window for resize updates.
 *
 * Causes a given camera to attach itself to a window, thus automatically
 * changing its display dimensions to match that of the window whenever a
 * display update is invoked. Note that only one camera can be attached to any
 * one window at a time. Attaching a camera to a window that already has a
 * camera will replace the hook, not append to it.
 *
 * If a <c>NULL</c> window is provided, then the camera will be unattached.
 *
 * @param[in,out] camera The camera to attach to the window.
 * @param[in,out] window The window to attach to the camera.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void        S_camera_attach(Scamera *, Swindow *);

/**
 * @brief Set the near-plane of a camera.
 *
 * Sets the near-plane of a given camera. Try to keep the near-plane and
 * far-plane of the camera to a limited range to avoid Z-fighting when
 * rendering.
 *
 * @param[in,out] camera The camera.
 * @param[in] near_plane The near-plane to set for the camera.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void        S_camera_set_near_plane(Scamera *, Sfloat);

/**
 * @brief Set the far-plane of a camera.
 *
 * Sets the far-plane of a given camera. Try to keep the near-plane and
 * far-plane of the camera to a limited range to avoid Z-fighting when
 * rendering.
 *
 * @param[in,out] camera The camera.
 * @param[in] far_plane The near-plane to set for the camera.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void        S_camera_set_far_plane(Scamera *, Sfloat);

/**
 * @brief Set the field-of-view of a camera.
 *
 * Sets the field-of-view of a given camera. The @p fov parameter is measured in
 * degrees and applies to the <c>y</c> dimension.
 *
 * @param[in,out] camera The camera.
 * @param[in] fov The field-of-view to set for the camera.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void        S_camera_set_field_of_view(Scamera *, Sfloat);

/**
 * @brief Set the pixel size and aspect-ratio of a camera.
 *
 * Sets the width and height of the camera, and by extension the aspect-ratio of
 * a given camera.
 *
 * @param[in,out] camera The camera.
 * @param[in] width The width of the camera in pixels.
 * @param[in] height The height of the camera in pixels.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void        S_camera_set_size(Scamera *, Suint32, Suint32);

/**
 * @brief Get the near-plane of a camera.
 *
 * Returns the current near-plane of a given camera.
 *
 * @param[in] camera The camera.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sfloat      S_camera_get_near_plane(const Scamera *);

/**
 * @brief Get the far-plane of a camera.
 *
 * Returns the current far-plane of a given camera.
 *
 * @param[in] camera The camera.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sfloat      S_camera_get_far_plane(const Scamera *);

/**
 * @brief Get the field-of-view of a camera.
 *
 * Returns the current field-of-view of a given camera.
 *
 * @param[in] camera The camera.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sfloat      S_camera_get_field_of_view(const Scamera *);

/*
 * @brief Get the pixel size of a camera.
 *
 * Sets either of the two given pointers to the width and height respectively of
 * the camera so long as they are not equal to <c>NULL</c>.
 *
 * @param[in] camera The camera.
 * @param[out] width The width output of the camera in pixels.
 * @param[out] height The height output of the camera in pixels.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sfloat      S_camera_get_aspect_ratio(const Scamera *);

/**
 * @brief Get the aspect-ratio of a camera.
 *
 * Returns the current aspect-ratio of a given camera.
 *
 * @param[in] camera The camera.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sfloat      S_camera_get_aspect_ratio(const Scamera *);

/**
 * @brief Get the transform of a camera.
 *
 * Returns the transform of a given camera. Any movement or rotation
 * applications to the camera should be done via. this transform.
 *
 * @param[in] camera The camera.
 * @return The transform for a given camera.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Stransform *S_camera_get_transform(const Scamera *);

/**
 * @brief Get the perspective matrix of a camera.
 *
 * Returns the perspective matrix of a given camera used for 3D drawing.
 *
 * @param[in] camera The camera.
 * @param[out] dest The matrix to store the perspective projection in.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera or 4x4 matrix
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_camera_get_perspective_matrix(const Scamera *,
                                                       Smat4 *);

/**
 * @brief Get the orthographic matrix of a camera.
 *
 * Returns the orthographic matrix of a given camera used for 2D drawing.
 *
 * @param[in] camera The camera.
 * @param[out] dest The matrix to store the orthographic projection in.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera or 4x4 matrix
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_camera_get_orthographic_matrix(const Scamera *,
                                                        Smat4 *);

/**
 * @brief Get the view matrix of a camera.
 *
 * The view matrix is equivalent to the inverse of the transformation matrix.
 *
 * @param[in] camera The camera.
 * @param[out] dest The matrix to store the view matrix in.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera or 4x4 matrix
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_camera_get_view_matrix(const Scamera *, Smat4 *);

void       _S_camera_resize_hook(Scamera *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_CAMERA_H */

