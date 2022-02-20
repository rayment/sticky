/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * shader.c
 * GL shader source.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2022
 */

#include "sticky/common/error.h"
#include "sticky/common/includes.h"
#include "sticky/common/types.h"
#include "sticky/math/vec3.h"
#include "sticky/math/vec4.h"
#include "sticky/math/mat3.h"
#include "sticky/math/mat4.h"
#include "sticky/memory/allocator.h"
#include "sticky/video/shader.h"

#define ERR_BUF_LEN 512

static
char *
_S_shader_read_file(const Schar *filename,
                    Sint64 *len)
{
	FILE *fp;
	Sint64 size;
	Schar *buf;

	if (!(fp = fopen(filename, "rb")))
	{
		perror("fopen");
		return NULL;
	}
	if (fseek(fp, 0, SEEK_END) != 0)
	{
		perror("fseek");
		return NULL;
	}
	if ((size = ftell(fp)) == -1L)
	{
		perror("ftell");
		return NULL;
	}
	rewind(fp);
	buf = (Schar *) S_memory_new(size + 1);
	if (fread(buf, size, 1, fp) != 1)
	{
		perror("fread");
		return NULL;
	}
	if (fclose(fp) == EOF)
		perror("fclose");
	*len = size;
	return buf;
}

Sshader *
S_shader_new(const Schar *vertex_source,
             Sint64 vlen,
             const Schar *fragment_source,
             Sint64 flen)
{
	Sshader *shader;
	Schar errbuf[ERR_BUF_LEN];
	GLint status;
	GLuint vs, fs, prog;

	if (!vertex_source || !fragment_source || vlen == 0 || flen == 0)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_shader_new");
		return NULL;
	}

	_S_GL(vs = glCreateShader(GL_VERTEX_SHADER));
	_S_GL(fs = glCreateShader(GL_FRAGMENT_SHADER));

	_S_GL(glShaderSource(vs, 1,
	                     (const GLchar **) &vertex_source, (GLint *) &vlen));
	_S_GL(glCompileShader(vs));
	_S_GL(glGetShaderiv(vs, GL_COMPILE_STATUS, &status));
	if (status == GL_FALSE)
	{
		_S_GL(glGetShaderInfoLog(vs, ERR_BUF_LEN, NULL, errbuf));
		_S_error_other("GL", "glCompileShader: %s\n", errbuf);
		exit(EXIT_FAILURE);
	}
	_S_GL(glShaderSource(fs, 1,
	                     (const GLchar **) &fragment_source, (GLint *) &flen));
	_S_GL(glCompileShader(fs));
	_S_GL(glGetShaderiv(fs, GL_COMPILE_STATUS, &status));
	if (status == GL_FALSE)
	{
		_S_GL(glGetShaderInfoLog(fs, ERR_BUF_LEN, NULL, errbuf));
		_S_error_other("GL", "glCompileShader: %s\n", errbuf);
		exit(EXIT_FAILURE);
	}

	if ((prog = glCreateProgram()) == 0)
		_S_error_gl(_S_ERR_LOC);
	_S_GL(glAttachShader(prog, vs));
	_S_GL(glAttachShader(prog, fs));
	_S_GL(glLinkProgram(prog));
	_S_GL(glGetProgramiv(prog, GL_LINK_STATUS, &status));
	if (status == GL_FALSE)
	{
		_S_GL(glGetProgramInfoLog(prog, ERR_BUF_LEN, NULL, errbuf));
		_S_error_other("GL", "glLinkProgram: %s\n", errbuf);
		exit(EXIT_FAILURE);
	}

	_S_GL(glDeleteShader(vs));
	_S_GL(glDeleteShader(fs));

	shader = (Sshader *) S_memory_new(sizeof(Sshader));
	shader->program = prog;

	return shader;
}

Sshader *
S_shader_load(const Schar *vertex_file,
              const Schar *fragment_file)
{
	Schar *vbuf, *fbuf;
	Sint64 vlen, flen;
	Sshader *shader;

	if (!vertex_file || !fragment_file)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_shader_load");
		return NULL;
	}

	_S_CALL("_S_shader_read_file",
	        vbuf = _S_shader_read_file(vertex_file, &vlen));
	_S_CALL("_S_shader_read_file",
	        fbuf = _S_shader_read_file(fragment_file, &flen));
	_S_CALL("S_shader_new", shader = S_shader_new(vbuf, vlen, fbuf, flen));
	S_memory_delete(vbuf);
	S_memory_delete(fbuf);

	return shader;
}

void
S_shader_delete(Sshader *shader)
{
	if (!shader)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_shader_delete");
		return;
	}
	_S_GL(glDeleteProgram(shader->program));
	S_memory_delete(shader);
}

static
GLint
_S_shader_get_uniform(const Sshader *shader,
                      const Schar *name)
{
	GLint uniform;
	if (!shader || !name)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "_S_shader_get_uniform");
		return 0;
	}
	_S_GL(uniform = glGetUniformLocation(shader->program, name));
	return uniform;
}

void
S_shader_set_uniform_float(Sshader *shader,
                           const Schar *name,
                           Sfloat val)
{
	GLint uniform;
	if (!shader || !name)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_shader_set_uniform_float");
		return;
	}
	_S_CALL("_S_shader_get_uniform",
	        uniform = _S_shader_get_uniform(shader, name));
	_S_GL(glUniform1f(uniform, val));
}

void
S_shader_set_uniform_int32(Sshader *shader,
                           const Schar *name,
                           Sint32 val)
{
	GLint uniform;
	if (!shader || !name)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_shader_set_uniform_int32");
		return;
	}
	_S_CALL("_S_shader_get_uniform",
	        uniform = _S_shader_get_uniform(shader, name));
	_S_GL(glUniform1i(uniform, val));
}

void
S_shader_set_uniform_vec3(Sshader *shader,
                          const Schar *name,
                          const Svec3 *val)
{
	GLint uniform;
	if (!shader || !name || !val)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_shader_set_uniform_vec3");
		return;
	}
	_S_CALL("_S_shader_get_uniform",
	        uniform = _S_shader_get_uniform(shader, name));
	_S_GL(glUniform3f(uniform, val->x, val->y, val->z));
}

void
S_shader_set_uniform_vec4(Sshader *shader,
                          const Schar *name,
                          const Svec4 *val)
{
	GLint uniform;
	if (!shader || !name || !val)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_shader_set_uniform_vec4");
		return;
	}
	_S_CALL("_S_shader_get_uniform",
	        uniform = _S_shader_get_uniform(shader, name));
	_S_GL(glUniform4f(uniform, val->x, val->y, val->z, val->w));
}

void
S_shader_set_uniform_mat3(Sshader *shader,
                          const Schar *name,
                          const Smat3 *val)
{
	GLint uniform;
	if (!shader || !name || !val)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_shader_set_uniform_mat3");
		return;
	}
	_S_CALL("_S_shader_get_uniform",
	        uniform = _S_shader_get_uniform(shader, name));
	_S_GL(glUniformMatrix3fv(uniform, 1, GL_FALSE, (Sfloat *) val));
}

void
S_shader_set_uniform_mat4(Sshader *shader,
                          const Schar *name,
                          const Smat4 *val)
{
	GLint uniform;
	if (!shader || !name || !val)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_shader_set_uniform_mat4");
		return;
	}
	_S_CALL("_S_shader_get_uniform",
	        uniform = _S_shader_get_uniform(shader, name));
	_S_GL(glUniformMatrix4fv(uniform, 1, GL_FALSE, (Sfloat *) val));
}

void
_S_shader_attach(const Sshader *shader)
{
	if (!shader)
	{
		_S_SET_ERROR(S_INVALID_VALUE, "S_shader_attach");
		return;
	}
	_S_GL(glUseProgram(shader->program));
}

