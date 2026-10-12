%{
#include "gui/component.h"

#include "gui/button.h"
#include "gui/frame.h"
#include "gui/image.h"
#include "gui/label.h"
#include "gui/progress.h"
%}

%nspace st::gui::component;
%include "gui/component.h"

ST_SWIG_IMPLICIT_CONVERSION_TO_FUNCTION(st::gui::frame::on_close_callback);

%nspace st::gui::frame;
%include "gui/frame.h"

// Lua images are given textures that were loaded during the init phase.
%ignore st::gui::image::set_texture(const std::shared_ptr<st::video::texture> &);
%ignore st::gui::image::load_file;
%nspace st::gui::image;
%include "gui/image.h"

%nspace st::gui::label;
%include "gui/label.h"

%nspace st::gui::progress;
%include "gui/progress.h"

ST_SWIG_IMPLICIT_CONVERSION_TO_FUNCTION(st::gui::button::on_click_callback);

%nspace st::gui::button;
%include "gui/button.h"
