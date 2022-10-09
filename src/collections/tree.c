/*
 * tree.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 16/04/2022
 */

/* See: https://rosettacode.org/wiki/AVL_tree/C */

#include "sticky/collections/tree.h"
#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/math/math.h"
#include "sticky/memory/allocator.h"

Stree *
S_tree_new(Stree_comparator comparator)
{
	Stree *t;
	t = (Stree *) S_memory_new(sizeof(Stree));
	t->root = NULL;
	t->min = NULL;
	t->max = NULL;
	t->len = 0;
	t->comparator = comparator;
	return t;
}

void
S_tree_delete(Stree *t)
{
	if (!t)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_free");
		return;
	}
	_S_CALL("S_tree_clear", S_tree_clear(t));
	S_memory_delete(t);
}

void
S_tree_set_comparator(Stree *t,
                      Stree_comparator comparator)
{
	if (!t || !comparator)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_set_comparator");
		return;
	}
	t->comparator = comparator;
}

void
S_tree_clear(Stree *t)
{
	if (!t)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_clear");
		return;
	}
	while (t->root)
	{
		_S_CALL("S_tree_remove", S_tree_remove(t, t->root->ptr));
	}
	t->root = NULL;
	t->min = NULL;
	t->max = NULL;
	t->len = 0;
	/* TODO: IMPLEMENT */
}

static
_Stree_node *
_S_tree_new_node(void *ptr,
                 _Stree_node *parent)
{
	_Stree_node *n;
	n = (_Stree_node *) S_memory_new(sizeof(_Stree_node));
	n->ptr = ptr;
	n->parent = parent;
	n->left = NULL;
	n->right = NULL;
	n->bal = 0;
	return n;
}

static
_Stree_node *
_S_tree_rot_l(Stree *t,
              _Stree_node *n)
{
	_Stree_node *r, *rl, *p;
	if (!t || !n)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_tree_rot_l");
		return NULL;
	}
	r = n->right;
	rl = r->left;
	p = n->parent;

	r->parent = p;
	r->left = n;
	n->right = rl;
	n->parent = r;

	if (rl)
		rl->parent = n;
	if (n == t->root)
		t->root = r;
	else if (p->right == n)
		p->right = r;
	else
		p->left = r;

	--r->bal;
	n->bal = -r->bal;
	return r;
}

static
_Stree_node *
_S_tree_rot_r(Stree *t,
              _Stree_node *n)
{
	_Stree_node *l, *lr, *p;
	if (!t || !n)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_tree_rot_r");
		return NULL;
	}
	l = n->left;
	lr = l->right;
	p = n->parent;

	l->parent = p;
	l->right = n;
	n->left = lr;
	n->parent = l;

	if (lr)
		lr->parent = n;
	if (n == t->root)
		t->root = l;
	else if (p->left == n)
		p->left = l;
	else
		p->right = l;

	++l->bal;
	n->bal = -l->bal;
	return l;
}

static
_Stree_node *
_S_tree_rot_lr(Stree *t,
              _Stree_node *n)
{
	_Stree_node *l, *lr, *lrr, *lrl, *p;
	if (!t || !n)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_tree_rot_lr");
		return NULL;
	}
	l = n->left;
	lr = l->right;
	lrr = lr->right;
	lrl = lr->left;
	p = n->parent;

	lr->parent = p;
	n->left = lrr;
	l->right = lrl;
	lr->left = l;
	lr->right = n;
	l->parent = lr;
	n->parent = lr;

	if (lrr)
		lrr->parent = n;
	if (lrl)
		lrl->parent = l;
	if (n == t->root)
		t->root = lr;
	else if (p->left == n)
		p->left = lr;
	else
		p->right = lr;

	if (lr->bal == 1)
	{
		n->bal = 0;
		l->bal = -1;
	}
	else if (lr->bal == 0)
	{
		n->bal = 0;
		l->bal = 0;
	}
	else
	{
		n->bal = 1;
		l->bal = 0;
	}
	lr->bal = 0;
	return lr;
}

static
_Stree_node *
_S_tree_rot_rl(Stree *t,
              _Stree_node *n)
{
	_Stree_node *r, *rl, *rll, *rlr, *p;
	if (!t || !n)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_tree_rot_rl");
		return NULL;
	}
	r = n->right;
	rl = r->left;
	rll = rl->left;
	rlr = rl->right;
	p = n->parent;

	rl->parent = p;
	n->right = rll;
	r->left = rlr;
	rl->right = r;
	rl->left = n;
	r->parent = rl;
	n->parent = rl;

	if (rll)
		rll->parent = n;
	if (rlr)
		rlr->parent = r;
	if (n == t->root)
		t->root = rl;
	else if (p->right == n)
		p->right = rl;
	else
		p->left = rl;

	if (rl->bal == -1)
	{
		n->bal = 0;
		r->bal = 1;
	}
	else if (rl->bal == 0)
	{
		n->bal = 0;
		r->bal = 0;
	}
	else
	{
		n->bal = -1;
		r->bal = 0;
	}
	rl->bal = 0;
	return rl;
}

static
void
_S_tree_balance_insert(Stree *t,
                      _Stree_node *n,
                      Sssize_t b)
{
	_Stree_node *parent;
	if (!t)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_tree_balance_insert");
		return;
	}
	while (n)
	{
		b = (n->bal += b);
		if (b == 0)
		{
			return;
		}
		else if (b == -2)
		{
			if (n->left->bal == -1)
			{
				_S_CALL("_S_tree_rot_r", _S_tree_rot_r(t, n));
			}
			else
			{
				_S_CALL("_S_tree_rot_lr", _S_tree_rot_lr(t, n));
			}
			return;
		}
		else if (b == 2)
		{
			if (n->right->bal == 1)
			{
				_S_CALL("_S_tree_rot_l", _S_tree_rot_l(t, n));
			}
			else
			{
				_S_CALL("_S_tree_rot_rl", _S_tree_rot_rl(t, n));
			}
			return;
		}
		parent = n->parent;
		if (parent)
			b = parent->left == n ? -1 : 1;
		n = parent;
	}
}

void
_S_tree_balance_remove(Stree *t,
                      _Stree_node *n,
                      Sssize_t b)
{
	_Stree_node *parent;
	if (!t)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_tree_balance_remove");
		return;
	}
	while (n)
	{
		b = (n->bal += b);
		if (b == -2)
		{
			if (n->left->bal <= 0)
			{
				_S_CALL("_S_tree_rot_r", n = _S_tree_rot_r(t, n));
				if (n->bal == 1)
					return;
			}
			else
			{
				_S_CALL("_S_tree_rot_lr", n = _S_tree_rot_lr(t, n));
			}
		}
		else if (b == 2)
		{
			if (n->right->bal >= 0)
			{
				_S_CALL("_S_tree_rot_l", n = _S_tree_rot_l(t, n));
				if (n->bal == -1)
					return;
			}
			else
			{
				_S_CALL("_S_tree_rot_rl", n = _S_tree_rot_rl(t, n));
			}
		}
		else if (b != 0)
		{
			return;
		}
		parent = n->parent;
		if (parent)
			b = parent->left == n ? 1 : -1;
		n = parent;
	}
}

void *
S_tree_insert(Stree *t,
              void *ptr)
{
	_Stree_node *n;
	Scomparator comp;
	if (!t || !ptr)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_insert");
		return NULL;
	}
	if (!t->root)
	{
		_S_CALL("_S_tree_new_node", t->root = _S_tree_new_node(ptr, NULL));
		t->min = t->root;
		t->max = t->root;
		++t->len;
	}
	else
	{
		n = t->root;
		while (n)
		{
			if ((comp = t->comparator(ptr, n->ptr)) < 0)
			{
				if (!n->left)
				{
					_S_CALL("_S_tree_new_node",
					        n->left = _S_tree_new_node(ptr, n));
					_S_CALL("_S_tree_balance_insert",
					        _S_tree_balance_insert(t, n, -1));
					if (n == t->min)
						t->min = n->left;
					++t->len;
					return ptr;
				}
				else
				{
					n = n->left;
				}
			}
			else if (comp > 0)
			{
				if (!n->right)
				{
					_S_CALL("_S_tree_new_node",
					        n->right = _S_tree_new_node(ptr, n));
					_S_CALL("_S_tree_balance_insert",
					        _S_tree_balance_insert(t, n, 1));
					if (n == t->max)
						t->max = n->right;
					++t->len;
					return ptr;
				}
				else
				{
					n = n->right;
				}
			}
			else
			{
				/* duplicate values */
				_S_SET_ERROR(S_INVALID_OPERATION, "S_tree_insert");
				return NULL;
			}
		}
	}
	return ptr;
}

static
void
_S_tree_replace(_Stree_node *src,
                _Stree_node *target)
{
	_Stree_node *l, *r;
	l = src->left;
	r = src->right;
	target->bal = src->bal;
	target->ptr = src->ptr;
	target->left = l;
	target->right = r;
	if (l)
		l->parent = target;
	if (r)
		r->parent = target;
}

void *
S_tree_remove(Stree *t,
              void *ptr)
{
	_Stree_node *n, *l, *r, *p, *del, *suc, *sucr, *sucp;
	Scomparator comp;
	void *tmp;
	if (!t || !ptr)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_remove");
		return NULL;
	}
	n = t->root;
	while (n)
	{
		if ((comp = t->comparator(ptr, n->ptr)) < 0)
			n = n->left;
		else if (comp > 0)
			n = n->right;
		else
			break; /* found the value */
	}
	if (!n)
	{
		/* no match found */
		return NULL;
	}
	tmp = n->ptr;
	l = n->left;
	r = n->right;
	del = n;
	if (!l)
	{
		if (!r)
		{
			if (n == t->root)
			{
				t->root = NULL;
			}
			else
			{
				p = n->parent;
				if (p->left == n)
				{
					p->left = NULL;
					_S_CALL("_S_tree_balance_remove",
					        _S_tree_balance_remove(t, p, 1));
				}
				else
				{
					p->right = NULL;
					_S_CALL("_S_tree_balance_remove",
					        _S_tree_balance_remove(t, p, -1));
				}
			}
		}
		else
		{
			_S_CALL("_S_tree_replace", _S_tree_replace(r, n));
			_S_CALL("_S_tree_balance_remove", _S_tree_balance_remove(t, n, 0));
			del = r;
		}
	}
	else if (!r)
	{
		_S_CALL("_S_tree_replace", _S_tree_replace(l, n));
		_S_CALL("_S_tree_balance_remove", _S_tree_balance_remove(t, n, 0));
		del = l;
	}
	else
	{
		suc = r;
		if (!suc->left)
		{
			p = n->parent;
			suc->parent = p;
			suc->left = l;
			suc->bal = n->bal;
			if (l)
				l->parent = suc;
			if (n == t->root)
				t->root = suc;
			else if (p->left == n)
				p->left = suc;
			else
				p->right = suc;
			_S_CALL("_S_tree_balance_remove",
			        _S_tree_balance_remove(t, suc, -1));
		}
		else
		{
			while (suc->left)
				suc = suc->left;
			p = n->parent;
			sucp = suc->parent;
			sucr = suc->right;
			if (sucp->left == suc)
				sucp->left = sucr;
			else
				sucp->right = sucr;
			if (sucr)
				sucr->parent = sucp;
			suc->parent = p;
			suc->left = l;
			suc->bal = n->bal;
			suc->right = r;
			r->parent = suc;
			if (l)
				l->parent = suc;
			if (n == t->root)
				t->root = suc;
			else if (p->left == n)
				p->left = suc;
			else
				p->right = suc;
			_S_CALL("_S_tree_balance_remove",
			        _S_tree_balance_remove(t, sucp, 1));
		}
	}
	if (del == t->min)
		t->min = del->parent;
	if (del == t->max)
		t->max = del->parent;
	--t->len;
	S_memory_delete(del);
	return tmp;
}

Stree_iter *
S_tree_iter_begin(Stree *t)
{
	if (!t)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_iter_begin");
		return NULL;
	}
	return t->min;
}

Stree_iter *
S_tree_iter_end(Stree *t)
{
	if (!t)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_iter_begin");
		return NULL;
	}
	return t->max;
}

void *
S_tree_iter_next(Stree_iter **i)
{
	Stree_iter *val, *tmp, *tmp2;
	if (!i || !*i)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_iter_next");
		return NULL;
	}
	val = tmp = *i;
	if (tmp->right)
	{
		tmp = tmp->right;
		while (tmp->left)
			tmp = tmp->left;
	}
	else
	{
		tmp2 = tmp;
		tmp = tmp->parent;
		while (tmp && tmp->right == tmp2)
		{
			tmp2 = tmp;
			tmp = tmp->parent;
		}
	}
	*i = tmp;
	return val->ptr;
}

void *
S_tree_iter_prev(Stree_iter **i)
{
	Stree_iter *val, *tmp, *tmp2;
	if (!i || !*i)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_iter_prev");
		return NULL;
	}
	val = tmp = *i;
	if (tmp->left)
	{
		tmp = tmp->left;
		while (tmp->right)
			tmp = tmp->right;
	}
	else
	{
		tmp2 = tmp;
		tmp = tmp->parent;
		while (tmp && tmp->left == tmp2)
		{
			tmp2 = tmp;
			tmp = tmp->parent;
		}
	}
	*i = tmp;
	return val->ptr;
}

Sbool
S_tree_iter_hasnext(const Stree_iter *i)
{
	return i != NULL;
}

Sbool
S_tree_iter_hasprev(const Stree_iter *i)
{
	return i != NULL;
}

void *
S_tree_get_min(const Stree *t)
{
	if (!t)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_get_min");
		return NULL;
	}
	if (t->min)
		return t->min->ptr;
	return NULL;
}

void *
S_tree_get_max(const Stree *t)
{
	if (!t)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_get_max");
		return NULL;
	}
	if (t->max)
		return t->max->ptr;
	return NULL;
}

Sbool
S_tree_search(const Stree *t,
              const void *ptr)
{
	_Stree_node *n;
	Scomparator comp;
	if (!t || !ptr)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_search");
		return S_FALSE;
	}
	n = t->root;
	while (n)
	{
		if ((comp = t->comparator(ptr, n->ptr)) < 0)
			n = n->left;
		else if (comp > 0)
			n = n->right;
		else
			return S_TRUE;
	}
	return S_FALSE;
}

Ssize_t
S_tree_size(const Stree *t)
{
	if (!t)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_tree_size");
		return 0;
	}
	return t->len;
}

