add_executable(sticky_test)

target_sources(sticky_test PRIVATE
    test_main.cpp
)

target_populate_compiler_features(sticky_test)
target_populate_linker_features(sticky_test)
target_mark_warnings_as_errors(sticky_test)
target_disable_exceptions(sticky_test)
target_disable_rtti(sticky_test)
target_include_directories(sticky_test PUBLIC .)

target_link_libraries(sticky_test PRIVATE
    doctest
    libsticky
)

add_test(NAME sticky_test COMMAND sticky_test)
