include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckIPOSupported)

if(WIN32)
    if(NOT MINGW)
        message(FATAL_ERROR "Windows builds must use a MinGW compatible build environment!")
    endif()
endif()

if(CMAKE_SIZEOF_VOID_P LESS 8)
    message(FATAL_ERROR "Only 64-bit builds are supported!")
endif()

check_c_compiler_flag("-fno-omit-frame-pointer"         HAVE_FNO_OMIT_FRAME_POINTER)
check_c_compiler_flag("-fstack-clash-protection"        HAVE_FSTACK_CLASH_PROTECTION)
check_c_compiler_flag("-fstack-protector-strong"        HAVE_FSTACK_PROTECTOR_STRONG)
check_c_compiler_flag("-fstrict-flex-arrays=3"          HAVE_FSTRICT_FLEX_ARRAYS_3)
check_c_compiler_flag("-fcf-protection=full"            HAVE_FCF_PROTECTION_FULL)
check_c_compiler_flag("-fno-delete-null-pointer-checks" HAVE_FNO_DELETE_NULL_POINTER_CHECKS)
check_c_compiler_flag("-ftrivial-auto-var-init=zero"    HAVE_FTRIVIAL_AUTO_VAR_INIT_ZERO)
check_cxx_compiler_flag("-fno-exceptions"               HAVE_FNO_EXCEPTIONS)
check_cxx_compiler_flag("-fno-rtti"                     HAVE_FNO_RTTI)

# Expose project meta variables to compiler.
add_compile_definitions(
    PROGRAM_TITLE="${PROGRAM_TITLE}"
    VERSION_MAJOR="${VERSION_MAJOR}"
    VERSION_MINOR="${VERSION_MINOR}"
    VERSION_PATCH="${VERSION_PATCH}"
    VERSION_TAG="${VERSION_TAG}"
    VERSION_STRING="${VERSION_STRING}"
    COPYRIGHT_STRING="${COPYRIGHT_STRING}"
    COPYRIGHT_OWNER="${COPYRIGHT_OWNER}"
)

if (NOT CMAKE_C_COMPILER_ID MATCHES "^(GNU|Clang)$")
    message(FATAL_ERROR
        "Unsupported C compiler: ${CMAKE_C_COMPILER_ID} "
        "(${CMAKE_C_COMPILER})"
    )
endif()

function(target_populate_compiler_features TARGET)
    check_ipo_supported(
        RESULT LTO_SUPPORTED
        OUTPUT LTO_ERROR
        LANGUAGES C CXX
    )

    target_compile_options(${TARGET} PRIVATE
        # Security/hardening
        $<$<BOOL:${HAVE_FSTACK_CLASH_PROTECTION}>:-fstack-clash-protection> # better stack overflow detection
        $<$<BOOL:${HAVE_FSTACK_PROTECTOR_STRONG}>:-fstack-protector-strong> # prevent stack smashing
        $<$<BOOL:${HAVE_FSTRICT_FLEX_ARRAYS_3}>:-fstrict-flex-arrays=3> # strict array members
        $<$<BOOL:${HAVE_FCF_PROTECTION_FULL}>:-fcf-protection=full> # control flow integrity protection
        $<$<BOOL:${HAVE_FNO_DELETE_NULL_POINTER_CHECKS}>:-fno-delete-null-pointer-checks>  # prevent optimiser from removing null checks

        $<$<OR:$<CONFIG:Debug>,$<CONFIG:DebugWithSanitizer>>:
            -O0 # disable all optimisations
            -g3 # enable highest debug level
            $<$<BOOL:${HAVE_FNO_OMIT_FRAME_POINTER}>:-fno-omit-frame-pointer> # do not omit frame pointers for debugging
        >

        $<$<CONFIG:DebugWithSanitizer>:
            -fsanitize=address,undefined # enable address/UB sanitisers
        >

        $<$<CONFIG:RelWithDebInfo>:
            -O2 # enable some optimisations
            -g3 # enable highest debug level
            $<$<BOOL:${HAVE_FNO_OMIT_FRAME_POINTER}>:-fno-omit-frame-pointer> # do not omit frame pointers for debugging
        >

        $<$<CONFIG:Release>:
            -O3 # enable all optimisations
            $<$<BOOL:${LTO_SUPPORTED}>:-flto> # enable link-time optimisations
            $<$<BOOL:${HAVE_FTRIVIAL_AUTO_VAR_INIT_ZERO}>:-ftrivial-auto-var-init=zero> # zero-initialise trivial variables
        >
    )
    target_compile_definitions(${TARGET} PRIVATE
        _FORTIFY_SOURCE=3 # aggressive libc hardening

        $<$<OR:$<CONFIG:Debug>,$<CONFIG:DebugWithSanitizer>>:
            STICKY_DEBUG=1
        >

        $<$<CONFIG:RelWithDebInfo>:
            STICKY_DEBUG=0
        >

        $<$<CONFIG:Release>:
            STICKY_DEBUG=0
            NDEBUG=1 # disable assertions
        >

        $<$<PLATFORM_ID:Windows>:
            WIN32_LEAN_AND_MEAN=1 # Do not import crap from windows.h.
        >
    )

    set_target_properties(${TARGET} PROPERTIES
        $<$<PLATFORM_ID:Windows>:
            # Prevent console from spawning in release builds.
            WIN32_EXECUTABLE $<NOT:$<CONFIG:Debug,DebugWithSanitizer>>
        >
    )

    get_target_property(TARGET_TYPE ${TARGET} TYPE)
    if(TARGET_TYPE STREQUAL "EXECUTABLE")
        # Executable-specific features.
        #set_target_properties(${TARGET} PROPERTIES
        #)
    endif()
    if(TARGET_TYPE MATCHES "_LIBRARY$")
        # Library-specific features.
        set_target_properties(${TARGET} PROPERTIES
            POSITION_INDEPENDENT_CODE ON
        )
    endif()
endfunction()

function(target_populate_compiler_warnings TARGET)
    target_compile_options(${TARGET} PRIVATE
        -Wpedantic
        -Wall
        -Wextra
        -Wconversion         # warn on implicit conversions
        -Wsign-conversion    # warn on implicit sign changes
        -Wshadow             # warn on variable shadowing
        -Wformat=2           # aggressively check format strings
        -Wnull-dereference   # warn on obvious null pointer usage
        -Wdouble-promotion   # warn on implicit float to double conversion
        $<$<COMPILE_LANGUAGE:C>:
            -Wstrict-prototypes  # warn on functions without explicit parameters
            -Wmissing-prototypes # enforce prototypes for every function
        >
        $<$<COMPILE_LANGUAGE:CXX>:
            -Wnon-virtual-dtor       # warn when a class with virtual functions has a non-virtual destructor
            -Woverloaded-virtual     # warn when a derived class function hides a virtual from a base class
            -Wold-style-cast         # warn on C-style casts (use static_cast/reinterpret_cast etc.)
            -Wzero-as-null-pointer-constant  # warn on using 0 instead of nullptr
            -Wextra-semi             # warn on redundant semicolons (Clang only, GCC ignores it)
            -Wuseless-cast           # warn on casts to the same type (GCC only)
            -Wsuggest-override       # warn on overriding virtual functions without the override specifier
        >
        -Wundef              # warn on undefined macro checks without defined(...) wrapping
    )
endfunction()

function(target_disable_exceptions TARGET)
    target_compile_options(${TARGET} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:
            $<$<BOOL:${HAVE_FNO_EXCEPTIONS}>:-fno-exceptions> # disable C++ exceptions
        >
    )
endfunction()

function(target_disable_rtti TARGET)
    target_compile_options(${TARGET} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:
            $<$<BOOL:${HAVE_FNO_RTTI}>:-fno-rtti> # disable C++ RTTI
        >
    )
endfunction()

function(target_mark_warnings_as_errors TARGET)
    target_compile_options(${TARGET} PRIVATE -Werror)
endfunction()
