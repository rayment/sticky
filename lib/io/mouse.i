%{
#include "io/mouse.h"
%}

// Lua only needs the button enumerators, which are passed to the callbacks of the components.
%ignore st::io::mouse::delta_pos;
%ignore st::io::mouse::global_pos;
%ignore st::io::mouse::window_pos;
%ignore st::io::mouse::button_down;
%ignore st::io::mouse::button_up;

%nspace st::io::mouse::button;
%include "io/mouse.h"
