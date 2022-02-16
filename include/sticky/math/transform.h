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
 * @file transform.h
 * @addtogroup transform
 * @{
 */

typedef struct
Stransform_s
{
	Svec3 pos, scale;
	Squat rot;
	struct Stransform_s *parent;
	Slinkedlist *children;
} Stransform;

Stransform *S_transform_new(void);
void        S_transform_delete(Stransform *);

void        S_transform_set_parent(Stransform *, Stransform *);
Stransform *S_transform_get_parent(const Stransform *);

Ssize_t     S_transform_get_number_children(const Stransform *);
Stransform *S_transform_get_child(const Stransform *, Ssize_t);
Sbool       S_transform_is_child_in_tree(const Stransform *,
                                         const Stransform *);

void        S_transform_set_pos(Stransform *, const Svec3 *);
void        S_transform_add_pos(Stransform *, const Svec3 *);
void        S_transform_get_pos(const Stransform *, Svec3 *);
void        S_transform_set_rot(Stransform *, const Squat *);
void        S_transform_add_rot(Stransform *, const Squat *);
void        S_transform_get_rot(const Stransform *, Squat *);
void        S_transform_set_scale(Stransform *, const Svec3 *);
void        S_transform_add_scale(Stransform *, const Svec3 *);
void        S_transform_get_scale(const Stransform *, Svec3 *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_TRANSFORM_H */

