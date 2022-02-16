/*
 * linkedlist.c
 * Linked list implementation.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 15/03/2021
 */

#include "sticky/collections/linkedlist.h"
#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/memory/allocator.h"

/* calculate iteration direction and prepare iterator for a given index */
static
void
_S_linkedlist_iter_calculate(Slinkedlist *l,
                             _Slinkedlist_node **n,
                             Ssize_t i,
                             int *dir)
{
	if (l->iter)
	{
		*dir = 1;
		*n = l->head;
		if (i < l->iterpos)
		{
			if (l->iterpos - i <= i)
			{
				/* iterator is closest to element */
				*n = l->iter;
				*dir = -1;
			}
			else
			{
				/* HEAD is closest to element */
				l->iterpos = 0;
				*dir = 1;
			}
		}
		else if (i > l->iterpos)
		{
			if (i - l->iterpos > l->len - i)
			{
				/* TAIL is closest to element */
				*n = l->tail;
				l->iterpos = l->len - 1;
				*dir = -1;
			}
			else
			{
				/* iterator is closest to element */
				*n = l->iter;
				*dir = 1;
			}
		}
	}
	else
	{
		/* create iterator */
		*n = l->iter = l->head;
		l->iterpos = 0;
		*dir = 1;
	}
}

Slinkedlist *
S_linkedlist_new(void)
{
	Slinkedlist *l;
	l = (Slinkedlist *) S_memory_new(sizeof(Slinkedlist));
	l->head = NULL;
	l->tail = NULL;
	l->iter = NULL;
	l->len = 0;
	l->iterpos = 0;
	return l;
}

void
S_linkedlist_destroy(Slinkedlist *l)
{
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_free");
		return;
	}
	_S_CALL("S_linkedlist_clear", S_linkedlist_clear(l));
	S_memory_delete(l);
}

void *
S_linkedlist_add(Slinkedlist *l,
                 void *val,
                 Ssize_t i)
{
	int dir;
	void *ptr;
	_Slinkedlist_node *itern, *n;
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_add");
		return NULL;
	}
	else if (i > l->len)
	{
		_S_SET_ERROR(S_INVALID_INDEX, "S_linkedlist_add");
		return NULL;
	}
	if (i == 0)
	{
		_S_CALL("S_linkedlist_add_head", ptr = S_linkedlist_add_head(l, val));
		return ptr;
	}
	else if (i == l->len)
	{
		_S_CALL("S_linkedlist_add_tail", ptr = S_linkedlist_add_tail(l, val));
		return ptr;
	}
	_S_CALL("_S_linkedlist_iter_calculate",
	        _S_linkedlist_iter_calculate(l, &itern, i, &dir));
	while (l->iterpos != i)
	{
		if (dir > 0)
			itern = itern->next;
		else if (dir < 0)
			itern = itern->last;
		l->iterpos += dir;
	}
	n = (_Slinkedlist_node *) S_memory_new(sizeof(_Slinkedlist_node));
	n->ptr = val;
	n->next = itern;
	if (l->iter)
	{
		n->last = itern->last;
		itern->last = n;
		if (n->last)
			n->last->next = n;
		else
			l->head = n;
	}
	else
	{
		n->last = NULL;
		l->head = n;
	}
	l->iter = n;
	++l->len;
	return val;
}

void *
S_linkedlist_add_head(Slinkedlist *l,
                      void *val)
{
	_Slinkedlist_node *n;
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_add_head");
		return NULL;
	}
	++l->iterpos; /* this element is being pushed forward */
	n = (_Slinkedlist_node *) S_memory_new(sizeof(_Slinkedlist_node));
	n->next = l->head;
	if (n->next)
		n->next->last = n;
	n->last = NULL;
	n->ptr = val;
	if (!l->head)
		l->tail = n;
	l->head = n;
	++l->len;
	return val;
}

void *
S_linkedlist_add_tail(Slinkedlist *l,
                      void *val)
{
	_Slinkedlist_node *n;
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_add_tail");
		return NULL;
	}
	n = (_Slinkedlist_node *) S_memory_new(sizeof(_Slinkedlist_node));
	n->last = l->tail;
	if (n->last)
		n->last->next = n;
	n->next = NULL;
	n->ptr = val;
	if (!l->tail)
		l->head = n;
	l->tail = n;
	++l->len;
	return val;
}

Sbool
S_linkedlist_remove_ptr(Slinkedlist *l,
                        const void *val)
{
	_Slinkedlist_node *n;
	Ssize_t j;
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_remove_ptr");
		return S_FALSE;
	}
	n = l->head;
	j = 0;
	while (n)
	{
		if (n->ptr == val)
		{
			if (n->last)
				n->last->next = n->next;
			else
				l->head = l->head->next;
			if (n->next)
				n->next->last = n->last;
			else
				l->tail = l->tail->last;
			--l->len;
			if (l->iter && j == l->iterpos)
			{
				if (n->last)
				{
					--l->iterpos;
					l->iter = n->last;
				}
				else if (n->next)
				{
					l->iter = n->next;
				}
				else
				{
					l->iter = NULL;
				}
			}
			else if (l->iter && j < l->iterpos)
			{
				--l->iterpos;
			}
			S_memory_delete(n);
			return S_TRUE;
		}
		++j;
		n = n->next;
	}
	return S_FALSE;
}

void *
S_linkedlist_remove(Slinkedlist *l,
                    Ssize_t i)
{
	int dir;
	_Slinkedlist_node *n;
	void *ptr;
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_remove");
		return NULL;
	}
	else if (i >= l->len)
	{
		_S_SET_ERROR(S_INVALID_INDEX, "S_linkedlist_remove");
		return NULL;
	}
	if (l->len == 0)
		return NULL;
	if (i == 0)
	{
		_S_CALL("S_linkedlist_remove_head", ptr = S_linkedlist_remove_head(l));
		return ptr;
	}
	else if (i == l->len - 1)
	{
		_S_CALL("S_linkedlist_remove_tail", ptr = S_linkedlist_remove_tail(l));
		return ptr;
	}
	else if (l->iter && i == l->iterpos)
	{
		n = l->iter;
	}
	else
	{
		_S_CALL("_S_linkedlist_iter_calculate",
		        _S_linkedlist_iter_calculate(l, &n, i, &dir));
		while (l->iterpos != i)
		{
			if (dir > 0)
				n = n->next;
			else if (dir < 0)
				n = n->last;
			l->iterpos += dir;
		}
		l->iter = n;
	}
	n->last->next = n->next;
	n->next->last = n->last;
	--l->len;
	ptr = n->ptr;
	if (l->iter && i == l->iterpos)
	{
		if (n->last)
		{
			--l->iterpos;
			l->iter = n->last;
		}
		else if (n->next)
		{
			l->iter = n->next;
		}
		else
		{
			l->iter = NULL;
		}
	}
	else if (l->iter && i < l->iterpos)
	{
		--l->iterpos;
	}
	S_memory_delete(n);
	return ptr;
}

void *
S_linkedlist_remove_head(Slinkedlist *l)
{
	_Slinkedlist_node *n;
	void *ptr;
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_remove_head");
		return NULL;
	}
	if (l->len == 0)
		return NULL;
	n = l->head;
	l->head = l->head->next;
	if (l->iter == n)
		l->iter = l->head;
	else
		--l->iterpos;
	if (l->head)
		l->head->last = NULL;
	else
		l->tail = NULL;
	ptr = n->ptr;
	S_memory_delete(n);
	--l->len;
	return ptr;
}

void *
S_linkedlist_remove_tail(Slinkedlist *l)
{
	_Slinkedlist_node *n;
	void *ptr;
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_remove_tail");
		return NULL;
	}
	if (l->len == 0)
		return NULL;
	n = l->tail;
	l->tail = l->tail->last;
	if (l->iter == n)
	{
		l->iter = l->tail;
		--l->iterpos;
	}
	if (l->tail)
		l->tail->next = NULL;
	else
		l->head = NULL;
	ptr = n->ptr;
	S_memory_delete(n);
	--l->len;
	return ptr;
}

void
S_linkedlist_clear(Slinkedlist *l)
{
	_Slinkedlist_node *n, *tmp;
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_clear");
		return;
	}
	if (l->len == 0)
		return;
	n = l->head;
	while (n)
	{
		tmp = n->next;
		S_memory_delete(n);
		n = tmp;
	}
	l->head = NULL;
	l->tail = NULL;
	l->iter = NULL;
	l->iterpos = 0;
	l->len = 0;
}

void *
S_linkedlist_get(Slinkedlist *l,
                 Ssize_t i)
{
	int dir;
	_Slinkedlist_node *n;
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_get");
		return NULL;
	}
	else if (i >= l->len)
	{
		_S_SET_ERROR(S_INVALID_INDEX, "S_linkedlist_get");
		return NULL;
	}
	if (i == 0)
		return l->head;
	else if (i == l->len - 1)
		return l->tail;
	else if (l->iter && i == l->iterpos)
		return l->iter->ptr;
	_S_CALL("_S_linkedlist_iter_calculate",
	        _S_linkedlist_iter_calculate(l, &n, i, &dir));
	while (l->iterpos != i)
	{
		if (dir > 0)
			n = n->next;
		else if (dir < 0)
			n = n->last;
		l->iterpos += dir;
	}
	l->iter = n;
	return n->ptr;
}

void *
S_linkedlist_get_head(const Slinkedlist *l)
{
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_get_head");
		return NULL;
	}
	if (l->len == 0)
		return NULL;
	return l->head->ptr;
}

void *
S_linkedlist_get_tail(const Slinkedlist *l)
{
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_get_tail");
		return NULL;
	}
	if (l->len == 0)
		return NULL;
	return l->tail->ptr;
}

Sbool
S_linkedlist_search(const Slinkedlist *l,
                    const void *val,
                    Ssize_t *i)
{
	_Slinkedlist_node *n;
	Ssize_t j;
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_search");
		return S_FALSE;
	}
	n = l->head;
	j = 0;
	while (n)
	{
		if (n->ptr == val)
		{
			if (i)
				*i = j;
			return S_TRUE;
		}
		++j;
		n = n->next;
	}
	return S_FALSE;
}

Ssize_t
S_linkedlist_size(const Slinkedlist *l)
{
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_size");
		return 0;
	}
	return l->len;
}

Slinkedlist_iter *
S_linkedlist_iter_begin(Slinkedlist *l)
{
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_iter_begin");
		return NULL;
	}
	return l->head;
}

Slinkedlist_iter *
S_linkedlist_iter_end(Slinkedlist *l)
{
	if (!l)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_iter_end");
		return NULL;
	}
	return l->tail;
}

void *
S_linkedlist_iter_next(Slinkedlist_iter **i)
{
	Slinkedlist_iter *tmp;
	if (!i || !*i)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_iter_next");
		return NULL;
	}
	tmp = *i;
	*i = (*i)->next;
	return tmp->ptr;
}

void *
S_linkedlist_iter_prev(Slinkedlist_iter **i)
{
	Slinkedlist_iter *tmp;
	if (!i || !*i)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_linkedlist_iter_prev");
		return NULL;
	}
	tmp = *i;
	*i = (*i)->last;
	return tmp->ptr;
}

Sbool
S_linkedlist_iter_hasnext(const Slinkedlist_iter *i)
{
	return i != NULL;
}

Sbool
S_linkedlist_iter_hasprev(const Slinkedlist_iter *i)
{
	return i != NULL;
}

