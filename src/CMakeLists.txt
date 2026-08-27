set(ST_EXE_SOURCES
    main.cpp
)

add_executable(sticky)

target_sources(sticky PRIVATE ${ST_EXE_SOURCES})
target_populate_compiler_features(sticky)
target_populate_linker_features(sticky)
target_populate_compiler_warnings(sticky)
target_mark_warnings_as_errors(sticky)
target_disable_exceptions(sticky)
target_disable_rtti(sticky)
target_link_libraries(sticky PRIVATE libsticky)
