#================================
# Project setup.
#================================
project(atlas)

cmake_minimum_required(VERSION 3.0)

# Now setup the version data.
set(ATLAS_VERSION_MAJOR "1")
set(ATLAS_VERSION_MINOR "0")
set(ATLAS_VERSION_PATCH "0")
set(ATLAS_VERSION_EXTRA "")
set(ATLAS_VERSION "${ATLAS_VERSION_MAJOR}.${ATLAS_VERSION_MINOR}")
set(ATLAS_VERSION_FULL "${ATLAS_VERSION}.${ATLAS_VERSION_PATCH}${ATLAS_VERSION_EXTRA}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Setup the options
option(ATLAS_BUILD_DOCS "Build the Atlas documentation" ON)

#================================
# Directory variables.
#================================
# Set up the directory roots for Atlas sources.
set(ATLAS_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(ATLAS_BINARY_DIR "${PROJECT_BINARY_DIR}")
set(ATLAS_SOURCE_ROOT "${ATLAS_SOURCE_DIR}/source/atlas")
set(ATLAS_INCLUDE_ROOT "${ATLAS_SOURCE_DIR}/include/atlas")
set(ATLAS_DOCS_ROOT "${ATLAS_SOURCE_DIR}/docs")

# Now the external directories.
set(ATLAS_GLM_ROOT "${ATLAS_SOURCE_DIR}/external/glm")
set(ATLAS_GLI_ROOT "${ATLAS_SOURCE_DIR}/external/gli")
set(ATLAS_GLFW_ROOT "${ATLAS_SOURCE_DIR}/external/glfw")

#================================
# Compilation flags.
#================================
if (MSVC)
    add_definitions(
        -wd4714 # force inline not inlined.
        -wd4510 # default constructor could not be generated.
        -wd4512 # assignment operator could not be generated.
        -wd4610 # nonstandard extension used.
        -wd4316 # object allocated on the heap may not be aligned 16.
        -wd4201 # nonstandard extension used: nameless struct/union.
        -wd4310 # cast truncates constant value.
        -wd4458 # declaration of '' hides class member.
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

#================================
# External dependencies.
#================================
# If we want to build documentation, we need Doxygen.
if (ATLAS_BUILD_DOCS)
    # Skip the graph stuff from Doxygen.
    set(DOXYGEN_SKIP_DOT TRUE)
    find_package(Doxygen)
endif()

if (NOT APPLE)
    find_package(GLEW REQUIRED)
    if (NOT GLEW_FOUND)
        message(FATAL_ERROR "Cannot build Atlas without GLEW. Please see the wiki for information on how to install GLEW on your platform.")
    endif()
endif()

#================================
# Generate files.
#================================
if (ATLAS_BUILD_DOCS)
    configure_file("${ATLAS_DOCS_ROOT}/Doxyfile.in"
        "${ATLAS_BINARY_DIR}/docs/Doxyfile" @ONLY)
endif()

#================================
# Add subdirectories.
#================================
# Prevent GLFW from building examples and tests.
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
add_subdirectory("${ATLAS_GLFW_ROOT}")

# Now add the Atlas source and include directories.
add_subdirectory("${ATLAS_INCLUDE_ROOT}")
add_subdirectory("${ATLAS_SOURCE_ROOT}")

if (DOXYGEN_FOUND AND ATLAS_BUILD_DOCS)
    add_subdirectory("${ATLAS_DOCS_ROOT}")
endif()

#================================
# Include directories.
#================================
include_directories(
    "${ATLAS_GLFW_ROOT}/include"
    ${ATLAS_GLM_ROOT}
    ${ATLAS_GLI_ROOT}
    "${ATLAS_SOURCE_DIR}/include"
    )

#================================
# Source groups.
#================================
source_group("include" FILES)
source_group("include\\atlas" FILES ${ATLAS_INCLUDE_ROOT_LIST_PARENT})
source_group("include\\atlas\\core" FILES ${ATLAS_INCLUDE_CORE_LIST_PARENT})
source_group("include\\atlas\\gl" FILES ${ATLAS_INCLUDE_GL_LIST_PARENT})
source_group("include\\atlas\\math" FILES ${ATLAS_INCLUDE_MATH_LIST_PARENT})
source_group("include\\atlas\\utils" FILES ${ATLAS_INCLUDE_UTILS_LIST_PARENT})

source_group("source\\atlas\\core" FILES ${ATLAS_SOURCE_CORE_LIST_PARENT})
source_group("source\\atlas\\gl" FILES ${ATLAS_SOURCE_GL_LIST_PARENT})
source_group("source\\atlas\\utils" FILES ${ATLAS_SOURCE_UTILS_LIST_PARENT})

#================================
# Build the library.
#================================
add_library(atlas ${ATLAS_SOURCE_LIST} ${ATLAS_INCLUDE_LIST})
target_link_libraries(atlas glfw ${GLFW_LIBRARIES} ${GLEW_LIBRARIES})
set_target_properties(atlas PROPERTIES FOLDER "atlas")

#================================
# Set external variables.
#================================
# Make the include directories and libraries visible to other projects.
if (NOT ${CMAKE_PROJECT_NAME} STREQUAL atlas)
    set(ATLAS_INCLUDE_DIRS 
        "${ATLAS_SOURCE_DIR}/include"
        ${GLEW_INCLUDE_DIRS} 
        ${ATLAS_GLM_ROOT}
        ${ATLAS_GLI_ROOT}
        "${ATLAS_GLFW_ROOT}/include"
        PARENT_SCOPE)

    set(ATLAS_LIBRARIES
        atlas
        glfw
        ${GLFW_LIBRARIES}
        ${GLEW_LIBRARIES}
        PARENT_SCOPE)
endif()
