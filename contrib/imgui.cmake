set(IMGUI_VERSION 1.92.9b)

FetchContent_Declare(
    imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v${IMGUI_VERSION}.tar.gz
)
FetchContent_MakeAvailable(imgui)

find_package(OpenGL REQUIRED)

add_library(imgui SHARED)

set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)

target_sources(
    imgui PRIVATE

    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp

    $<$<OR:$<CONFIG:Debug>,$<CONFIG:DebugWithSanitizer>>:${imgui_SOURCE_DIR}/imgui_demo.cpp>
)

target_include_directories(
    imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(
    imgui PUBLIC
    SDL3::SDL3
    ${OPENGL_LIBRARY}
)

target_populate_compiler_features(imgui)
target_populate_linker_features(imgui)
target_export_all_symbols(imgui)
target_disable_exceptions(imgui)
target_disable_rtti(imgui)
