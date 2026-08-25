set(SDL_VERSION 3.4.14)

set(SDL3_DISABLE_INSTALL ON)
set(SDL3_DISABLE_UNINSTALL ON)
set(SDL_TEST OFF)
set(SDL_SHARED ON)
set(SDL_STATIC OFF)

string(REPLACE "." ";" SDL_VERSION_LIST ${SDL_VERSION})
list(GET SDL_VERSION_LIST 0 SDL_VERSION_MAJOR)

FetchContent_Declare(
    SDL3
    URL https://github.com/libsdl-org/SDL/releases/download/release-${SDL_VERSION}/SDL${SDL_VERSION_MAJOR}-${SDL_VERSION}.tar.gz
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3)
