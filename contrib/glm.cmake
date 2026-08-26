set(GLM_VERSION 1.0.3)

set(GLM_BUILD_LIBRARY ON  CACHE BOOL "" FORCE)
set(GLM_BUILD_TESTS   OFF CACHE BOOL "" FORCE)

set(GLM_ENABLE_CXX_17 ON  CACHE BOOL "" FORCE)
set(GLM_ENABLE_CXX_20 OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/archive/refs/tags/${GLM_VERSION}.tar.gz
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(glm)

target_populate_compiler_features(glm)
target_populate_linker_features(glm)
target_disable_exceptions(glm)
target_disable_rtti(glm)
