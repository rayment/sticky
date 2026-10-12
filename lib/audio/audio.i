%{
#include "audio/sound_handle.h"
#include "audio/source.h"
%}

// Lua only ever sees named sounds loaded during the init phase.
%ignore st::audio::sound_handle::sound_handle;
%ignore st::audio::sound_handle::set_registry;

%nspace st::resource_handle<st::audio::sound>;
%template(sound_resource_handle) st::resource_handle<st::audio::sound>;

%rename(sound) st::audio::sound_handle;

%nspace st::audio::sound_handle;
%include "audio/sound_handle.h"

// Lua passes positions as components and sounds as named handles.
%ignore st::audio::source::select(const st::audio::sound &);
%ignore st::audio::source::set_position(glm::f64vec3);
%ignore st::audio::source::set_velocity(glm::f64vec3);

%nspace st::audio::source;
%include "audio/source.h"
