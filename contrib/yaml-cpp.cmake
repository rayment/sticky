set(YAML_CPP_VERSION 0.9.0)

set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS   OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL       OFF CACHE BOOL "" FORCE)
set(YAML_BUILD_SHARED_LIBS ON  CACHE BOOL "" FORCE)

FetchContent_Declare(
    yaml-cpp
    URL https://github.com/jbeder/yaml-cpp/archive/refs/tags/yaml-cpp-${YAML_CPP_VERSION}.tar.gz
)
FetchContent_MakeAvailable(yaml-cpp)

target_populate_compiler_features(yaml-cpp)
target_populate_linker_features(yaml-cpp)
target_disable_rtti(yaml-cpp)

add_compile_definitions(YAML_CPP_STATIC_DEFINE=1)
