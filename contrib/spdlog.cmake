set(SPDLOG_VERSION 1.17.0)

set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_PIC     ON  CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_SHARED  ON  CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(SPDLOG_ENABLE_PCH    ON CACHE BOOL "" FORCE)
set(SPDLOG_FMT_EXTERNAL  ON CACHE BOOL "" FORCE)
set(SPDLOG_NO_EXCEPTIONS ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    spdlog
    URL https://github.com/gabime/spdlog/archive/refs/tags/v${SPDLOG_VERSION}.tar.gz
)
FetchContent_MakeAvailable(spdlog)

target_populate_compiler_features(spdlog)
target_populate_linker_features(spdlog)
target_disable_exceptions(spdlog)
target_disable_rtti(spdlog)
