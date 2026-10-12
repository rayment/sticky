set(ST_EXE_SOURCES
    argparse.cpp
    main.cpp
    modloader.cpp
)

add_executable(sticky)
set_target_properties(sticky PROPERTIES OUTPUT_NAME "${PROGRAM_EXECUTABLE}")
target_sources(sticky PRIVATE ${ST_EXE_SOURCES})
target_compile_definitions(sticky PRIVATE PROGRAM_TITLE="${PROGRAM_TITLE}")
target_populate_compiler_features(sticky)
target_populate_linker_features(sticky)
target_populate_compiler_warnings(sticky)
target_mark_warnings_as_errors(sticky)
#target_disable_exceptions(sticky)
target_disable_rtti(sticky)
target_include_directories(sticky PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sticky PRIVATE libsticky)
target_link_resource_file(sticky app.rc.in)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/mod"
        "$<TARGET_FILE_DIR:sticky>/mod"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/res"
        "$<TARGET_FILE_DIR:sticky>/res"
    VERBATIM
)
