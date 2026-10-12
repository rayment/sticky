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
%include "video/video.i"
%include "io/mouse.i"
%include "gui/gui.i"
#endif
%include "io/log.i"

// Shift all class namespaces back under the correct `st` namespace to avoid double nesting (e.g. `st.st.lua`)
%luacode %{
    local M = sticky

    M.audio = M.st and M.st.audio
    M.video = M.st and M.st.video
    M.gui = M.st and M.st.gui

    if M.st and M.st.io then
        M.io = M.io or {}
        M.io.mouse = M.st.io.mouse
    end

    M.st = nil

    -- Internals of the bindings that are not part of the API.
    M.string = nil

    -- SWIG gives every class a `__disown` method. Lua code must never release ownership of an object.
    local visited = {}

    local function remove_disown(instance)
        if visited[instance] then
            return
        end
        visited[instance] = true

        if instance['.fn'] then
            instance['.fn'].__disown = nil
        end
        for _, base in ipairs(instance['.bases'] or {}) do
            remove_disown(base)
        end
    end

    local function visit(namespace)
        for _, member in pairs(namespace) do
            if type(member) == 'table' then
                local metatable = getmetatable(member)
                if metatable and metatable['.instance'] then
                    remove_disown(metatable['.instance'])
                elseif not visited[member] then
                    visited[member] = true
                    visit(member)
                end
            end
        end
    end

    visit(M)
%}
