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

#include "sticky/math/transform.h"

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
	Sfloat near, far, fov;
} Scamera;

/**
 * @brief Create a new camera.
 *
 * Allocates a block of memory on the heap for a camera and an internal
 * {@link Stransform} that defines the camera position and rotation. The default
 * near-plane of <c>1.0f</c>, far-plane of <c>100.0f</c> and field-of-view of
 * 60 degrees will be applied to the new camera.
 *
 * @since 1.0.0
 */
Scamera    *S_camera_new(void);

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
void        S_camera_delete(Scamera *);

/**
 * @brief Set the near-plane of a camera.
 *
 * Sets the near-plane of a given camera. Try to keep the near-plane and
 * far-plane of the camera to a limited range to avoid Z-fighting when
 * rendering.
 *
 * @param[in,out] camera The camera.
 * @param[in] near The near-plane to set for the camera.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
void        S_camera_set_near_plane(Scamera *, Sfloat);

/**
 * @brief Set the far-plane of a camera.
 *
 * Sets the far-plane of a given camera. Try to keep the near-plane and
 * far-plane of the camera to a limited range to avoid Z-fighting when
 * rendering.
 *
 * @param[in,out] camera The camera.
 * @param[in] far The near-plane to set for the camera.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid camera is provided to
 * the function.
 * @since 1.0.0
 */
void        S_camera_set_far_plane(Scamera *, Sfloat);

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
void        S_camera_set_field_of_view(Scamera *, Sfloat);

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
Sfloat      S_camera_get_near_plane(const Scamera *);

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
Sfloat      S_camera_get_far_plane(const Scamera *);

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
Sfloat      S_camera_get_field_of_view(const Scamera *);

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
Stransform *S_camera_get_transform(const Scamera *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_CAMERA_H */

