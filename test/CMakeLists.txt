add_executable(sticky_test)

target_sources(sticky_test PRIVATE
    test_main.cpp
    io/test_filesystem.cpp
    io/test_log.cpp
    lua/test_runtime.cpp
    util/test_config.cpp
    ${PROJECT_SOURCE_DIR}/src/mod.cpp
)

if(ST_MODULE_AUDIO)
    target_sources(sticky_test PRIVATE
        audio/test_client.cpp
        audio/test_server.cpp
        audio/test_sound.cpp
        audio/test_source.cpp
    )
endif()

if(ST_MODULE_VIDEO)
    target_sources(sticky_test PRIVATE
        mod/test_mod_phases.cpp
        video/test_texture.cpp
        video/test_window.cpp
    )
endif()

target_populate_compiler_features(sticky_test)
target_populate_linker_features(sticky_test)
target_mark_warnings_as_errors(sticky_test)
target_disable_exceptions(sticky_test)
target_disable_rtti(sticky_test)
target_include_directories(sticky_test PUBLIC . ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(sticky_test PRIVATE
    ST_TEST_RES_DIR="$<TARGET_FILE_DIR:sticky_test>/test"
)

add_custom_target(sticky_test_res
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:sticky_test>/test"
)
add_dependencies(sticky_test sticky_test_res)

target_link_libraries(sticky_test PRIVATE
    doctest
    libsticky
)
target_link_resource_file(sticky_test test.rc.in)

add_test(NAME sticky_test COMMAND sticky_test)
