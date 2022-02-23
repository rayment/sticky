/*
 * texture.h
 * OpenGL texture header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 18/04/2021
 */

#ifndef FR_RAYMENT_STICKY_TEXTURE_H
#define FR_RAYMENT_STICKY_TEXTURE_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/includes.h"
#include "sticky/common/types.h"

#define S_TEXTURE_LINEAR  GL_LINEAR
#define S_TEXTURE_NEAREST GL_NEAREST

#define S_TEXTURE_CLAMP   GL_CLAMP_TO_EDGE
#define S_TEXTURE_REPEAT  GL_REPEAT

typedef struct
Stexture_s
{
	GLuint tex;
	Sbool cubemap;
	Senum filter, wrap;
} Stexture;

Stexture *S_texture_load(const Schar *);
Stexture *S_texture_load_cubemap(const Schar *, const Schar *, const Schar *,
                                 const Schar *, const Schar *, const Schar *);

void      S_texture_delete(Stexture *);

void      S_texture_set_filter(Stexture *, Senum);
void      S_texture_set_wrap(Stexture *, Senum);

void     _S_texture_init(void);
void     _S_texture_attach(const Stexture *, Suint32);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_TEXTURE_H */

