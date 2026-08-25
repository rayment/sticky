add_executable(sticky_test)

target_sources(sticky_test PRIVATE
    test_main.cpp
    math/test_trig.cpp
    math/test_vec2.cpp
    type/test_string.cpp
)

target_populate_compiler_features(sticky_test)
target_populate_linker_features(sticky_test)
target_include_directories(sticky_test PUBLIC .)

target_link_libraries(sticky_test PRIVATE
    doctest
    libsticky
)
