find_package(Doxygen OPTIONAL_COMPONENTS dot)
if(NOT DOXYGEN_FOUND)
    message(STATUS "Doxygen not found: documentation targets disabled")
    return()
endif()

if(DOXYGEN_DOT_FOUND)
    set(ST_DOXYGEN_HAVE_DOT YES)
    get_filename_component(ST_DOXYGEN_DOT_PATH "${DOXYGEN_DOT_EXECUTABLE}" DIRECTORY)
else()
    set(ST_DOXYGEN_HAVE_DOT NO)
    set(ST_DOXYGEN_DOT_PATH "")
endif()

function(add_doxygen_target name dir)
    set(out "${CMAKE_BINARY_DIR}/doc/${name}")
    configure_file("${CMAKE_SOURCE_DIR}/doc/${dir}/Doxyfile.in" "${out}/Doxyfile" @ONLY)
    add_custom_target(doc-${name}
        COMMAND "${DOXYGEN_EXECUTABLE}" Doxyfile
        WORKING_DIRECTORY "${out}"
        COMMENT "Generating ${name} documentation"
        VERBATIM
    )
endfunction()

add_doxygen_target(cpp cpp)
add_doxygen_target(lua lua)

add_custom_target(doc DEPENDS doc-cpp doc-lua)
