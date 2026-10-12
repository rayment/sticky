%{
#include "lua/resource_bindings.h"
#include "video/texture_handle.h"
%}

// Lua only ever sees named textures loaded during the init phase.
%ignore st::video::texture_handle::texture_handle;

%nspace st::resource_handle<st::video::texture>;
%template(texture_resource_handle) st::resource_handle<st::video::texture>;

%rename(texture) st::video::texture_handle;

%nspace st::video::texture_handle;

// Names are looked up in the registry of the Lua state that makes the call.
%extend st::video::texture_handle {
    static const st::video::texture_handle *get(std::string_view name, lua_State *state)
    {
        return st::lua::find_texture(state, name);
    }
}

%include "video/texture_handle.h"
