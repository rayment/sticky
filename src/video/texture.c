/*
 * texture.c
 * OpenGL texture source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 18/04/2021
 */

#include "sticky/common/error.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

static GLint max_units;

static
Sbool
_S_texture_load_tex(const Schar *filename,
                    Sint32 *width,
                    Sint32 *height,
                    GLint *format,
                    Suint8 **dataptr)
{
	Suint8 *data;
	Sint32 w, h, channels;

	stbi_set_flip_vertically_on_load(S_FALSE);
	data = stbi_load(filename, &w, &h, &channels, 0);
	if (!data)
		_S_error_stb("_S_texture_load_tex");

	if (w % 2 != 0 || h % 2 != 0)
	{
		_S_SET_ERROR(S_INVALID_FORMAT, "_S_texture_load_tex");
		free(data); /* do not replace with S_memory_delete */
		return S_FALSE;
	}
	if (channels == 4)
	{
		*format = GL_RGBA;
	}
	else if (channels == 3)
	{
		*format = GL_RGB;
	}
	else
	{
		_S_SET_ERROR(S_INVALID_CHANNELS, "_S_texture_load_tex");
		free(data); /* do not replace with S_memory_delete */
		return S_FALSE;
	}
	*dataptr = data;
	*width = w;
	*height = h;
	return S_TRUE;
}

Stexture *
S_texture_load(const Schar *filename)
{
	Stexture *tex;
	Suint8 *data;
	Sint32 w, h;
	GLint format;
	Sbool b;

	if (!filename)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_texture_load");
		return NULL;
	}

	_S_CALL("_S_texture_load_tex",
	        b = _S_texture_load_tex(filename, &w, &h, &format, &data));
	if (!b)
		return NULL;

	tex = (Stexture *) S_memory_new(sizeof(Stexture));
	tex->cubemap = S_FALSE;

	_S_GL(glGenTextures(1, &tex->tex));
	_S_GL(glBindTexture(GL_TEXTURE_2D, tex->tex));

	_S_CALL("S_texture_set_filter",
	        S_texture_set_filter(tex, S_TEXTURE_NEAREST));
	_S_CALL("S_texture_set_wrap",
	        S_texture_set_wrap(tex, S_TEXTURE_REPEAT));

	_S_GL(glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0,
	                   format, GL_UNSIGNED_BYTE, data));
	_S_GL(glGenerateMipmap(GL_TEXTURE_2D));

	free(data); /* do not replace with S_memory_delete */
	return tex;
}

Stexture *
S_texture_load_cubemap(const Schar *px_filename,
                       const Schar *nx_filename,
                       const Schar *py_filename,
                       const Schar *ny_filename,
                       const Schar *pz_filename,
                       const Schar *nz_filename)
{
	Suint8 *data_px, *data_nx, *data_py, *data_ny, *data_pz, *data_nz;
	Stexture *tex;
	Sint32 w_px, w_nx, w_py, w_ny, w_pz, w_nz,
	       h_px, h_nx, h_py, h_ny, h_pz, h_nz,
	       i;
	GLint format_px, format_nx,
	      format_py, format_ny,
	      format_pz, format_nz;

	if (!px_filename || !nx_filename ||
	    !py_filename || !ny_filename ||
	    !pz_filename || !nz_filename)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_texture_load_cubemap");
		return NULL;
	}

	i = 0;
	_S_CALL("_S_texture_load_tex",
	        i += _S_texture_load_tex(px_filename, &w_px, &h_px,
	                                 &format_px, &data_px));
	_S_CALL("_S_texture_load_tex",
	        i += _S_texture_load_tex(nx_filename, &w_nx, &h_nx,
	                                 &format_nx, &data_nx));
	_S_CALL("_S_texture_load_tex",
	        i += _S_texture_load_tex(py_filename, &w_py, &h_py,
	                                 &format_py, &data_py));
	_S_CALL("_S_texture_load_tex",
	        i += _S_texture_load_tex(ny_filename, &w_ny, &h_ny,
	                                 &format_ny, &data_ny));
	_S_CALL("_S_texture_load_tex",
	        i += _S_texture_load_tex(pz_filename, &w_pz, &h_pz,
	                                 &format_pz, &data_pz));
	_S_CALL("_S_texture_load_tex",
	        i += _S_texture_load_tex(nz_filename, &w_nz, &h_nz,
	                                 &format_nz, &data_nz));
	if (i != 6)
		return NULL;

	tex = (Stexture *) S_memory_new(sizeof(Stexture));
	tex->cubemap = S_TRUE;

	_S_GL(glGenTextures(1, &tex->tex));
	_S_GL(glBindTexture(GL_TEXTURE_CUBE_MAP, tex->tex));

	_S_CALL("S_texture_set_filter",
	        S_texture_set_filter(tex, S_TEXTURE_NEAREST));
	_S_CALL("S_texture_set_wrap",
	        S_texture_set_wrap(tex, S_TEXTURE_CLAMP));

	_S_GL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format_px,
	                   w_px, h_px, 0, format_px, GL_UNSIGNED_BYTE, data_px));
	_S_GL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format_nx,
	                   w_nx, h_nx, 0, format_nx, GL_UNSIGNED_BYTE, data_nx));
	_S_GL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format_py,
	                   w_py, h_py, 0, format_py, GL_UNSIGNED_BYTE, data_py));
	_S_GL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format_ny,
	                   w_ny, h_ny, 0, format_ny, GL_UNSIGNED_BYTE, data_ny));
	_S_GL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format_pz,
	                   w_pz, h_pz, 0, format_pz, GL_UNSIGNED_BYTE, data_pz));
	_S_GL(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format_nz,
	                   w_nz, h_nz, 0, format_nz, GL_UNSIGNED_BYTE, data_nz));
	/* do not replace with S_memory_delete */
	free(data_px);
	free(data_nx);
	free(data_py);
	free(data_ny);
	free(data_pz);
	free(data_nz);
	return tex;
}

void
S_texture_delete(Stexture *texture)
{
	if (!texture)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_texture_delete");
		return;
	}
	_S_GL(glDeleteTextures(1, &texture->tex));
	S_memory_delete(texture);
}

void
S_texture_set_filter(Stexture *texture,
                     Senum filter)
{
	GLenum mode;
	if (!texture)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_texture_set_filter");
		return;
	}
	else if (filter != S_TEXTURE_LINEAR && filter != S_TEXTURE_NEAREST)
	{
		_S_SET_ERROR(S_INVALID_ENUM, "S_texture_set_filter");
		return;
	}
	texture->filter = filter;
	if (texture->cubemap)
		mode = GL_TEXTURE_CUBE_MAP;
	else
		mode = GL_TEXTURE_2D;
	_S_GL(glBindTexture(mode, texture->tex));
	_S_GL(glTexParameteri(mode, GL_TEXTURE_MIN_FILTER, filter));
	_S_GL(glTexParameteri(mode, GL_TEXTURE_MAG_FILTER, filter));
}

void
S_texture_set_wrap(Stexture *texture,
                   Senum wrap)
{
	GLenum mode;
	if (!texture)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_texture_set_wrap");
		return;
	}
	else if (wrap != S_TEXTURE_CLAMP && wrap != S_TEXTURE_REPEAT)
	{
		_S_SET_ERROR(S_INVALID_ENUM, "S_texture_set_wrap");
		return;
	}
	texture->wrap = wrap;
	if (texture->cubemap)
		mode = GL_TEXTURE_CUBE_MAP;
	else
		mode = GL_TEXTURE_2D;
	_S_GL(glBindTexture(mode, texture->tex));
	_S_GL(glTexParameteri(mode, GL_TEXTURE_WRAP_S, wrap));
	_S_GL(glTexParameteri(mode, GL_TEXTURE_WRAP_T, wrap));
	_S_GL(glTexParameteri(mode, GL_TEXTURE_WRAP_R, wrap));
}

void
_S_texture_init(void)
{
	_S_GL(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_units));
	S_debug("GL max combined texture image units: %d\n", max_units);
}

void
_S_texture_attach(const Stexture *texture,
                  Suint32 idx)
{
	if (!texture)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_texture_attach");
		return;
	}
	if (idx >= (Suint32) max_units)
	{
		_S_SET_ERROR(S_INVALID_INDEX, "_S_texture_attach");
		return;
	}
	_S_GL(glActiveTexture(GL_TEXTURE0 + idx));
	if (texture->cubemap)
	{
		_S_GL(glBindTexture(GL_TEXTURE_CUBE_MAP, texture->tex));
	}
	else
	{
		_S_GL(glBindTexture(GL_TEXTURE_2D, texture->tex));
	}
}

