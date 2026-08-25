set(LIBXMP_VERSION 4.7.2)

set(BUILD_LITE ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    libxmp-lite
    URL https://github.com/libxmp/libxmp/archive/refs/tags/libxmp-${LIBXMP_VERSION}.tar.gz
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(libxmp-lite)
