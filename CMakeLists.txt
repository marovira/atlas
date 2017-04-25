#================================
# Project setup.
#================================
project(atlas)

cmake_minimum_required(VERSION 3.0)

# Now setup the version data.
set(ATLAS_VERSION_MAJOR "2")
set(ATLAS_VERSION_MINOR "0")
set(ATLAS_VERSION_PATCH "0")
set(ATLAS_VERSION_EXTRA "")
set(ATLAS_VERSION "${ATLAS_VERSION_MAJOR}.${ATLAS_VERSION_MINOR}")
set(ATLAS_VERSION_FULL 
    "${ATLAS_VERSION}.${ATLAS_VERSION_PATCH}${ATLAS_VERSION_EXTRA}")

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

# Set the configs folder.
set(ATLAS_CONFIG_ROOT "${ATLAS_SOURCE_DIR}/config")

# Now the external directories.
set(ATLAS_GLM_ROOT "${ATLAS_SOURCE_DIR}/external/glm")
set(ATLAS_GLFW_ROOT "${ATLAS_SOURCE_DIR}/external/glfw")
set(ATLAS_OBJ_ROOT "${ATLAS_SOURCE_DIR}/external/tinyobjloader")
set(ATLAS_STB_ROOT "${ATLAS_SOURCE_DIR}/external/stb")
set(ATLAS_IMGUI_ROOT "${ATLAS_SOURCE_DIR}/external/imgui")
set(ATLAS_GL3W_ROOT "${ATLAS_SOURCE_DIR}/external/gl3w")

#================================
# Compilation flags.
#================================
include("${ATLAS_CONFIG_ROOT}/Compiler.cmake")

#================================
# External dependencies.
#================================
include("${ATLAS_CONFIG_ROOT}/Externals.cmake")

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
add_subdirectory("${ATLAS_GLFW_ROOT}")
add_subdirectory("${ATLAS_IMGUI_ROOT}")
add_subdirectory("${ATLAS_GL3W_ROOT}")
add_subdirectory("${ATLAS_STB_ROOT}")
add_subdirectory("${ATLAS_OBJ_ROOT}")

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
    "${ATLAS_OBJ_ROOT}/include"
    "${ATLAS_STB_ROOT}/include"
    "${ATLAS_IMGUI_ROOT}/include"
    "${ATLAS_GL3W_ROOT}/include"
    "${ATLAS_SOURCE_DIR}/include"
    )

#================================
# Source groups.
#================================
include("${ATLAS_CONFIG_ROOT}/SourceGroups.cmake")

#================================
# Build the library.
#================================
add_library(atlas ${ATLAS_SOURCE_LIST} ${ATLAS_INCLUDE_LIST})
target_link_libraries(atlas glfw ${GLFW_LIBRARIES} imgui gl3w stb tinyobjloader 
    ${OPENGL_gl_LIBRARY})
set_target_properties(atlas PROPERTIES FOLDER "atlas")

#================================
# Set external variables.
#================================
# Make the include directories and libraries visible to other projects.
if (NOT ${CMAKE_PROJECT_NAME} STREQUAL atlas)
    set(ATLAS_INCLUDE_DIRS 
        ${ATLAS_GLM_ROOT}
        "${ATLAS_SOURCE_DIR}/include"
        "${ATLAS_OBJ_ROOT}/include"
        "${ATLAS_STB_ROOT}/include"
        "${ATLAS_IMGUI_ROOT}/include"
        "${ATLAS_GLFW_ROOT}/include"
        "${ATLAS_GL3W_ROOT}/include"
        PARENT_SCOPE)

    set(ATLAS_LIBRARIES
        atlas
        glfw
        imgui
        gl3w
        stb
        tinyobjloader
        ${GLFW_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        PARENT_SCOPE)
endif()
