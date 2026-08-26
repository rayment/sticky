set(DOCTEST_VERSION 2.5.3)

set(DOCTEST_WITH_MAIN_IN_STATIC_LIB OFF)
set(DOCTEST_WITH_TESTS OFF)
set(DOCTEST_NO_INSTALL ON)
FetchContent_Declare(
    doctest
    URL https://github.com/doctest/doctest/archive/refs/tags/v${DOCTEST_VERSION}.tar.gz
)
FetchContent_MakeAvailable(doctest)
