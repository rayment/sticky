set(FMT_VERSION 12.2.0)

set(FMT_DOC     OFF CACHE BOOL "" FORCE)
set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
set(FMT_TEST    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    fmt
    URL https://github.com/fmtlib/fmt/archive/refs/tags/${FMT_VERSION}.tar.gz
)
FetchContent_MakeAvailable(fmt)

set_target_properties(fmt PROPERTIES PUBLIC_HEADER "")

target_populate_compiler_features(fmt)
target_populate_linker_features(fmt)
target_disable_exceptions(fmt)
target_disable_rtti(fmt)
