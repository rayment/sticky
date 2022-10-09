/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * tree.h
 * Self-balancing binary tree header.
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

/**
 * @addtogroup tree
 * @{
 */

typedef struct
_Stree_node_s
{
	void *ptr;
	struct _Stree_node_s *parent, *left, *right;
	Sssize_t bal;
} _Stree_node;

/**
 * Tree comparator type.
 *
 * The tree comparator is a function pointer that points to a function which
 * performs a comparison on two pieces of data, and returns a value based on the
 * equality of the two pieces of data.
 *
 * Given two function parameters <c>a</c> and <c>b</c>:
 * <ul>
 * <li>If @f$a<b@f$, then return @f$-1@f$.</li>
 * <li>If @f$a>b@f$, then return @f$1@f$.</li>
 * <li>If @f$a=b@f$, then return @f$0@f$.</li>
 * </ul>
 *
 * This comparator is internally used to correctly order the tree and allow
 * iterations to occur. If the function is incorrectly defined, then the list
 * cannot be guaranteed to store data coherently.
 *
 * It is also important to note that tree insertion operations will fail if
 * @f$0@f$ is returned from this function when comparing a new piece of data
 * with any other element already in the tree, because that would imply that
 * they are equal, and a tree cannot store multiple copies of the same piece of
 * data, despite the fact that the pointers may point to different locations in
 * memory.
 *
 * @since 1.0.0
 */
typedef Scomparator (*Stree_comparator)(const void *, const void *);

/**
 * Self-balancing binary tree struct.
 *
 * Binary trees store information such that each internal node has up to two
 * children, where the left child only contains data with a value
 * computationally less than that of the parent, and the right child only
 * contains data with a value computationally greater than that of the parent.
 * The time complexity of tree operations is directly proportional to the tree's
 * height.
 *
 * The self-balancing aspect of the tree ensures that the height of the tree is
 * always at a minimal, and each insertion and removal into and from the tree
 * will automatically force a rebalance of the tree if required.
 *
 * Trees can only store unique values, and as such, two or more data pointers
 * that have equivalent values as per the chosen comparator cannot be stored in
 * the same tree at the same time.
 *
 * @warning Trees are not thread safe. To ensure synchronisation across threads,
 * {@link Smutex} must be used to synchronise interactions with the tree.
 *
 * @since 1.0.0
 */
typedef struct
Stree_s
{
	struct _Stree_node_s *root, *min, *max;
	Ssize_t len;
	Stree_comparator comparator;
} Stree;

/**
 * Tree iterator type.
 *
 * The tree iterator is a struct that is allocated on the stack to iterate over
 * a tree in either the forwards or backwards direction.
 *
 * The typedef <c>_Stree_node_s</c> is internal and should not be accessed by
 * the user.
 *
 * @since 1.0.0
 */
typedef struct _Stree_node_s Stree_iter;

/**
 * @brief Create a new tree.
 *
 * Allocates a new tree to the heap and returns a pointer. This tree will have
 * no elements. The @p comparator argument defines which comparison function to
 * use for element ordering.
 *
 * @param[in] comparator The comparison function to use for element ordering.
 * @return A new tree allocated on the heap with no elements. To correctly
 * destroy the tree, call {@link S_tree_delete(Stree *)}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid comparator is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API Stree      *S_tree_new(Stree_comparator);

/**
 * @brief Free a tree from memory.
 *
 * Takes a given tree, clears all elements from it (note that this does not
 * deallocate the elements - that must be done separately), and frees the tree
 * from memory.
 *
 * Once this function is called for a given tree, that tree becomes invalid and
 * may not be used again in any other function.
 *
 * @param[in,out] t The tree to free from memory.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid tree is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void        S_tree_delete(Stree *);

/**
 * @brief Add an element to a tree.
 *
 * Adds an element to a tree and automatically balances the tree if required to
 * ensure that it remains optimised.
 *
 * If an element of equivalent value is already in the tree, then <c>NULL</c>
 * will be returned and no operation will be performed.
 *
 * @param[in,out] t The tree to which the element should be added.
 * @param[in] ptr The element to add to the tree.
 * @return @p ptr if the addition was successful, or <c>NULL</c> otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid tree or element is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void       *S_tree_insert(Stree *, void *);

/**
 * @brief Remove an element from a tree and return it.
 *
 * Removing an element from a tree will automatically balance the tree if
 * required to ensure that it remains optimised. Note that removing an element
 * will not free it from memory, rather, it is the return value.
 *
 * @param[in,out] t The tree from which the element should be removed.
 * @param[in] ptr The element to remove from the tree.
 * @return @p ptr if the removal was successful, or <c>NULL</c> otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid tree or element is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void       *S_tree_remove(Stree *, void *);

/**
 * @brief Remove all elements from a tree.
 *
 * If a given tree is already empty, then this function will return immediately
 * and do nothing.
 *
 * Note that cleraing a tree does not free the contained pointers from memory -
 * this is a separate task that must be handled by the user.
 *
 * @param[in,out] t The tree to be emptied of its contents.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid tree is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void        S_tree_clear(Stree *);

/**
 * @brief Get an iterator for a tree pointing to the smallest element of the
 * tree.
 *
 * Returns an iterator that points to the smallest element of a tre. Note that
 * this iterator cannot move backwards until it is moved forwards at least once.
 *
 * Tree traversal occurs in an in-order manner.
 *
 * @param[in] t The tree for which the iterator should be generated.
 * @return An iterator pointing to the smallest element of a given tree.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid tree is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Stree_iter *S_tree_iter_begin(Stree *);

/**
 * @brief Get an iterator for a tree pointing to the largest element of the
 * tree.
 *
 * Returns an iterator that points to the largest element of a tre. Note that
 * this iterator cannot move forwards until it is moved backwards at least once.
 *
 * Tree traversal occurs in an in-order manner.
 *
 * @param[in] t The tree for which the iterator should be generated.
 * @return An iterator pointing to the largest element of a given tree.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid tree is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Stree_iter *S_tree_iter_end(Stree *);

/**
 * @brief Advance an iterator to the next element.
 *
 * Takes an iterator and advances it to the next element, returning the element
 * as it does so. Note that this function <b>must</b> be used in conjunction
 * with {@link S_tree_iter_hasnext(const Stree_iter *)} to determine whether or
 * not the advance is a possible action, or else the behaviour by this function
 * shall be undefined.
 *
 * @param[in,out] i The iterator from which the next pointer shall be returned.
 * @return The next pointer in a given iteration.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid iterator is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void       *S_tree_iter_next(Stree_iter **);

/**
 * @brief Advance an iterator to the previous element.
 *
 * Takes an iterator and advances it to the previous element, returning the
 * element as it does so. Note that this function <b>must</b> be used in
 * conjunction with {@link S_tree_iter_hasprev(const Stree_iter *)} to determine
 * whether or not the advance is a possible action, or else the behaviour by
 * this function shall be undefined.
 *
 * @param[in,out] i The iterator from which the previous pointer shall be
 * returned.
 * @return The previous pointer in a given iteration.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid iterator is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void       *S_tree_iter_prev(Stree_iter **);

/**
 * @brief Check whether or not a tree iterator has a following element or not.
 *
 * @param[in] i The iterator to check.
 * @return {@link S_TRUE} if an iterator has more elements ahead of it, or
 * {@link S_FALSE} otherwise.
 * @since 1.0.0
 */
STICKY_API Sbool       S_tree_iter_hasnext(const Stree_iter *);

/**
 * @brief Check whether or not a tree iterator has a previous element or not.
 *
 * @param[in] i The iterator to check.
 * @return {@link S_TRUE} if an iterator has more elements behind of it, or
 * {@link S_FALSE} otherwise.
 * @since 1.0.0
 */
STICKY_API Sbool       S_tree_iter_hasprev(const Stree_iter *);

/**
 * @brief Get the smallest element from a tree and return it.
 *
 * If the tree is empty, <c>NULL</c> is returned.
 *
 * @param[in,out] t The tree from which the element should be retrieved.
 * @return The smallest element in the tree if retrieval was successful, or
 * <c>NULL</c> otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid tree is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void       *S_tree_get_min(const Stree *);

/**
 * @brief Get the largest element from a tree and return it.
 *
 * If the tree is empty, <c>NULL</c> is returned.
 *
 * @param[in,out] t The tree from which the element should be retrieved.
 * @return The largest element in the tree if retrieval was successful, or
 * <c>NULL</c> otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid tree is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API void       *S_tree_get_max(const Stree *);

/**
 * @brief Search a tree for an element.
 *
 * Searches within a tree for a given element, and returns {@link S_TRUE} if an
 * element of equivalent value can be found.
 *
 * @param[in] t The tree in which the search is conducted.
 * @param[in] ptr The element to search for within the tree.
 * @return {@link S_TRUE} if the element is found, else {@link S_FALSE}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid tree is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Sbool       S_tree_search(const Stree *, const void *);

/**
 * @brief Get the size of a tree.
 *
 * Returns the number of elements that are currently contained within a given
 * tree.
 *
 * @param[in] t The tree from which the size should be retrieved.
 * @return The size of a given tree, or 0 in case of an error.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid tree is provided to
 * the function.
 * @since 1.0.0
 */
STICKY_API Ssize_t     S_tree_size(const Stree *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_TREE_H */

