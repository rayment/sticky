/*
 * This file is licensed under BSD 3-Clause.
 * All license information is available in the included COPYING file.
 */

/*
 * includes.h
 * Library include header.
 *
 * Author       : Finn Rayment <finn@rayment.fr>
 * Date created : 11/04/2021
 */

#ifndef FR_RAYMENT_STICKY_INCLUDES_H
#define FR_RAYMENT_STICKY_INCLUDES_H 1

#include "sticky/common/defines.h"

/* silence macOS deprecation warnings */
#ifdef STICKY_MACOS
#define GL_SILENCE_DEPRECATION 1
#define OPENAL_DEPRECATED
#endif /* STICKY_MACOS */

/* OpenGL */
#include <GL/glew.h>

/* SDL */
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

/* OpenAL */
#ifdef STICKY_MACOS
#include <OpenAL/al.h>
#include <OpenAL/al.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif /* STICKY_MACOS */

/* FreeType */
#include <ft2build.h>
#include FT_FREETYPE_H

#endif /* FR_RAYMENT_STICKY_INCLUDES_H */

