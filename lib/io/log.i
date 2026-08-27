%{
#include "lua/log_bindings.h"
%}

%ignore st::lua::logger;
%ignore st::lua::set_logger;

%include "lua/log_bindings.h"

%luacode {
    local M = sticky

    M.io = M.io or {}
    M.io.log = M.io.log or {}

    M.io.log.trace = M.trace
    M.io.log.debug = M.debug
    M.io.log.info = M.info
    M.io.log.warn = M.warn
    M.io.log.error = M.error

    M.trace = nil
    M.debug = nil
    M.info = nil
    M.warn = nil
    M.error = nil
}
