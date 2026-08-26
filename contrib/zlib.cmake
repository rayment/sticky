set(ZLIB_VERSION 1.3.2)

set(ZLIB_INSTALL       OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_SHARED  ON  CACHE BOOL "" FORCE)
set(ZLIB_BUILD_STATIC  OFF CACHE BOOL "" FORCE)
set(ZLIB_BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    zlib
    URL https://github.com/madler/zlib/archive/refs/tags/v${ZLIB_VERSION}.zip
)
FetchContent_MakeAvailable(zlib)

target_populate_compiler_features(zlib)
target_populate_linker_features(zlib)

#get_target_property(_ZLIB_INCLUDES zlib INTERFACE_INCLUDE_DIRECTORIES)
#set(ZLIB_FOUND TRUE CACHE BOOL "" FORCE)
#set(ZLIB_INCLUDE_DIR "${_ZLIB_INCLUDES}" CACHE PATH "" FORCE)
#set(ZLIB_LIBRARY "$<TARGET_FILE:zlib>" CACHE STRING "" FORCE)

#install(TARGETS zlib RUNTIME DESTINATION . COMPONENT core)
