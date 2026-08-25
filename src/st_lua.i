%module sticky

%{
#include <cstdint>
#include <memory>
#include <string>

#include <SDL3/SDL_scancode.h>

#include "st_primitives.h"
%}

%include <stdint.i>
%include <std_string.i>
%include <std_unique_ptr.i>

%include <SDL3/SDL_scancode.h>

%include "st_primitives.h"

%include "math/vec3.i"
%include "audio/client.i"
%include "audio/sound.i"
%include "audio/source.i"
%include "io/keyboard.i"
