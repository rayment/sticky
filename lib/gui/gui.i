%{
#include "gui/component.h"

#include "gui/button.h"
#include "gui/frame.h"
#include "gui/label.h"
#include "gui/layout.h"
#include "gui/popup.h"
#include "gui/progress.h"
#include "gui/rect.h"
%}

// Components are created by Lua and rendered by the engine from show() or their parent. Lua never renders.
%ignore st::gui::component::render;
%ignore st::gui::component::render_shown;
%ignore st::gui::component::id;
%ignore st::gui::component::parent;
%ignore st::gui::nestable_component::children;
%ignore st::gui::frame::render;
%ignore st::gui::button::render;
%ignore st::gui::rect::render;
%ignore st::gui::label::render;
%ignore st::gui::layout::render_group;
%ignore st::gui::hlayout::render;
%ignore st::gui::vlayout::render;
%ignore st::gui::popup::render;
%ignore st::gui::progress::render;

// Showing a component or adding it to a parent pins the Lua object in the registry until the engine lets go of it
// (closed, orphaned or destroyed), so a component that Lua no longer references stays alive and renders.
%ignore st::gui::component::show;
%ignore st::gui::component::set_parent;
%rename(show) st::gui::component::show_pinned;
%rename(set_parent) st::gui::component::set_parent_pinned;

%typemap(in, numinputs=0) lua_State *state {
    $1 = L;
}

%extend st::gui::component {
    void show_pinned(lua_State *state)
    {
        self->show(st_swig_lua_pin(state, 1));
    }

    void set_parent_pinned(st::gui::nestable_component *parent, lua_State *state)
    {
        self->set_parent(parent, st_swig_lua_pin(state, 1));
    }
}

%nspace st::gui::component;
%nspace st::gui::nestable_component;
%nspace st::gui::root_component;
%nspace st::gui::mono_component;
%include "gui/component.h"

ST_SWIG_IMPLICIT_CONVERSION_TO_FUNCTION(st::gui::frame::on_close_callback);

// Lua only sets positions and sizes with numbers.
%ignore st::gui::frame::set_position(glm::f32vec2);
%ignore st::gui::frame::set_size(glm::f32vec2);

%nspace st::gui::frame;
%include "gui/frame.h"

// Lua rects are given textures that were loaded during the init phase.
%ignore st::gui::rect::set_texture(const std::shared_ptr<st::video::texture> &);
%ignore st::gui::rect::load_file;
ST_SWIG_IMPLICIT_CONVERSION_TO_ENUM_FUNCTION(st::gui::rect::on_click_callback, st::io::mouse::button);
%nspace st::gui::rect;
%include "gui/rect.h"

%nspace st::gui::label;
%include "gui/label.h"

%nspace st::gui::layout;
%nspace st::gui::hlayout;
%nspace st::gui::vlayout;
%include "gui/layout.h"

%nspace st::gui::progress;
%include "gui/progress.h"

ST_SWIG_IMPLICIT_CONVERSION_TO_FUNCTION(st::gui::button::on_click_callback);

%nspace st::gui::button;
%include "gui/button.h"

ST_SWIG_IMPLICIT_CONVERSION_TO_FUNCTION(st::gui::popup::on_close_callback);

%nspace st::gui::popup;
%include "gui/popup.h"
