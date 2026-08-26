set(ST_LIB_DEPENDENCIES
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

add_executable(sticky)

target_sources(sticky PRIVATE
    main.cpp
)

target_populate_compiler_features(sticky)
target_populate_linker_features(sticky)
target_populate_compiler_warnings(sticky)
target_mark_warnings_as_errors(sticky)
target_disable_exceptions(sticky)
target_disable_rtti(sticky)
#target_link_libraries(sticky PRIVATE libsticky)
target_link_libraries(sticky PRIVATE ${ST_LIB_DEPENDENCIES})
