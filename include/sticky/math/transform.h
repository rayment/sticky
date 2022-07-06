/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * transform.h
 * Object transform header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/02/2022
 */

#ifndef FR_RAYMENT_STICKY_TRANSFORM_H
#define FR_RAYMENT_STICKY_TRANSFORM_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/collections/linkedlist.h"
#include "sticky/common/types.h"
#include "sticky/math/math.h"

/**
 * @addtogroup transform
 * @{
 */

/**
 * @brief Object transform.
 *
 * Object transforms are attached to every object, sound, light and camera that
 * may appear within a 2D or 3D scene. Transforms have a position, rotation and
 * scale.
 *
 * Transforms may also have parent and child transforms which creates an
 * ancestory graph. The top-most transform is said to be in world-space, that
 * is, the position, rotation and scale of the transform is relative to the
 * world origin. All descendants of a top-most transform are relative to their
 * parent origin, and <i>not</i> the world origin.
 *
 * @since 1.0.0
 */
typedef struct
Stransform_s
{
	Svec3 pos, scale;
	Squat rot;
	struct Stransform_s *parent;
	Slinkedlist *children;
} Stransform;

/**
 * @brief Create a new object transform.
 *
 * Creates a new blank transform (position is set to zero, rotation is set to
 * zero and scale is set to 1, no parent, no children) by allocating memory on
 * the heap, and returning a pointer.
 *
 * @return A new object transform allocated on the heap. To correctly destroy
 * the transform, call {@link S_transform_delete(Stransform *)}.
 * @since 1.0.0
 */
STICKY_API Stransform *S_transform_new(void);

/**
 * @brief Free an object transform from memory.
 *
 * When this function is called, the parent of the transform will remove the
 * transform from its list of children. Any children of the transform will be
 * inherited by the parent of the transform. If the transform has no parent,
 * the children will also have no parent.
 *
 * @param[in,out] transform The transform to free from memory.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_delete(Stransform *);

/**
 * @brief Create a parent-child relationship between two transforms.
 *
 * When this function is called, two given transforms will be linked together in
 * a parent-child relationship. That is, the first transform @p transform will
 * be set as a child of @p parent, and @p parent will be set as the parent of @p
 * transform.
 *
 * <c>NULL</c> is an acceptable value for @p parent, in which case the transform
 * will become parentless.
 *
 * If this function is called on a transform that already has a parent, the
 * previous parent-child relationship will be severed.
 *
 * If this function is called for a given transform and parent where the parent
 * already exists in the ancestor hierarchy for @p transform, then the direct
 * descendant of @p transform from which @p parent originates will become the
 * new root node of the hierarchy and cease to be a child of @p transform to
 * avoid cyclic relations.
 *
 * Note that changing a parent will not preserve the position, rotation or scale
 * relative to world space of the transform and its descendants. The transform
 * will keep its local position, rotation and scale as set by the user via. the
 * appropriate functions.
 *
 * @param[in,out] transform The child transform.
 * @param[in,out] parent The parent transform.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_set_parent(Stransform *, Stransform *);

/**
 * @brief Get the parent of a transform.
 *
 * If a given transform has no parent, then <c>NULL</c> will be returned.
 *
 * @param[in] transform The transform from which to get the parent.
 * @return The parent of a given transform.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API Stransform *S_transform_get_parent(const Stransform *);

/**
 * @brief Get the number of direct children a transform contains.
 *
 * <i>Direct</i> children refer to the direct descendants of a given transform,
 * set with {@link S_transform_set_parent(Stransform *, Stransform *)} and does
 * not count the entire ancestory tree.
 *
 * @param[in] transform The transform from which to get the number of children.
 * @return The number of children of a given transform.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API Ssize_t     S_transform_get_number_children(const Stransform *);

/**
 * @brief Get a child of a transform by index.
 *
 * To get the number of children contained in a given transform, use the
 * function {@link S_transform_get_number_children(const Stransform *)}.
 *
 * @param[in] transform The transform from which to get the child.
 * @param[in] idx The index of the child.
 * @return The child of a given transform denoted by the index @p idx.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform is provided
 * to the function.
 * @exception S_INVALID_INDEX If an index greater than or equal to the number of
 * children of the transform is provided to the function.
 * @since 1.0.0
 */
STICKY_API Stransform *S_transform_get_child(const Stransform *, Ssize_t);

/**
 * @brief Determine whether or not a child is a descendant of a transform.
 *
 * If a child is a descendant, that is, a direct child or a child of a child,
 * etc. then this function will return {@link S_TRUE}.
 *
 * @param[in] parent The transform from which to conduct the search.
 * @param[in] child The transform to find in the ancestory tree of @p parent.
 * @return {@link S_TRUE} if @p child is a descendant of @p parent, otherwise
 * {@link S_FALSE}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API Sbool       S_transform_is_child_in_tree(const Stransform *,
                                                    const Stransform *);

/**
 * @brief Set the local position of a transform.
 *
 * When the world position of a transform is determined, the position, rotation
 * and scale of all parent (ancestor) transforms is taken into account. This
 * function will only set the <i>local</i> position of a given transform, using
 * the parents transform as the origin.
 *
 * If no parent is set, then the local position is of course equivalent to the
 * world position of the transform.
 *
 * @param[in,out] transform The transform whose position shall be set.
 * @param[in] pos The position that the transform should be moved to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_set_pos(Stransform *, const Svec3 *);

/**
 * @brief Add to the local position of a transform.
 *
 * @param[in,out] transform The transform whose position shall be added to.
 * @param[in] pos The position delta that the transform should be moved by.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_add_pos(Stransform *, const Svec3 *);

/**
 * @brief Get the local position of a transform.
 *
 * @param[in] transform The transform from which to get the position.
 * @param[out] pos The vector to store the position in.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_get_pos(const Stransform *, Svec3 *);

/**
 * @brief Set the local rotation of a transform.
 *
 * @param[in,out] transform The transform whose rotation shall be set.
 * @param[in] rot The rotation that the transform should be rotated to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or
 * quaternion is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_set_rot(Stransform *, const Squat *);

/**
 * @brief Add to the local rotation of a transform.
 *
 * @param[in,out] transform The transform whose rotation shall be added to.
 * @param[in] rot The rotation delta that the transform should be rotated by.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or
 * quaternion is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_add_rot(Stransform *, const Squat *);

/**
 * @brief Get the local rotation of a transform.
 *
 * @param[in] transform The transform from which to get the rotation.
 * @param[out] rot The quaternion to store the rotation in.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or
 * quaternion is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_get_rot(const Stransform *, Squat *);

/**
 * @brief Set the local scale of a transform.
 *
 * When the world scale of a transform is determined, the position, rotation
 * and scale of all parent (ancestor) transforms is taken into account. This
 * function will only set the <i>local</i> scale of a given transform, using
 * the parents transform as the origin.
 *
 * If no parent is set, then the local scale is of course equivalent to the
 * world scale of the transform.
 *
 * @param[in,out] transform The transform whose scale shall be set.
 * @param[in] scale The scale that the transform should be set to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_set_scale(Stransform *, const Svec3 *);

/**
 * @brief Add to the local scale of a transform.
 *
 * @param[in,out] transform The transform whose scale shall be added to.
 * @param[in] scale The scale delta that the transform should be scaled by.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_add_scale(Stransform *, const Svec3 *);

/**
 * @brief Get the local scale of a transform.
 *
 * @param[in] transform The transform from which to get the scale.
 * @param[out] scale The vector to store the scale in.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_get_scale(const Stransform *, Svec3 *);

/**
 * @brief Get the local transformation matrix of a transform.
 *
 * @param[in] transform The transform from which to get the transformation
 * matrix.
 * @param[out] dest The matrix to store the local transform in.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or matrix
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_get_transformation_matrix(const Stransform *,
                                                             Smat4 *);

/**
 * @brief Get the forward vector of a transform.
 *
 * The forward vector is the vector that points in the direction that the
 * transform is facing in local space.
 *
 * @param[in] transform The transform from which to get the vector.
 * @param[out] dest The destination for the forward vector.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_get_forward(const Stransform *, Svec3 *);

/**
 * @brief Get the right vector of a transform.
 *
 * The right vector is the vector that points to the right of the direction
 * that the transform is facing in local space.
 *
 * @param[in] transform The transform from which to get the vector.
 * @param[out] dest The destination for the right vector.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_get_right(const Stransform *, Svec3 *);

/**
 * @brief Get the upward vector of a transform.
 *
 * The upward vector is the vector that points up from the transform orientated
 * in the direction that it is facing in local space.
 *
 * @param[in] transform The transform from which to get the vector.
 * @param[out] dest The destination for the upward vector.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid transform or vector
 * is provided to the function.
 * @since 1.0.0
 */
STICKY_API void        S_transform_get_up(const Stransform *, Svec3 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_TRANSFORM_H */

