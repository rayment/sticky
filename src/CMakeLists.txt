include(UseSWIG)

set(ST_LUA_INTERFACES
    audio/client.i
    audio/sound.i
    audio/source.i
    io/keyboard.i
    math/vec3.i
)

set(ST_LIB_DEPENDENCIES
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
    # Archives
    zlib
)

set_property(
    SOURCE st_lua.i
    PROPERTY CPLUSPLUS ON
)

set_property(
    SOURCE st_lua.i
    PROPERTY DEPENDS ${ST_LUA_INTERFACES}
)

swig_add_library(libsticky_lua
    TYPE STATIC
    LANGUAGE lua
    SOURCES st_lua.i
)

set_target_properties(libsticky_lua PROPERTIES
    OUTPUT_NAME sticky_lua
    SWIG_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR};${sdl3_SOURCE_DIR}/include"
)

#target_populate_compiler_features(libsticky_lua)
#target_populate_linker_features(libsticky_lua)
target_include_directories(libsticky_lua PRIVATE .)
target_compile_definitions(libsticky_lua PRIVATE STATIC_LINKED)

target_link_libraries(libsticky_lua PRIVATE ${ST_LIB_DEPENDENCIES})

add_library(libsticky)

set_target_properties(libsticky PROPERTIES
    OUTPUT_NAME sticky
)

target_sources(libsticky PRIVATE
    audio/client.c
    audio/server.c
    audio/sound.c
    audio/source.c
    gui/backend/imgui/gui_imgui.cpp
    gui/button.c
    gui/widget.c
    io/gamepad.c
    io/keyboard.c
    io/mouse.c
    log/log.c
    lua/lua.c
    #log/rotate.c
    os/popup.c
    type/string.c
#    video/renderer.c
    video/window.c
)

target_populate_compiler_features(libsticky)
target_populate_linker_features(libsticky)
target_include_directories(libsticky PUBLIC .)

target_link_libraries(libsticky PUBLIC ${ST_LIB_DEPENDENCIES} PRIVATE libsticky_lua)

add_executable(sticky)

target_sources(sticky PRIVATE
    main.c
)

target_populate_compiler_features(sticky)
target_populate_linker_features(sticky)
target_link_libraries(sticky PRIVATE libsticky)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/mod"
        "$<TARGET_FILE_DIR:sticky>/mod"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:sticky>/res"
    VERBATIM
)
