/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * linkedlist.h
 * Linked list header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/03/2021
 */

#ifndef FR_RAYMENT_STICKY_LINKEDLIST_H
#define FR_RAYMENT_STICKY_LINKEDLIST_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/types.h"

/**
 * @addtogroup linkedlist
 * @{
 */

typedef struct
_Slinkedlist_node_s
{
	void *ptr;
	struct _Slinkedlist_node_s *next, *last;
} _Slinkedlist_node;

/**
 * @brief Doubley linked-list struct.
 *
 * Linked-lists store information in a dynamically sized format, so that when
 * elements are added or removed, the list may resize itself automatically,
 * opposed to an array that may only hold a fixed number of element calculated
 * in advance.
 *
 * The implementation of linked-lists in <b>sticky</b> are doubley linked, that
 * is, each element of a list holds a pointer to the next <i>and</i> previous
 * element, which allows for robust actions such as iterating backwards through
 * a given list.
 *
 * Each list furthermore contains an iterator to speed up addition and removal
 * at arbitrary indexes as the list does not need to iterate directly from the
 * beginning or end, but from a previously stored index. As such, inserting or
 * removing sequential elements in separate calls, starting from a random
 * position within the list, may be performed in @f$O(1)@f$ time for every
 * additional element (the first element to be manipulated at a given index
 * will always take @f$O(n)@f$ time to perform the action).
 *
 * @warning Linked-lists are not thread safe. To ensure synchronisation across
 * threads, {@link Smutex} must be used to synchronise interactions with the
 * list.
 *
 * @since 1.0.0
 */
typedef struct
Slinkedlist_s
{
	struct _Slinkedlist_node_s *head, *tail, *iter;
	Ssize_t len, iterpos;
} Slinkedlist;

/**
 * @brief Linked-list iterator type.
 *
 * The linked-list iterator is a struct that is allocated on the stack to
 * iterate over a linked-list in either the forwards or backwards direction.
 *
 * An iterator is internally used within the {@link Slinkedlist} data-type which
 * speeds up element read/write when an index other than the head or tail of the
 * list are given. This allows for insertion of multiple sequential elements
 * into the middle of a pre-existing list in @f$O(n)@f$ time for the first
 * element, and @f$O(1)@f$ for every element following. It is thus useful to use
 * an iterator when multiple elements are being read or written to a list.
 *
 * The typedef <c>_Slinkedlist_node_s</c> is internal and should not be accessed
 * by the user.
 *
 * @since 1.0.0
 */
typedef struct _Slinkedlist_node_s Slinkedlist_iter;

/**
 * @brief Create a new linked-list.
 *
 * Allocates a new linked-list to the heap and returns a pointer. This
 * linked-list will have no elements.
 *
 * @return A new linked-list allocated on the heap with no elements. To
 * correctly destroy the linked-list, call
 * {@link S_linkedlist_delete(Slinkedlist *)}.
 * @since 1.0.0
 */
STICKY_API Slinkedlist *S_linkedlist_new(void);

/**
 * @brief Free a linked-list from memory.
 *
 * Takes a given linked-list, clears all elements from it (note that this does
 * not deallocate the elements - that must be done separately), and frees the
 * linked-list from memory.
 *
 * Once this function is called for a given linked-list, that list becomes
 * invalid and may not be used again in any other function.
 *
 * @param[in,out] l The linked-list to free from memory.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void    S_linkedlist_delete(Slinkedlist *);

/**
 * @brief Add an element to a given index of a linked-list.
 *
 * Adds an element to a linked-list so that the resulting index of the new
 * element is equal to the one provided to the function. The previous element
 * occupying the same index will be shifted forwards by one position.
 *
 * @param[in,out] l The linked-list to which the element should be added.
 * @param[in] val The element to add to the linked-list
 * @param[in] i The index within the linked-list that the element should be
 * added to.
 * @return @p val if the addition was successful, or <c>NULL</c> otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @exception S_INVALID_INDEX If an index greater than the size of the
 * linked-list is provided to the function.
 * @since 1.0.0
 */
STICKY_API void   *S_linkedlist_add(Slinkedlist *, void *, Ssize_t);

/**
 * @brief Add an element to the start of a linked-list.
 *
 * Adds an element to a linked-list at the 0 index and pushes every other
 * element forwards by one position.
 *
 * This function call is equivalent to calling
 * {@link S_linkedlist_add(Slinkedlist *, void *, Ssize_t)} with a 0 index
 * value.
 *
 * @param[in,out] l The linked-list to which the element should be added.
 * @param[in] val The element to add to the linked-list
 * @return @p val if the addition was successful, or <c>NULL</c> otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void   *S_linkedlist_add_head(Slinkedlist *, void *);

/**
 * @brief Add an element to the end of a linked-list.
 *
 * @param[in,out] l The linked-list to which the element should be added.
 * @param[in] val The element to add to the linked-list
 * @return @p val if the addition was successful, or <c>NULL</c> otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void   *S_linkedlist_add_tail(Slinkedlist *, void *);

/**
 * @brief Remove a given element from a linked-list.
 *
 * Attempts to remove a given pointer element from a linked-list and return
 * {@link S_TRUE} only if the element was in the list and could be removed.
 *
 * Removing an element from a linked-list will cause every following element to
 * be shifted back by one position. Note that removing an element will not free
 * it from memory.
 *
 * @param[in,out] l The linked-list from which the element should be removed.
 * @param[in] val The element within the linked-list to be removed.
 * @return {@link S_TRUE} If the element was in the list and could be removed,
 * otherwise {@link S_FALSE}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API Sbool   S_linkedlist_remove_ptr(Slinkedlist *, const void *);

/**
 * @brief Remove an element at a certain index from a linked-list and return it.
 *
 * Removing an element from a linked-list will cause every following element to
 * be shifted back by one position. Note that removing an element will not free
 * it from memory, rather, it is the return value.
 *
 * @param[in,out] l The linked-list from which the element should be removed.
 * @param[in] i The index within the linked-list that the element should be
 * removed from.
 * @return @p The removed element if the removal was successful, or <c>NULL</c>
 * otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @exception S_INVALID_INDEX If an index greater than the size of the
 * linked-list is provided to the function.
 * @since 1.0.0
 */
STICKY_API void   *S_linkedlist_remove(Slinkedlist *, Ssize_t);

/**
 * @brief Remove an element from the start of a linked-list.
 *
 * Removing an element from a linked-list will cause every element following a
 * given index to be shifted back by one position. Note that removing an
 * element will not free it from memory, rather, it is the return value.
 *
 * This function call is equivalent to calling
 * {@link S_linkedlist_remove(Slinkedlist *, Ssize_t)} with a 0 index
 * value.
 *
 * @param[in,out] l The linked-list from which the element should be removed.
 * @return @p The removed element if the removal was successful, or <c>NULL</c>
 * otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void   *S_linkedlist_remove_head(Slinkedlist *);

/**
 * @brief Remove an element from the end of a linked-list.
 *
 * Note that removing an element will not free it from memory, rather, it is the
 * return value.
 *
 * @param[in,out] l The linked-list from which the element should be removed.
 * @return @p The removed element if the removal was successful, or <c>NULL</c>
 * otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void   *S_linkedlist_remove_tail(Slinkedlist *);

/**
 * @brief Remove all elements from a linked-list.
 *
 * If a given linked-list is already empty, then this function will return
 * immediately and do nothing.
 *
 * Note that clearing a linked-list does not free the contained pointers from
 * memory - this is a seperate task that must be handled by the user.
 *
 * @param[in,out] l The linked-list to be emptied of its contents.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void    S_linkedlist_clear(Slinkedlist *);

/**
 * @brief Get an element at a certain index from a linked-list and return it.
 *
 * Note that the given linked-list in this function is not marked <c>const</c>
 * because the internal iterator may be updated while traversing the list.
 *
 * @param[in,out] l The linked-list from which the element should be retrieved.
 * @param[in] i The index within the linked-list that the element should be
 * retrieved from.
 * @return @p The requested element if the retrieval was successful, or
 * <c>NULL</c> otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @exception S_INVALID_INDEX If an index greater than the size of the
 * linked-list is provided to the function.
 * @since 1.0.0
 */
STICKY_API void   *S_linkedlist_get(Slinkedlist *, Ssize_t);

/**
 * @brief Get the first element from a linked-list and return it.
 *
 * @param[in,out] l The linked-list from which the element should be retrieved.
 * @return @p The requested element if the retrieval was successful, or
 * <c>NULL</c> otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void   *S_linkedlist_get_head(const Slinkedlist *);

/**
 * @brief Get the last element from a linked-list and return it.
 *
 * @param[in,out] l The linked-list from which the element should be retrieved.
 * @return @p The requested element if the retrieval was successful, or
 * <c>NULL</c> otherwise.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API void   *S_linkedlist_get_tail(const Slinkedlist *);

/**
 * @brief Search a linked-list for an element and store the index it is found
 * in.
 *
 * @param[in] l The linked-list from which the search is conducted.
 * @param[in] val The element to search for within the linked-list.
 * @param[out] i A pointer within which to store the resulting index if the
 * element is found. This parameter may be <c>NULL</c>, in which case no index
 * shall be stored.
 * @return {@link S_TRUE} if the element is found, else {@link S_FALSE}.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API Sbool   S_linkedlist_search(const Slinkedlist *, const void *,
                                       Ssize_t *);

/**
 * @brief Get the size of a linked-list.
 *
 * Returns the number of elements that are currently contained within a given
 * linked-list.
 *
 * @param[in] l The linked-list from which the size should be retrieved.
 * @return The size of a given linked-list, or 0 in case of an error.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API Ssize_t S_linkedlist_size(const Slinkedlist *);

/**
 * @brief Get an iterator for a linked-list pointing to the beginning of the
 * list.
 *
 * Returns an iterator that points to the start of a given list. Note that this
 * iterator cannot move backwards until it is moved forwards at least once.
 *
 * @param[in] l The linked-list for which the iterator should be generated.
 * @return An iterator pointing to the start of a given linked-list.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API Slinkedlist_iter *S_linkedlist_iter_begin(Slinkedlist *);

/**
 * @brief Get an iterator for a linked-list pointing to the end of the list.
 *
 * Returns an iterator that points to the end of a given list. Note that this
 * iterator cannot move forwards until it is moved backwards at least once.
 *
 * @param[in] l The linked-list for which the iterator should be generated.
 * @return An iterator pointing to the start of a given linked-list.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid linked-list is
 * provided to the function.
 * @since 1.0.0
 */
STICKY_API Slinkedlist_iter *S_linkedlist_iter_end(Slinkedlist *);

/**
 * @brief Advance an iterator to the next element.
 *
 * Takes an iterator and advances it to the next element, returning the element
 * as it does so. Note that this function <b>must</b> be used in conjunction
 * with {@link S_linkedlist_iter_hasnext(const Slinkedlist_iter *)} to determine
 * whether or not the advance is a possible action, or else the behaviour by
 * this function shall be undefined.
 *
 * @param[in,out] i The iterator from which the next pointer shall be returned.
 * @return The next pointer in a given iteration.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid iterator is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void   *S_linkedlist_iter_next(Slinkedlist_iter **);

/**
 * @brief Advance an iterator to the previous element.
 *
 * Takes an iterator and advances it to the previous element, returning the
 * element as it does so. Note that this function <b>must</b> be used in
 * conjunction with {@link S_linkedlist_iter_hasprev(const Slinkedlist_iter *)}
 * to determine whether or not the advance is a possible action, or else the
 * behaviour by this function shall be undefined.
 *
 * @param[in,out] i The iterator from which the previous pointer shall be
 * returned.
 * @return The previous pointer in a given iteration.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid iterator is provided
 * to the function.
 * @since 1.0.0
 */
STICKY_API void   *S_linkedlist_iter_prev(Slinkedlist_iter **);

/**
 * @brief Check whether or not a list iterator has a following element or not.
 *
 * @param[in] i The iterator to check
 * @return {@link S_TRUE} if an iterator has more elements in front of it, or
 * {@link S_FALSE} otherwise.
 * @since 1.0.0
 */
STICKY_API Sbool   S_linkedlist_iter_hasnext(const Slinkedlist_iter *);

/**
 * @brief Check whether or not a list iterator has a previous element or not.
 *
 * @param[in] i The iterator to check
 * @return {@link S_TRUE} if an iterator has more elements behind it, or
 * {@link S_FALSE} otherwise.
 * @since 1.0.0
 */
STICKY_API Sbool   S_linkedlist_iter_hasprev(const Slinkedlist_iter *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_LINKEDLIST_H */

