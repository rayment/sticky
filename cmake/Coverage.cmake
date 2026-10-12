function(target_enable_coverage TARGET)
    if(NOT ST_ENABLE_COVERAGE)
        return()
    endif()
    target_compile_options(${TARGET} PRIVATE --coverage)
    target_link_options(${TARGET} PUBLIC --coverage)
endfunction()

if(NOT ST_ENABLE_COVERAGE)
    return()
endif()

if(NOT ST_BUILD_TESTS)
    message(STATUS "ST_BUILD_TESTS is OFF: coverage target disabled")
    return()
endif()

find_program(GCOVR_EXECUTABLE gcovr)
if(NOT GCOVR_EXECUTABLE)
    message(STATUS "gcovr not found: coverage target disabled")
    return()
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    find_program(LLVM_COV_EXECUTABLE llvm-cov REQUIRED)
    set(ST_GCOV_EXECUTABLE "${LLVM_COV_EXECUTABLE} gcov")
else()
    find_program(GCOV_EXECUTABLE gcov REQUIRED)
    set(ST_GCOV_EXECUTABLE "${GCOV_EXECUTABLE}")
endif()

set(ST_COVERAGE_DIR "${CMAKE_BINARY_DIR}/coverage")

add_custom_target(coverage
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${ST_COVERAGE_DIR}"
    COMMAND "${CMAKE_CTEST_COMMAND}" --output-on-failure
    COMMAND "${GCOVR_EXECUTABLE}"
        --root "${CMAKE_SOURCE_DIR}"
        --filter "${CMAKE_SOURCE_DIR}/lib/"
        --object-directory "${CMAKE_BINARY_DIR}"
        --gcov-executable "${ST_GCOV_EXECUTABLE}"
        --html-details "${ST_COVERAGE_DIR}/index.html"
        --print-summary
    DEPENDS sticky_test
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Generating coverage report in ${ST_COVERAGE_DIR}"
    VERBATIM
)
