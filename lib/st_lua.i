%module sticky

%{
#include <string_view>
#include <string>
#include <vector>
%}

%include <std_string_view.i>
%include <std_string.i>
%include <std_vector.i>

%include "st_types.h"

%include "st_lua_typemaps.i"

%include "io/log.i"
