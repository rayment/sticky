/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * shader.h
 * GL shader header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_SHADER_H
#define FR_RAYMENT_STICKY_SHADER_H 1

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/math/vec3.h"
#include "sticky/math/vec4.h"
#include "sticky/math/mat3.h"
#include "sticky/math/mat4.h"

typedef struct
Sshader_s
{
	GLuint program;
} Sshader;

Sshader *S_shader_new(const Schar *, Sint64, const Schar *, Sint64);
Sshader *S_shader_load(const Schar *, const Schar *);
void     S_shader_delete(Sshader *);
void     S_shader_attach(const Sshader *);

void     S_shader_set_uniform_float(Sshader *, const Schar *, Sfloat);
void     S_shader_set_uniform_int32(Sshader *, const Schar *, Sint32);
void     S_shader_set_uniform_vec3(Sshader *, const Schar *, const Svec3 *);
void     S_shader_set_uniform_vec4(Sshader *, const Schar *, const Svec4 *);
void     S_shader_set_uniform_mat3(Sshader *, const Schar *, const Smat3 *);
void     S_shader_set_uniform_mat4(Sshader *, const Schar *, const Smat4 *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_SHADER_H */

