%{
#include "audio/sound_handle.h"
#include "lua/resource_bindings.h"
#include "audio/source.h"
%}

// Lua only ever sees named sounds loaded during the init phase.
%ignore st::audio::sound_handle::sound_handle;

%nspace st::resource_handle<st::audio::sound>;
%template(sound_resource_handle) st::resource_handle<st::audio::sound>;

%rename(sound) st::audio::sound_handle;

%nspace st::audio::sound_handle;

// Names are looked up in the registry of the Lua state that makes the call.
%extend st::audio::sound_handle {
    static const st::audio::sound_handle *get(std::string_view name, lua_State *state)
    {
        return st::lua::find_sound(state, name);
    }
}

%include "audio/sound_handle.h"

// Lua passes positions as components and sounds as named handles.
%ignore st::audio::source::set_sound(const std::shared_ptr<st::audio::sound> &);
%ignore st::audio::source::set_position(glm::f64vec3);
%ignore st::audio::source::set_velocity(glm::f64vec3);

%nspace st::audio::source;
%include "audio/source.h"
