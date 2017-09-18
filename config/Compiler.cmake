#================================
# Compiler Configurations file.
#================================

# First identify the compiler we're using.
if (MSVC)
    if ((MSVC_VERSION VERSION_GREATER 1800))
        set(ATLAS_COMPILER_MSVC 1)
        message(STATUS "Using MSVC ${MSVC_VERSION}...")
    else()
        message(FATAL_ERROR "Atlas requires at least Visual Studio 12 to run.")
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
        -wd4201 # nonstandard extension used: nameless struct/union.
        -D_CRT_SECURE_NO_WARNINGS
        )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++14")

    if (APPLE OR WIN32)
        if (ATLAS_COMPILER_CLANG)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        endif()
    endif()
endif()
