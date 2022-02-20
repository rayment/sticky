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

/**
 * @addtogroup shader
 * @{
 */

typedef struct
Sshader_s
{
	GLuint program;
} Sshader;

/**
 * @brief Create a new shader program.
 *
 * Creates and allocates a new OpenGL shader program from a given source text.
 * The shader takes both a vertex shader for geometry and fragment shader for
 * colour.
 *
 * @param[in] vertex_source The source code for the vertex shader.
 * @param[in] vlen The number of bytes present in the vertex shader source.
 * @param[in] fragment_source The source code for the fragment shader.
 * @param[in] flen The number of bytes present in the fragment shader source.
 * @return A new shader program with two given vertex and fragment shaders.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid source pointer, or a
 * string length of 0 is provided to the function.
 * @since 1.0.0
 */
Sshader *S_shader_new(const Schar *, Sint64, const Schar *, Sint64);

/**
 * @brief Load a shader program from file.
 *
 * @param[in] vertex_file The file path for the vertex shader.
 * @param[in] fragment_file The file path for the fragment shader.
 * @return A new shader program loaded from two given vertex and fragment shader
 * files.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid file path is provided
 * to the function.
 * @since 1.0.0
 */
Sshader *S_shader_load(const Schar *, const Schar *);

/**
 * @brief Free a shader program from memory.
 *
 * Once this function is called for a give shader program, that shader becomes
 * invalid cand may not be used in any other shader or rendering function.
 *
 * @param[in,out] shader The shader to free.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid shader is provided to
 * the function.
 * @since 1.0.0
 */
void     S_shader_delete(Sshader *);

/**
 * @brief Set a single-precision floating-point uniform for a shader.
 *
 * Sets the value of a uniform value that is defined within a shader program in
 * either the vertex or fragment source to a single-precision floating-point
 * value.
 *
 * @warning If an invalid uniform or a uniform of the incorrect type is provided
 * to this function, GL may throw an error which shall crash the program. It is
 * thus important that the correct function is used for a given uniform type.
 * @param[in,out] shader The shader.
 * @param[in] name The name of the uniform to set.
 * @param[in] val The value to set the uniform to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid shader or uniform name
 * is provided to the function.
 * @since 1.0.0
 */
void     S_shader_set_uniform_float(Sshader *, const Schar *, Sfloat);

/**
 * @brief Set a 32-bit signed integer uniform for a shader.
 *
 * Sets the value of a uniform value that is defined within a shader program in
 * either the vertex or fragment source to a 32-bit signed integer value.
 *
 * @warning If an invalid uniform or a uniform of the incorrect type is provided
 * to this function, GL may throw an error which shall crash the program. It is
 * thus important that the correct function is used for a given uniform type.
 * @param[in,out] shader The shader.
 * @param[in] name The name of the uniform to set.
 * @param[in] val The value to set the uniform to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid shader or uniform name
 * is provided to the function.
 * @since 1.0.0
 */
void     S_shader_set_uniform_int32(Sshader *, const Schar *, Sint32);

/**
 * @brief Set a 3D vector uniform for a shader.
 *
 * Sets the value of a uniform value that is defined within a shader program in
 * either the vertex or fragment source to a 3D vector value.
 *
 * @warning If an invalid uniform or a uniform of the incorrect type is provided
 * to this function, GL may throw an error which shall crash the program. It is
 * thus important that the correct function is used for a given uniform type.
 * @param[in,out] shader The shader.
 * @param[in] name The name of the uniform to set.
 * @param[in] val The value to set the uniform to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid shader or uniform name
 * is provided to the function.
 * @since 1.0.0
 */
void     S_shader_set_uniform_vec3(Sshader *, const Schar *, const Svec3 *);

/**
 * @brief Set a 4D vector uniform for a shader.
 *
 * Sets the value of a uniform value that is defined within a shader program in
 * either the vertex or fragment source to a 4D vector value.
 *
 * @warning If an invalid uniform or a uniform of the incorrect type is provided
 * to this function, GL may throw an error which shall crash the program. It is
 * thus important that the correct function is used for a given uniform type.
 * @param[in,out] shader The shader.
 * @param[in] name The name of the uniform to set.
 * @param[in] val The value to set the uniform to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid shader or uniform name
 * is provided to the function.
 * @since 1.0.0
 */
void     S_shader_set_uniform_vec4(Sshader *, const Schar *, const Svec4 *);

/**
 * @brief Set a 3D matrix uniform for a shader.
 *
 * Sets the value of a uniform value that is defined within a shader program in
 * either the vertex or fragment source to a 3D matrix value.
 *
 * @warning If an invalid uniform or a uniform of the incorrect type is provided
 * to this function, GL may throw an error which shall crash the program. It is
 * thus important that the correct function is used for a given uniform type.
 * @param[in,out] shader The shader.
 * @param[in] name The name of the uniform to set.
 * @param[in] val The value to set the uniform to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid shader or uniform name
 * is provided to the function.
 * @since 1.0.0
 */
void     S_shader_set_uniform_mat3(Sshader *, const Schar *, const Smat3 *);

/**
 * @brief Set a 4D matrix uniform for a shader.
 *
 * Sets the value of a uniform value that is defined within a shader program in
 * either the vertex or fragment source to a 4D matrix value.
 *
 * @warning If an invalid uniform or a uniform of the incorrect type is provided
 * to this function, GL may throw an error which shall crash the program. It is
 * thus important that the correct function is used for a given uniform type.
 * @param[in,out] shader The shader.
 * @param[in] name The name of the uniform to set.
 * @param[in] val The value to set the uniform to.
 * @exception S_INVALID_VALUE If a <c>NULL</c> or invalid shader or uniform name
 * is provided to the function.
 * @since 1.0.0
 */
void     S_shader_set_uniform_mat4(Sshader *, const Schar *, const Smat4 *);

void     _S_shader_attach(const Sshader *);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FR_RAYMENT_STICKY_SHADER_H */

