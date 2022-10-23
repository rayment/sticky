/*
 * material.c
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#include "sticky/common/error.h"
#include "sticky/common/types.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/material.h"

Smaterial *
S_material_new(void)
{
	Smaterial *mat;
	Suint8 i;
	mat = (Smaterial *) S_memory_new(sizeof(Smaterial));
	mat->assigned = 0;
	for (i = 0; i < S_MATERIAL_LIMIT; ++i)
	{
		*(mat->bitfb+i) = S_FALSE;
		(mat->tex+i)->tex = NULL;
	}
	return mat;
}

void
S_material_delete(Smaterial *mat)
{
	if (!mat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_material_delete");
		return;
	}
	S_memory_delete(mat);
}

void
S_material_set_texture(Smaterial *mat,
                       Suint8 idx,
					   const Stexture *tex)
{
	if (!mat || idx >= S_MATERIAL_LIMIT)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_material_set_texture");
		return;
	}
	if (tex && !(mat->tex+idx)->tex)
		++mat->assigned;
	else if (!tex && (mat->tex+idx)->tex)
		--mat->assigned;
	*(mat->bitfb+idx) = S_FALSE;
	(mat->tex+idx)->tex = tex;
}

void
S_material_set_framebuffer(Smaterial *mat,
                           Suint8 idx,
						   const Sframebuffer *buf)
{
	if (!mat || idx >= S_MATERIAL_LIMIT)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_material_set_framebuffer");
		return;
	}
	if (buf && !(mat->tex+idx)->fb)
		++mat->assigned;
	else if (!buf && (mat->tex+idx)->fb)
		--mat->assigned;
	*(mat->bitfb+idx) = S_TRUE;
	(mat->tex+idx)->fb = buf;
}

void
_S_material_attach(Smaterial *mat)
{
	Suint8 i;
	if (!mat)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_material_attach");
		return;
	}
	if (mat->assigned == 0)
		return;
	for (i = 0; i < S_MATERIAL_LIMIT; ++i)
	{
		if (!*(mat->bitfb+i) && (mat->tex+i)->tex)
		{
			_S_CALL("_S_texture_attach",
			        _S_texture_attach((mat->tex+i)->tex, i));
		}
		else if (*(mat->bitfb+i) && (mat->tex+i)->fb)
		{
			_S_CALL("_S_texture_attach_raw",
			        _S_texture_attach_raw((mat->tex+i)->fb->tex_color,
					                      i, S_FALSE));
		}
	}
}

