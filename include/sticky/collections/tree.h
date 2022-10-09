/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * tree.h
 * Binary search tree header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 16/04/2022
 */

#ifndef FR_RAYMENT_STICKY_TREE_H
#define FR_RAYMENT_STICKY_TREE_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/types.h"

typedef struct
_Stree_node_s
{
	void *ptr;
	struct _Stree_node_s *parent, *left, *right;
	Sssize_t bal;
} _Stree_node;

typedef struct _Stree_node_s Stree_iter;

typedef Scomparator (*Stree_comparator)(const void *, const void *);

typedef struct
Stree_s
{
	struct _Stree_node_s *root, *min, *max;
	Ssize_t len;
	Stree_comparator comparator;
} Stree;

STICKY_API Stree      *S_tree_new(Stree_comparator);
STICKY_API void        S_tree_delete(Stree *);

STICKY_API void        S_tree_set_comparator(Stree *, Stree_comparator);

STICKY_API void        S_tree_clear(Stree *);

STICKY_API void       *S_tree_insert(Stree *, void *);
STICKY_API void       *S_tree_remove(Stree *, void *);

STICKY_API Stree_iter *S_tree_iter_begin(Stree *);
STICKY_API Stree_iter *S_tree_iter_end(Stree *);
STICKY_API void       *S_tree_iter_next(Stree_iter **);
STICKY_API void       *S_tree_iter_prev(Stree_iter **);
STICKY_API Sbool       S_tree_iter_hasnext(const Stree_iter *);
STICKY_API Sbool       S_tree_iter_hasprev(const Stree_iter *);

STICKY_API void       *S_tree_get_min(const Stree *t);
STICKY_API void       *S_tree_get_max(const Stree *t);

STICKY_API Sbool       S_tree_search(const Stree *, const void *);
STICKY_API Ssize_t     S_tree_size(const Stree *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_TREE_H */

