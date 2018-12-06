#================================
# Project setup.
#================================
cmake_minimum_required(VERSION 3.10)
project(atlas VERSION 3.0.0 LANGUAGES CXX)

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
set(ATLAS_TEST_ROOT "${ATLAS_SOURCE_DIR}/test")

#================================
# Add subdirectories.
#================================
add_subdirectory("${ATLAS_INCLUDE_ROOT}")

#================================
# Source groups.
#================================
include("${ATLAS_SOURCE_DIR}/config/SourceGroups.cmake")

#================================
# Core Module.
#================================
add_library(atlas-core INTERFACE)
target_include_directories(atlas-core INTERFACE "${ATLAS_SOURCE_DIR}/include")

#================================
# Build the tests.
#================================
if (ATLAS_BUILD_TESTS)
    find_package(Catch2 REQUIRED)

    add_subdirectory("${ATLAS_TEST_ROOT}")
    add_executable(atlas-test ${ATLAS_TEST_LIST})
    target_link_libraries(atlas-test atlas-core Catch2::Catch2)
    target_compile_features(atlas-test PRIVATE cxx_std_17)

    include(CTest)
    include(Catch)
    catch_discover_tests(atlas-test)
endif()
