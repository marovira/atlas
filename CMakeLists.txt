cmake_minimum_required(VERSION 3.0)

set(ATLAS atlas)
project(${ATLAS})

# Define useful variables for build.
set(ATLAS_ROOT_DIR "${CMAKE_SOURCE_DIR}")
set(ATLAS_SOURCE_ROOT "${ATLAS_ROOT_DIR}/source")
set(ATLAS_INCLUDE_ROOT "${ATLAS_ROOT_DIR}/include")

set(ATLAS_EXTERNAL_ROOT "${ATLAS_ROOT_DIR}/external")
set(ATLAS_GLM_ROOT "${ATLAS_EXTERNAL_ROOT}/glm")
set(ATLAS_GLFW_ROOT "${ATLAS_EXTERNAL_ROOT}/glfw")

# Set folders for VS solutions.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set compiler settings.
if (MSVC)
    add_definitions(
        -wd4714 # force inline not inlined.
        -wd4510 # default constructor could not be generated.
        -wd4512 # assignment operator could not be generated.
        -wd4610 # nonstandard extension used.
        -wd4316 # object allocated on the heap may not be aligned 16.
        -wd4201 # nonstandard extension used: nameless struct/union.
        -D_CRT_SECURE_NO_WARNINGS
        )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

    # Clang requires an additional library to be linked to use C++11.
    if (CLANG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
endif()

# Add GLFW.
add_subdirectory("${ATLAS_GLFW_ROOT}")

# Add GLEW except on apple.
if (NOT APPLE)
    find_package(GLEW REQUIRED)
    if (GLEW_FOUND)
        include_directories(${GLEW_INCLUDE_DIRS})
    else()
        message(FATAL_ERROR "Cannot build atlas without GLEW.")
    endif()
endif()

include_directories(
    "${ATLAS_GLFW_ROOT}/include"
    ${ATLAS_GLM_ROOT}
    ${ATLAS_INCLUDE_ROOT})

# Now add the source and include folders.
include("${ATLAS_INCLUDE_ROOT}/atlas/CMakeLists.txt")
include("${ATLAS_SOURCE_ROOT}/atlas/CMakeLists.txt")

# Add the library.
add_library(${ATLAS} ${ATLAS_SOURCE_LIST} ${ATLAS_INCLUDE_LIST})
target_link_libraries(${ATLAS} glfw ${GLFW_LIBRARIES} ${GLEW_LIBRARIES})
