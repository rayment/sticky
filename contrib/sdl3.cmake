set(SDL_VERSION 3.4.14)

set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES     OFF CACHE BOOL "" FORCE)
set(SDL_SHARED       ON CACHE BOOL "" FORCE)
set(SDL_STATIC       OFF CACHE BOOL "" FORCE)

set(SDL_OPENGL       ON  CACHE BOOL "" FORCE)
set(SDL_OPENGLES     OFF CACHE BOOL "" FORCE)
set(SDL_DIRECTX      OFF CACHE BOOL "" FORCE)
set(SDL_METAL        OFF CACHE BOOL "" FORCE)
set(SDL_VULKAN       OFF CACHE BOOL "" FORCE)

set(SDL_CAMERA       OFF CACHE BOOL "" FORCE)
set(SDL_DIALOG       OFF CACHE BOOL "" FORCE)
set(SDL_HAPTIC       OFF CACHE BOOL "" FORCE)
set(SDL_HIDAPI       OFF CACHE BOOL "" FORCE)
set(SDL_NOTIFICATION OFF CACHE BOOL "" FORCE)
set(SDL_OFFSCREEN    OFF CACHE BOOL "" FORCE)
set(SDL_POWER        OFF CACHE BOOL "" FORCE)
set(SDL_PROCESS      OFF CACHE BOOL "" FORCE)
set(SDL_RENDER       OFF CACHE BOOL "" FORCE)
set(SDL_SENSOR       OFF CACHE BOOL "" FORCE)
set(SDL_TRAY         OFF CACHE BOOL "" FORCE)

string(REPLACE "." ";" SDL_VERSION_LIST ${SDL_VERSION})
list(GET SDL_VERSION_LIST 0 SDL_VERSION_MAJOR)

FetchContent_Declare(
    SDL3
    URL https://github.com/libsdl-org/SDL/releases/download/release-${SDL_VERSION}/SDL${SDL_VERSION_MAJOR}-${SDL_VERSION}.tar.gz
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3)

target_populate_compiler_features(SDL3-shared)
target_populate_linker_features(SDL3-shared)
target_disable_exceptions(SDL3-shared)
target_disable_rtti(SDL3-shared)
