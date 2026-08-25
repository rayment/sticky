set(glew_VERSION 2.3.1)

set(BUILD_UTILS OFF)

FetchContent_Declare(
    glew
    URL https://github.com/nigels-com/glew/releases/download/glew-${glew_VERSION}/glew-${glew_VERSION}.tgz
)
FetchContent_MakeAvailable(glew)

add_subdirectory(${glew_SOURCE_DIR}/build/cmake ${glew_BINARY_DIR})
target_include_directories(glew PUBLIC $<BUILD_INTERFACE:${glew_SOURCE_DIR}/include>)
