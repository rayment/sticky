# Build options. Override on the command line (-DNAME=VALUE), in a preset, or in cmake-gui.

# Branding of the final game executable.
set(PROGRAM_TITLE "sticky" CACHE STRING "Display title of the program, also used to name the executable")

# GUI backend, selects lib/gui/backend_<name>/.
set(ST_GUI_BACKEND "imgui" CACHE STRING "GUI backend to build libsticky with")
set_property(CACHE ST_GUI_BACKEND PROPERTY STRINGS imgui)

option(ST_BUILD_TESTS     "Build the doctest suite and register it with CTest" ON)
option(ST_ENABLE_DOCS     "Generate Doxygen documentation targets when Doxygen is available" ON)
option(ST_ENABLE_COVERAGE "Instrument lib/ and enable the coverage target" OFF)

if(NOT IS_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/gui/backend_${ST_GUI_BACKEND}")
    message(FATAL_ERROR "Unknown ST_GUI_BACKEND '${ST_GUI_BACKEND}': no lib/gui/backend_${ST_GUI_BACKEND}/")
endif()

# Executable file name derived from the title (libsticky keeps its fixed name).
string(REGEX REPLACE "[^A-Za-z0-9_-]+" "_" PROGRAM_EXECUTABLE "${PROGRAM_TITLE}")
if(PROGRAM_EXECUTABLE STREQUAL "")
    message(FATAL_ERROR "PROGRAM_TITLE must contain at least one alphanumeric character")
endif()
