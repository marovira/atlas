#================================
# Compiler Configurations file.
#================================

# First identify the compiler we're using.
if (MSVC)
    if ((MSVC_VERSION EQUAL 1800) OR
        (MSVC_VERSION EQUAL 1900))
        set(ATLAS_COMPILER_MSVC 1)
        message(STATUS "Using MSVC ${MSVC_VERSION}...")
    else()
        message(FATAL_ERROR "Atlas requires Visual Studio 12 or 14 to run.")
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "^(Apple)?Clang$")
        set(ATLAS_COMPILER_CLANG 1)
        message(STATUS "Using Clang...")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(ATLAS_COMPILER_INTEL 1)
    message(STATUS "Using icc...")
else()
    set(ATLAS_COMPILER_GNU 1)
    message(STATUS "Using GNU...")
endif()

# Now setup the flags depending on the compiler we're using.
if (ATLAS_COMPILER_MSVC)
    add_definitions(
        -wd4714 # force inline not inlined.
        -wd4510 # default constructor could not be generated.
        -wd4512 # assignment operator could not be generated.
        -wd4610 # nonstandard extension used.
        -wd4316 # object allocated on the heap may not be aligned 16.
        -wd4201 # nonstandard extension used: nameless struct/union.
        -wd4310 # cast truncates constant value.
        -D_CRT_SECURE_NO_WARNINGS
        )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

    # Clang requires an additional library to be linked to use C++11.
    if (ATLAS_COMPILER_CLANG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
endif()
