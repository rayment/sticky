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

// Ignore macros that SWIG does not need to handle.
#define ST_DELETE_COPY_CTORS(_)
#define ST_DELETE_MOVE_CTORS(_)

%include "st_lua_typemaps.i"

%include "gui/frame.i"
%include "io/log.i"

// Shift all class namespaces back under the correct `st` namespace to avoid double nesting (e.g. `st.st.lua`)
%luacode %{
    local M = sticky

    M.gui = M.st.gui

    M.st = nil
%}
