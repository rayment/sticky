%{
#include "video/texture_handle.h"
%}

// Lua only ever sees named textures loaded during the init phase.
%ignore st::video::texture_handle::texture_handle;
%ignore st::video::texture_handle::set_registry;

%nspace st::resource_handle<st::video::texture>;
%template(texture_resource_handle) st::resource_handle<st::video::texture>;

%rename(texture) st::video::texture_handle;

%nspace st::video::texture_handle;
%include "video/texture_handle.h"
