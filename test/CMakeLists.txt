add_executable(sticky_test)

target_sources(sticky_test PRIVATE
    test_main.cpp
    io/test_filesystem.cpp
    io/test_log.cpp
    lua/test_runtime.cpp
    util/test_config.cpp
    mod/test_modloader.cpp
    ${PROJECT_SOURCE_DIR}/src/mod.cpp
    ${PROJECT_SOURCE_DIR}/src/modloader.cpp
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
        gui/test_component.cpp
        gui/test_lineedit.cpp
        gui/test_menubar.cpp
        gui/test_popup.cpp
        gui/test_progress.cpp
        gui/test_textedit.cpp
        io/test_mouse.cpp
        mod/test_mod_phases.cpp
        video/test_texture.cpp
        video/test_window.cpp
    )

    # SDL has no public API to inject keyboard/mouse state and its virtual
    # joystick driver is not built, so the library's input sources are built
    # into the test binary with their SDL state queries redirected to the
    # stubs in io/input_stub.h.
    set(ST_INPUT_STUBBED_SOURCES
        ${PROJECT_SOURCE_DIR}/lib/io/gamepad.cpp
        ${PROJECT_SOURCE_DIR}/lib/io/keyboard.cpp
        ${PROJECT_SOURCE_DIR}/lib/io/mouse.cpp
    )
    target_sources(sticky_test PRIVATE ${ST_INPUT_STUBBED_SOURCES})
    set_source_files_properties(${ST_INPUT_STUBBED_SOURCES} PROPERTIES
        COMPILE_OPTIONS "-include;${CMAKE_CURRENT_SOURCE_DIR}/io/sdl_input_redirect.h"
    )
endif()

target_populate_compiler_features(sticky_test)
target_populate_linker_features(sticky_test)
target_populate_compiler_warnings(sticky_test)
target_mark_warnings_as_errors(sticky_test)
target_disable_exceptions(sticky_test)
target_disable_rtti(sticky_test)
target_include_directories(sticky_test PUBLIC . ${PROJECT_SOURCE_DIR}/src)
target_compile_definitions(sticky_test PRIVATE
    ST_TEST_RES_DIR="$<TARGET_FILE_DIR:sticky_test>/test"
)

# Each suite keeps its resources in <suite>/res, copied to test/<suite> next to the test binary. Suites without
# resources still get a folder, which their tests write scratch files to.
set(ST_TEST_RES_COMMANDS)
foreach(suite IN ITEMS audio io lua mod util video)
    set(suite_res "${CMAKE_CURRENT_SOURCE_DIR}/${suite}/res")
    set(suite_out "$<TARGET_FILE_DIR:sticky_test>/test/${suite}")
    list(APPEND ST_TEST_RES_COMMANDS COMMAND ${CMAKE_COMMAND} -E make_directory "${suite_out}")
    if(IS_DIRECTORY "${suite_res}")
        list(APPEND ST_TEST_RES_COMMANDS
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${suite_res}" "${suite_out}")
    endif()
endforeach()
add_custom_target(sticky_test_res ${ST_TEST_RES_COMMANDS} VERBATIM)
add_dependencies(sticky_test sticky_test_res)

target_link_libraries(sticky_test PRIVATE
    doctest
    libsticky
)
target_link_resource_file(sticky_test test.rc.in)

add_test(NAME sticky_test COMMAND sticky_test)
