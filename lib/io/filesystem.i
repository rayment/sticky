%{
#include "io/filesystem.h"
%}

%include "io/filesystem.h"

%luacode {
    local M = sticky

    M.io = M.io or {}
    M.io.filesystem = M.io.filesystem or {}

    M.io.filesystem.exists       = M.exists
    M.io.filesystem.is_directory = M.is_directory
    M.io.filesystem.is_file      = M.is_file
    M.io.filesystem.basename     = M.basename
    M.io.filesystem.dirname      = M.dirname
    M.io.filesystem.extension    = M.extension
    M.io.filesystem.stem         = M.stem
    M.io.filesystem.list_all     = M.list_all
    M.io.filesystem.list_files   = M.list_files
    M.io.filesystem.list_subdirs = M.list_subdirs

    M.exists       = nil
    M.is_directory = nil
    M.is_file      = nil
    M.basename     = nil
    M.dirname      = nil
    M.extension    = nil
    M.stem         = nil
    M.list_all     = nil
    M.list_files   = nil
    M.list_subdirs = nil
}
