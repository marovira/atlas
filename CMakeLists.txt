#================================
# Project setup.
#================================
project(atlas)

cmake_minimum_required(VERSION 3.0)

# Now setup the version data.
set(ATLAS_VERSION_MAJOR "1")
set(ATLAS_VERSION_MINOR "3")
set(ATLAS_VERSION_PATCH "0")
set(ATLAS_VERSION_EXTRA "")
set(ATLAS_VERSION "${ATLAS_VERSION_MAJOR}.${ATLAS_VERSION_MINOR}")
set(ATLAS_VERSION_FULL "${ATLAS_VERSION}.${ATLAS_VERSION_PATCH}${ATLAS_VERSION_EXTRA}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Setup the options
option(ATLAS_BUILD_DOCS "Build the Atlas documentation" ON)
option(ATLAS_INCLUDE_TEXTURE "Includes the Texture class (broken in labs)" OFF)

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
set(ATLAS_GLI_ROOT "${ATLAS_SOURCE_DIR}/external/gli")
set(ATLAS_GLFW_ROOT "${ATLAS_SOURCE_DIR}/external/glfw")

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
# Prevent GLFW from building examples and tests.
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
include("${ATLAS_CONFIG_ROOT}/SourceGroups.cmake")

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
