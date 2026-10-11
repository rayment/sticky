set(ST_LIB_INTERFACES
    st_lua_typemaps.i
    io/filesystem.i
    io/log.i
)

set(ST_LIB_SOURCES
    audio/client.cpp
    audio/server.cpp
    audio/sound.cpp
    audio/source.cpp
    io/filesystem.cpp
    io/gamepad.cpp
    io/keyboard.cpp
    io/log.cpp
    io/mouse.cpp
    lua/log_bindings.cpp
    lua/runtime.cpp
    os/popup.cpp
    video/window.cpp
    st_init.cpp

    $<$<PLATFORM_ID:Linux>:
        os/popup_linux.c
    >
    $<$<PLATFORM_ID:Windows>:
        os/popup_win32.c
    >
)

set(ST_LIB_PUBLIC_DEPENDENCIES
    # Logging
    fmt::fmt
    spdlog::spdlog
    # Graphics
    SDL3::SDL3
    glew
    ${OPENGL_LIBRARY}
    # GUI
    imgui
    # Scripting
    lua
    # Audio
    OpenAL::OpenAL
    libxmp-lite::xmp_lite_shared
    SDL3_mixer::SDL3_mixer
    # Math
    glm
    # Config
    nlohmann_json::nlohmann_json
    # Archives
    zlib
)

include(UseSWIG)
set_property(SOURCE st_lua.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE st_lua.i PROPERTY DEPENDS ${ST_LIB_INTERFACES})
swig_add_library(libsticky_lua TYPE STATIC LANGUAGE lua SOURCES st_lua.i)
set_target_properties(libsticky_lua PROPERTIES
    OUTPUT_NAME sticky_lua
    SWIG_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR};${sdl3_SOURCE_DIR}/include"
)
target_include_directories(libsticky_lua PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(libsticky_lua PRIVATE STATIC_LINKED)
target_link_libraries(libsticky_lua PRIVATE ${ST_LIB_PUBLIC_DEPENDENCIES})

add_library(libsticky)
set_target_properties(libsticky PROPERTIES OUTPUT_NAME sticky)
target_sources(libsticky PRIVATE ${ST_LIB_SOURCES})
target_populate_compiler_features(libsticky)
target_populate_linker_features(libsticky)
target_populate_compiler_warnings(libsticky)
target_mark_warnings_as_errors(libsticky)
target_disable_exceptions(libsticky)
target_disable_rtti(libsticky)
target_include_directories(libsticky PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libsticky PUBLIC ${ST_LIB_PUBLIC_DEPENDENCIES} PRIVATE libsticky_lua)
target_link_resource_file(libsticky lib.rc.in)
