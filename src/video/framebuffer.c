/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * framebuffer.c
 * Video framebuffer source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 20/10/2022
 */

#include "sticky/common/error.h"
#include "sticky/common/includes.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/framebuffer.h"

static
GLuint
_S_framebuffer_gen_texture(Suint16 width,
                           Suint16 height)
{
	GLuint tex;
	_S_GL(glGenTextures(1, &tex));
	_S_GL(glBindTexture(GL_TEXTURE_2D, tex));
	_S_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
	                   GL_RGB, GL_UNSIGNED_BYTE, NULL));
	_S_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	_S_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	return tex;
}

static
GLuint
_S_framebuffer_gen_renderbuffer(Suint16 width,
                                Suint16 height,
								Sbool stencil)
{
	GLuint rb;
	GLint format;
	if (stencil)
		format = GL_DEPTH24_STENCIL8;
	else
		format = GL_DEPTH_COMPONENT24;
	_S_GL(glGenRenderbuffers(1, &rb));
	_S_GL(glBindRenderbuffer(GL_RENDERBUFFER, rb));
	_S_GL(glRenderbufferStorage(GL_RENDERBUFFER, format, width, height));
	return rb;
}

static
void
_S_framebuffer_reset(Suint16 width,
                     Suint16 height,
					 Suint8 buffers,
					 Sframebuffer *buf,
					 Sbool firstpass)
{
	if (!firstpass)
	{
		/* unbind the framebuffer */
		_S_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		/* if the buffer is already in memory, we need to delete it */
		if (buf->set_color)
		{
			_S_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				                         GL_TEXTURE_2D, 0, 0));
			_S_GL(glDeleteTextures(1, &(buf->tex_color)));
		}
		if (buf->set_depth)
		{
			_S_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				                            GL_RENDERBUFFER, 0));
			_S_GL(glDeleteRenderbuffers(1, &(buf->rb_depth)));
		}
		if (buf->set_depthstencil)
		{
			_S_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER,
				                            GL_DEPTH_STENCIL_ATTACHMENT,
					                        GL_RENDERBUFFER, 0));
			_S_GL(glDeleteRenderbuffers(1, &(buf->rb_depthstencil)));
		}
		/* texture/renderbuffer attachments are gone, now delete the frame */
		_S_GL(glDeleteFramebuffers(1, &(buf->fbo)));
	}

	buf->set_color = S_FALSE;
	buf->set_depth = S_FALSE;
	buf->set_depthstencil = S_FALSE;

	_S_GL(glGenFramebuffers(1, &(buf->fbo)));
	_S_GL(glBindFramebuffer(GL_FRAMEBUFFER, buf->fbo));
	if ((buffers & S_COLOR_BUFFER) != 0)
	{
		buf->set_color = S_TRUE;
		_S_CALL("_S_framebuffer_gen_texture",
		        buf->tex_color =
				    _S_framebuffer_gen_texture(width, height));
		_S_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		                             GL_TEXTURE_2D, buf->tex_color, 0));
	}
	if ((buffers & S_DEPTH_BUFFER) != 0)
	{
		buf->set_depth = S_TRUE;
		_S_CALL("_S_framebuffer_gen_renderbuffer",
		        buf->rb_depth =
				    _S_framebuffer_gen_renderbuffer(width, height, S_FALSE));
		_S_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		                                GL_RENDERBUFFER, buf->rb_depth));
	}
	if ((buffers & S_DEPTH_STENCIL_BUFFER) != 0)
	{
		buf->set_depthstencil = S_TRUE;
		_S_CALL("_S_framebuffer_gen_renderbuffer",
		        buf->rb_depthstencil =
				    _S_framebuffer_gen_renderbuffer(width, height, S_TRUE));
		_S_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER,
		                                GL_DEPTH_STENCIL_ATTACHMENT,
		                                GL_RENDERBUFFER, buf->rb_depthstencil));
	}
}

Sframebuffer *
S_framebuffer_new(Suint16 width,
                  Suint16 height,
				  Suint8 buffers)
{
	Sframebuffer *buf;
	Sbool b;

	if (((buffers & S_COLOR_BUFFER) == 0 &&
	    (buffers & S_DEPTH_BUFFER) == 0 &&
		(buffers & S_DEPTH_STENCIL_BUFFER) == 0) ||
		((buffers & S_DEPTH_BUFFER) != 0 &&
		(buffers & S_DEPTH_STENCIL_BUFFER) != 0))
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_framebuffer_new");
		return NULL;
	}

	buf = (Sframebuffer *) S_memory_new(sizeof(Sframebuffer));
	buf->bufset = buffers;
	_S_CALL("_S_framebuffer_reset",
	        _S_framebuffer_reset(width, height, buffers, buf, S_TRUE));
	/* check framebuffer validity */
	_S_GL(b =
	      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	_S_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	if (!b)
	{
		/* framebuffer is not complete for some reason */
		_S_SET_ERROR(S_UNKNOWN_ERROR, "S_framebuffer_new");
		return NULL;
	}

	return buf;
}

void
S_framebuffer_delete(Sframebuffer *buf)
{
	if (!buf)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_framebuffer_delete");
		return;
	}
	if (buf->set_color)
	{
		_S_GL(glDeleteTextures(1, &(buf->tex_color)));
	}
	if (buf->set_depth)
	{
		_S_GL(glDeleteRenderbuffers(1, &(buf->rb_depth)));
	}
	if (buf->set_depthstencil)
	{
		_S_GL(glDeleteRenderbuffers(1, &(buf->rb_depthstencil)));
	}
	_S_GL(glDeleteFramebuffers(1, &(buf->fbo)));
	S_memory_delete(buf);
}

void
S_framebuffer_resize(Sframebuffer *buf,
                     Suint16 width,
					 Suint16 height)
{
	if (!buf)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_framebuffer_resize");
	}
	else
	{
		_S_CALL("_S_framebuffer_reset",
		        _S_framebuffer_reset(width, height, buf->bufset, buf, S_FALSE));
		_S_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
}

void
S_framebuffer_clear(Sframebuffer *buf)
{
	if (!buf)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_framebuffer_clear");
		return;
	}
	_S_GL(glBindFramebuffer(GL_FRAMEBUFFER, buf->fbo));
	_S_GL(glClear(GL_COLOR_BUFFER_BIT));
	_S_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void
_S_framebuffer_attach(Sframebuffer *buf)
{
	GLuint fb;
	if (!buf)
		fb = 0;
	else
		fb = buf->fbo;
	_S_GL(glBindFramebuffer(GL_FRAMEBUFFER, fb));
}

