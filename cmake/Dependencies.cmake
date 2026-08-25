include(FetchContent)

set(BUILD_STATIC OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED ON  CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)
find_package(SWIG REQUIRED)

include(contrib/doctest.cmake)
include(contrib/glew.cmake)
include(contrib/lua.cmake)
include(contrib/sdl3.cmake)
include(contrib/imgui.cmake)
include(contrib/openal.cmake)
include(contrib/libxmp.cmake)
include(contrib/sdl3_mixer.cmake)
include(contrib/zlib.cmake)

if(MINGW)
    get_filename_component(MINGW_BIN_DIRECTORY "${CMAKE_CXX_COMPILER}" DIRECTORY)

    set(LIBGCC_DLL    "${MINGW_BIN_DIRECTORY}/libgcc_s_seh-1.dll")
    set(LIBSTDCPP_DLL "${MINGW_BIN_DIRECTORY}/libstdc++-6.dll")

    file(COPY ${LIBGCC_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    file(COPY ${LIBSTDCPP_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    install(FILES ${LIBGCC_DLL} ${LIBSTDCPP_DLL} DESTINATION . COMPONENT core)
endif()
