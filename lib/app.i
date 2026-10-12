%{
#include "lua/app_bindings.h"
%}

%ignore st::lua::app::quit_requested;
%ignore st::lua::app::exit_code;

%include "lua/app_bindings.h"

%luacode {
    local M = sticky

    M.app = M.app or {}

    M.app.quit = M.quit

    M.quit = nil
}
