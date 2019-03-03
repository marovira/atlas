#================================
# Project setup.
#================================
cmake_minimum_required(VERSION 3.10)
project(atlas VERSION 3.0.0 LANGUAGES CXX C)

if (APPLE)
    message(ERROR "Support for Apple has been deprecated")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(ATLAS_BUILD_TESTS "Build Atlas test" ON)

# Now setup the version data.
set(ATLAS_VERSION_MAJOR "3")
set(ATLAS_VERSION_MINOR "0")
set(ATLAS_VERSION_PATCH "0")
set(ATLAS_VERSION_EXTRA "")
set(ATLAS_VERSION "${ATLAS_VERSION_MAJOR}.${ATLAS_VERSION_MINOR}")
set(ATLAS_VERSION_FULL 
    "${ATLAS_VERSION}.${ATLAS_VERSION_PATCH}${ATLAS_VERSION_EXTRA}")

#================================
# Directory variables.
#================================
# Set up the directory roots for Atlas sources.
set(ATLAS_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(ATLAS_BINARY_DIR "${PROJECT_BINARY_DIR}")
set(ATLAS_INCLUDE_ROOT "${ATLAS_SOURCE_DIR}/include/atlas")
set(ATLAS_SOURCE_ROOT "${ATLAS_SOURCE_DIR}/source/atlas")
set(ATLAS_TEST_ROOT "${ATLAS_SOURCE_DIR}/test")

#================================
# Add subdirectories.
#================================
add_subdirectory("${ATLAS_INCLUDE_ROOT}")
add_subdirectory("${ATLAS_SOURCE_ROOT}")

#================================
# Source groups.
#================================
include("${ATLAS_SOURCE_DIR}/config/SourceGroups.cmake")

#================================
# Find Packages.
#================================
find_package(fmt REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

#================================
# Set compiler flags.
#================================
if (MSVC)
    set(ATLAS_COMPILER_FLAGS /W4 /WX)
    set(ATLAS_DEBUG_FLAGS "$<$<CONFIG:DEBUG>:/ZI>")
endif()

#================================
# Core Module.
#================================
add_library(atlas-core INTERFACE)
target_include_directories(atlas-core INTERFACE "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-core INTERFACE fmt::fmt)
target_compile_features(atlas-core INTERFACE cxx_std_17)
target_compile_options(atlas-core INTERFACE "${ATLAS_COMPILER_FLAGS}")
target_compile_options(atlas-core INTERFACE "${ATLAS_DEBUG_FLAGS}")
add_library(atlas::core ALIAS atlas-core)

#================================
# Math Module.
#================================
add_library(atlas-math INTERFACE)
target_include_directories(atlas-math INTERFACE "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-math INTERFACE glm atlas-core)
target_compile_features(atlas-math INTERFACE cxx_std_17)
target_compile_options(atlas-math INTERFACE "${ATLAS_COMPILER_FLAGS}")
target_compile_options(atlas-math INTERFACE "${ATLAS_DEBUG_FLAGS}")
add_library(atlas::math ALIAS atlas-math)

#================================
# GL module.
#================================
add_library(atlas-gl ${ATLAS_INCLUDE_GL_GROUP} ${ATLAS_SOURCE_GL_GROUP})
target_include_directories(atlas-gl PUBLIC "${ATLAS_INCLUDE_ROOT}/gl")
target_link_libraries(atlas-gl PUBLIC OpenGL::GL glfw)
target_compile_features(atlas-gl PUBLIC cxx_std_17)
target_compile_options(atlas-gl PUBLIC "${ATLAS_COMPILER_FLAGS}")
if (MSVC)
    target_compile_options(atlas-gl PRIVATE -wd4152)
endif()
add_library(atlas::gl ALIAS atlas-gl)

#================================
# GLEX module.
#================================
add_library(atlas-glx ${ATLAS_INCLUDE_GLX_GROUP} ${ATLAS_SOURCE_GLX_GROUP})
target_include_directories(atlas-glx PUBLIC "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-glx PRIVATE atlas-gl)
target_compile_features(atlas-glx PUBLIC cxx_std_17)
target_compile_options(atlas-glx PUBLIC "${ATLAS_COMPILER_FLAGS}")
target_compile_options(atlas-glx PUBLIC "${ATLAS_DEBUG_FLAGS}")
add_library(atlas::glx ALIAS atlas-glx)

#================================
# Build the tests.
#================================
if (ATLAS_BUILD_TESTS)
    find_package(Catch2 REQUIRED)

    add_subdirectory("${ATLAS_TEST_ROOT}")
    add_executable(atlas-test ${ATLAS_TEST_LIST})
    target_link_libraries(atlas-test 
        atlas-core 
        atlas-math 
        atlas-gl
        glfw
        Catch2::Catch2)

    include(CTest)
    include(Catch)
    catch_discover_tests(atlas-test)
endif()
