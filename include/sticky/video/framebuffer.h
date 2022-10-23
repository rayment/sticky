/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * framebuffer.h
 * Video framebuffer header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 20/10/2022
 */

#ifndef FR_RAYMENT_STICKY_FRAMEBUFFER_H
#define FR_RAYMENT_STICKY_FRAMEBUFFER_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/defines.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"

#define S_COLOR_BUFFER          0x01
#define S_DEPTH_BUFFER          0x02
#define S_DEPTH_STENCIL_BUFFER  0x04

typedef struct
Sframebuffer_s
{
	Sbool set_color, set_depth, set_depthstencil;
	Suint8 bufset;
	GLuint fbo, tex_color, rb_depth, rb_depthstencil;
} Sframebuffer;

STICKY_API Sframebuffer *S_framebuffer_new(Suint16, Suint16, Suint8);

STICKY_API void          S_framebuffer_delete(Sframebuffer *);

STICKY_API void          S_framebuffer_resize(Sframebuffer *, Suint16, Suint16);

STICKY_API void          S_framebuffer_clear(Sframebuffer *);

STICKY_API void _S_framebuffer_attach(Sframebuffer *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_FRAMEBUFFER_H */

