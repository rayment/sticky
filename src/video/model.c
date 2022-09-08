/*
 * model.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/model.h"

Smodel *
S_model_new(Smesh *mesh,
            Smaterial *mat,
			Sshader *shader)
{
	Smodel *model;

	model = (Smodel *) S_memory_new(sizeof(Smodel));
	model->mesh = mesh;
	model->mat = mat;
	model->shader = shader;
	model->parent = NULL;
	_S_CALL("S_linkedlist_new", model->children = S_linkedlist_new());

	return model;
}

void
S_model_delete(Smodel *model)
{
	if (!model)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_model_delete");
		return;
	}
	_S_CALL("S_linkedlist_delete", S_linkedlist_delete(model->children));
	S_memory_delete(model);
}

void
S_model_delete_tree(Smodel *model)
{
	Slinkedlist_iter *iter;
	Smodel *child;
	Ssize_t len;
	Sbool b;
	if (!model)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_model_delete");
		return;
	}
	_S_CALL("S_linkedlist_size", len = S_linkedlist_size(model->children));
	if (len < 1)
		return;
	_S_CALL("S_linkedlist_iter_begin",
	        iter = S_linkedlist_iter_begin(model->children));
	while (1)
	{
		_S_CALL("S_linkedlist_iter_hasnext",
		        b = S_linkedlist_iter_hasnext(iter));
		if (!b)
			break;
		_S_CALL("S_linkedlist_iter_next",
		        child = (Smodel *) S_linkedlist_iter_next(&iter));
		S_model_delete_tree(child);
	}
	_S_CALL("S_model_delete", S_model_delete(model));
}

void
S_model_set_mesh(Smodel *model,
                 Smesh *mesh)
{
	if (!model)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_model_set_mesh");
		return;
	}
	model->mesh = mesh;
}

void
S_model_set_material(Smodel *model,
                    Smaterial *mat)
{
	if (!model)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_model_set_material");
		return;
	}
	model->mat = mat;
}

void
S_model_set_shader(Smodel *model,
                   Sshader *shader)
{
	if (!model)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_model_set_shader");
		return;
	}
	model->shader = shader;
}

Smesh *
S_model_get_mesh(const Smodel *model)
{
	if (!model)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_model_get_mesh");
		return NULL;
	}
	return model->mesh;
}

Smaterial *
S_model_get_material(const Smodel *model)
{
	if (!model)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_model_get_material");
		return NULL;
	}
	return model->mat;
}

Sshader *
S_model_get_shader(const Smodel *model)
{
	if (!model)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_model_get_shader");
		return NULL;
	}
	return model->shader;
}

