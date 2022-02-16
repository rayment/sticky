/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * transform.c
 * Object transform source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/02/2022
 */

#include "sticky/common/error.h"
#include "sticky/math/transform.h"
#include "sticky/math/quat.h"
#include "sticky/math/vec3.h"
#include "sticky/memory/allocator.h"

Stransform *
S_transform_new(void)
{
	Stransform *transform;
	transform = (Stransform *) S_memory_new(sizeof(Stransform));
	_S_CALL("S_vec3_zero", S_vec3_zero(&(transform->pos)));
	_S_CALL("S_vec3_fill", S_vec3_fill(&(transform->scale), 1.0f));
	_S_CALL("S_quat_identity", S_quat_identity(&(transform->rot)));
	transform->parent = NULL;
	_S_CALL("S_linkedlist_new", transform->children = S_linkedlist_new());
	return transform;
}

void
S_transform_delete(Stransform *transform)
{
	if (!transform)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_delete");
		return;
	}
	_S_CALL("S_linkedlist_delete", S_linkedlist_delete(transform->children));
	S_memory_delete(transform);
}

static
Sbool
_S_transform_in_hierarchy(Stransform *root,
                          Stransform *transform,
                          Stransform **rootchild)
{
	Stransform *current;

	if (!root || !transform)
		return S_FALSE;

	current = transform;
	while (current)
	{
		if (current->parent == root)
		{
			if (rootchild)
				*rootchild = current;
			return S_TRUE;
		}
		current = current->parent;
	}
	return S_FALSE;
}

void
S_transform_set_parent(Stransform *transform,
                       Stransform *parent)
{
	Sbool b;
	Stransform *t;
	void *ptr;
	if (!transform || transform == parent)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_set_parent");
		return;
	}
	if (transform->parent == parent)
		return;
	/* unset old parent-child relation */
	if (transform->parent)
	{
		_S_CALL("S_linkedlist_remove_ptr",
		        b = S_linkedlist_remove_ptr(transform->parent->children,
		                                    transform));
		S_assert(b,
		         "S_transform_set_parent: parent does not contain transform.");
	}
	/* set new parent-child relation */
	if (parent)
	{
		/* search through the entire parent hierarchy to ensure that this
		   transform is not contained in it, and if so, remove it */
		_S_CALL("_S_transform_in_hierarchy",
		        b = _S_transform_in_hierarchy(transform, parent, &t));
		if (b)
		{
			_S_CALL("S_linkedlist_remove_ptr",
			        b = S_linkedlist_remove_ptr(transform->children, t));
			S_assert(b, "_S_transform_in_hierarchy: failed hierarchy search");
			t->parent = NULL;
		}
		_S_CALL("S_linkedlist_add_head",
		        ptr = S_linkedlist_add_head(parent->children, transform));
		S_assert(ptr == transform,
		         "S_transform_set_parent: failed to set child heirarchy.");
	}
	transform->parent = parent;
}

Stransform *
S_transform_get_parent(const Stransform *transform)
{
	if (!transform)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_get_parent");
		return NULL;
	}
	return transform->parent;
}

Ssize_t
S_transform_get_number_children(const Stransform *transform)
{
	Ssize_t len;
	if (!transform)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_get_number_children");
		return 0;
	}
	_S_CALL("S_linkedlist_size", len = S_linkedlist_size(transform->children));
	return len;
}

Sbool
S_transform_is_child_in_tree(const Stransform *parent,
                             const Stransform *child)
{
	Stransform *current;
	if (!parent || !child)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_is_child_in_tree");
		return S_FALSE;
	}
	if (child->parent == parent)
		return S_TRUE;
	current = child->parent;
	while (current)
	{
		if (current->parent == parent)
			return S_TRUE;
		current = current->parent;
	}
	return S_FALSE;
}

/* transform is not const because the underlying linked-list will update the
   iterator when a search is performed */
Stransform *
S_transform_get_child(const Stransform *transform,
                      Ssize_t idx)
{
	Ssize_t len;
	void *ptr;
	if (!transform)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_get_child");
		return NULL;
	}
	_S_CALL("S_linkedlist_size", len = S_linkedlist_size(transform->children));
	if (idx >= len)
	{
		_S_SET_ERROR(S_INVALID_INDEX, "S_transform_get_child");
		return NULL;
	}
	_S_CALL("S_linkedlist_get",
	        ptr = S_linkedlist_get(transform->children, idx));
	return (Stransform *) ptr;
}

void
S_transform_set_pos(Stransform *transform,
                    const Svec3 *pos)
{
	if (!transform || !pos)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_set_pos");
		return;
	}
	_S_CALL("S_vec3_copy", S_vec3_copy(&(transform->pos), pos));
}

void
S_transform_add_pos(Stransform *transform,
                    const Svec3 *pos)
{
	if (!transform || !pos)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_add_pos");
		return;
	}
	_S_CALL("S_vec3_add", S_vec3_add(&(transform->pos), pos));
}

void
S_transform_get_pos(const Stransform *transform,
                    Svec3 *pos)
{
	if (!transform || !pos)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_get_pos");
		return;
	}
	_S_CALL("S_vec3_copy", S_vec3_copy(pos, &(transform->pos)));
}

void
S_transform_set_rot(Stransform *transform,
                    const Squat *rot)
{
	if (!transform || !rot)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_set_rot");
		return;
	}
	_S_CALL("S_quat_copy", S_quat_copy(&(transform->rot), rot));
}

void
S_transform_add_rot(Stransform *transform,
                    const Squat *rot)
{
	if (!transform || !rot)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_add_rot");
		return;
	}
	_S_CALL("S_quat_multiply", S_quat_multiply(&(transform->rot), rot));
}

void
S_transform_get_rot(const Stransform *transform,
                    Squat *rot)
{
	if (!transform || !rot)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_get_rot");
		return;
	}
	_S_CALL("S_quat_copy", S_quat_copy(rot, &(transform->rot)));
}

void
S_transform_set_scale(Stransform *transform,
                      const Svec3 *scale)
{
	if (!transform || !scale)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_set_scale");
		return;
	}
	_S_CALL("S_vec3_copy", S_vec3_copy(&(transform->scale), scale));
}

void
S_transform_add_scale(Stransform *transform,
                      const Svec3 *scale)
{
	if (!transform || !scale)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_add_scale");
		return;
	}
	_S_CALL("S_vec3_add", S_vec3_add(&(transform->scale), scale));
}

void
S_transform_get_scale(const Stransform *transform,
                      Svec3 *scale)
{
	if (!transform || !scale)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_transform_get_scale");
		return;
	}
	_S_CALL("S_vec3_copy", S_vec3_copy(scale, &(transform->scale)));
}

