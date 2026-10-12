%module sticky

%{
#include <string_view>
#include <string>
#include <vector>
%}

%include <lua_fnptr.i>
%include <std_string_view.i>
%include <std_string.i>
%include <std_vector.i>
%include <stdint.i>

%include "st_types.h"

// Ignore macros that SWIG does not need to handle.
#define ST_DELETE_COPY_CTORS(_)
#define ST_DELETE_MOVE_CTORS(_)

%include "st_lua_typemaps.i"

// Handles are created by the engine from the init phase tables only.
%ignore st::resource_handle::resource_handle;
%ignore st::resource_handle::shared;
%include "resource/resource_handle.h"

#if ST_MODULE_AUDIO
%include "audio/audio.i"
#endif
#if ST_MODULE_VIDEO
// Video must come first so that gui bindings can refer to its types.
%include "video/video.i"
%include "gui/gui.i"
#endif
%include "io/log.i"

// Shift all class namespaces back under the correct `st` namespace to avoid double nesting (e.g. `st.st.lua`)
%luacode %{
    local M = sticky

    M.audio = M.st and M.st.audio
    M.gui = M.st and M.st.gui
    M.video = M.st and M.st.video

    M.st = nil
%}
