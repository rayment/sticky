set(ST_LIB_INTERFACES
    st_lua_typemaps.i
    io/log.i
)

set(ST_LIB_SOURCES
    io/filesystem.cpp
    io/log.cpp
    lua/log_bindings.cpp
    lua/runtime.cpp
    os/popup.cpp
    st_init.cpp
    util/config.cpp

    $<$<PLATFORM_ID:Linux>:
        io/filesystem_linux.cpp
        os/popup_linux.c
    >
    $<$<PLATFORM_ID:Windows>:
        io/filesystem_win32.cpp
        os/popup_win32.c
    >
)

set(ST_LIB_PUBLIC_DEPENDENCIES
    # Logging
    fmt::fmt
    spdlog::spdlog
    # Scripting
    lua
    # Math
    glm
    # Config
    nlohmann_json::nlohmann_json
    # Archives
    zlib
)

if(ST_MODULE_VIDEO OR ST_MODULE_AUDIO)
    list(APPEND ST_LIB_PUBLIC_DEPENDENCIES SDL3::SDL3)
endif()

if(ST_MODULE_AUDIO)
    list(APPEND ST_LIB_INTERFACES
        audio/audio.i
    )
endif()

if(ST_MODULE_VIDEO)
    list(APPEND ST_LIB_INTERFACES
        gui/gui.i
        io/mouse.i
        video/video.i
    )
    list(APPEND ST_LIB_SOURCES
        gui/component.cpp
        gui/backend_${ST_GUI_BACKEND}/button.cpp
        gui/backend_${ST_GUI_BACKEND}/core.cpp
        gui/backend_${ST_GUI_BACKEND}/frame.cpp
        gui/backend_${ST_GUI_BACKEND}/rect.cpp
        gui/backend_${ST_GUI_BACKEND}/label.cpp
        gui/backend_${ST_GUI_BACKEND}/layout.cpp
        gui/backend_${ST_GUI_BACKEND}/lineedit.cpp
        gui/backend_${ST_GUI_BACKEND}/menubar.cpp
        gui/backend_${ST_GUI_BACKEND}/popup.cpp
        gui/backend_${ST_GUI_BACKEND}/progress.cpp
        gui/backend_${ST_GUI_BACKEND}/textedit.cpp
        io/gamepad.cpp
        io/keyboard.cpp
        io/mouse.cpp
        video/texture.cpp
        video/texture_handle.cpp
        video/window.cpp
    )
    list(APPEND ST_LIB_PUBLIC_DEPENDENCIES
        glew
        ${OPENGL_LIBRARY}
        imgui
    )
endif()

if(ST_MODULE_AUDIO)
    list(APPEND ST_LIB_SOURCES
        audio/client.cpp
        audio/server.cpp
        audio/sound.cpp
        audio/sound_handle.cpp
        audio/source.cpp
    )
    list(APPEND ST_LIB_PUBLIC_DEPENDENCIES
        OpenAL::OpenAL
        libxmp-lite::xmp_lite_shared
        SDL3_mixer::SDL3_mixer
    )
endif()

set(ST_MODULE_DEFINITIONS
    ST_MODULE_AUDIO=$<BOOL:${ST_MODULE_AUDIO}>
    ST_MODULE_VIDEO=$<BOOL:${ST_MODULE_VIDEO}>
)

include(UseSWIG)
set_property(SOURCE st_lua.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE st_lua.i PROPERTY DEPENDS ${ST_LIB_INTERFACES})
set_property(SOURCE st_lua.i PROPERTY SWIG_FLAGS
    -DST_MODULE_AUDIO=$<BOOL:${ST_MODULE_AUDIO}>
    -DST_MODULE_VIDEO=$<BOOL:${ST_MODULE_VIDEO}>
)
swig_add_library(libsticky_lua TYPE STATIC LANGUAGE lua SOURCES st_lua.i)
set_target_properties(libsticky_lua PROPERTIES
    OUTPUT_NAME sticky_lua
    SWIG_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR};${sdl3_SOURCE_DIR}/include"
)
target_include_directories(libsticky_lua PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(libsticky_lua PRIVATE STATIC_LINKED ${ST_MODULE_DEFINITIONS})
target_link_libraries(libsticky_lua PRIVATE ${ST_LIB_PUBLIC_DEPENDENCIES})

add_library(libsticky)
set_target_properties(libsticky PROPERTIES OUTPUT_NAME sticky)
target_sources(libsticky PRIVATE ${ST_LIB_SOURCES})
target_populate_compiler_features(libsticky)
target_populate_linker_features(libsticky)
target_export_all_symbols(libsticky)
target_enable_coverage(libsticky)
target_populate_compiler_warnings(libsticky)
target_mark_warnings_as_errors(libsticky)
target_disable_exceptions(libsticky)
target_disable_rtti(libsticky)
target_compile_definitions(libsticky PUBLIC ${ST_MODULE_DEFINITIONS})
target_include_directories(libsticky PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(libsticky PUBLIC ${ST_LIB_PUBLIC_DEPENDENCIES} PRIVATE libsticky_lua)
target_link_resource_file(libsticky lib.rc.in)
