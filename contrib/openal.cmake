set(OPENAL_VERSION 1.25.2)

set(ALSOFT_UTILS          OFF CACHE BOOL "" FORCE)
set(ALSOFT_NO_CONFIG_UTIL ON  CACHE BOOL "" FORCE)
set(ALSOFT_EXAMPLES       OFF CACHE BOOL "" FORCE)
set(ALSOFT_TESTS          OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    openal
    URL https://github.com/kcat/openal-soft/archive/refs/tags/${OPENAL_VERSION}.tar.gz
)
FetchContent_MakeAvailable(openal)
