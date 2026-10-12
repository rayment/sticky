add_executable(sticky_test)

target_sources(sticky_test PRIVATE
    test_main.cpp
    io/test_filesystem.cpp
    io/test_log.cpp
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
    target_sources(sticky_test PRIVATE video/test_window.cpp)
endif()

target_populate_compiler_features(sticky_test)
target_populate_linker_features(sticky_test)
target_mark_warnings_as_errors(sticky_test)
target_disable_exceptions(sticky_test)
target_disable_rtti(sticky_test)
target_include_directories(sticky_test PUBLIC .)
target_compile_definitions(sticky_test PRIVATE
    ST_TEST_RES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/res"
)

target_link_libraries(sticky_test PRIVATE
    doctest
    libsticky
)
target_link_resource_file(sticky_test test.rc.in)

add_test(NAME sticky_test COMMAND sticky_test)
