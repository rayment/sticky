set(LIBXMP_VERSION 4.7.2)

set(BUILD_SHARED ON CACHE BOOL "" FORCE)
set(BUILD_STATIC OFF CACHE BOOL "" FORCE)
set(BUILD_LITE   ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    libxmp-lite
    URL https://github.com/libxmp/libxmp/archive/refs/tags/libxmp-${LIBXMP_VERSION}.tar.gz
    EXCLUDE_FROM_ALL
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(libxmp-lite)

target_compile_options(xmp_lite_shared PRIVATE -UDEBUG)
target_populate_compiler_features(xmp_lite_shared)
target_populate_linker_features(xmp_lite_shared)
