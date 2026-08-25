cmake_minimum_required(VERSION 4.2)

project(sticky LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROGRAM_TITLE "sticky")
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)
set(VERSION_TAG "alpha")
set(COPYRIGHT_OWNER "Finn Rayment")
set(COPYRIGHT_FROM "2025")

# Detect current year for copyright.
string(TIMESTAMP COPYRIGHT_TO "%Y")
if(COPYRIGHT_FROM STREQUAL COPYRIGHT_TO)
    set(COPYRIGHT_STRING "${COPYRIGHT_FROM}")
else()
    set(COPYRIGHT_STRING "${COPYRIGHT_FROM}-${COPYRIGHT_TO}")
endif()

# Create full version string.
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
if(NOT VERSION_TAG STREQUAL "")
    set(VERSION_STRING "${VERSION_STRING}-${VERSION_TAG}")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include(cmake/Compiler.cmake)
include(cmake/Linker.cmake)
include(cmake/Dependencies.cmake)

add_subdirectory(src)
add_subdirectory(test)
