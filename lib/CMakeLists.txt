set(ST_LIB_SOURCES
    st_static_init.cpp
    io/log.cpp
    os/popup.cpp
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
    # Archives
    zlib
)

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
target_link_libraries(libsticky PUBLIC ${ST_LIB_PUBLIC_DEPENDENCIES})
