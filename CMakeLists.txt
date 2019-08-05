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
option(ATLAS_BUILD_GL_TEST "Build Atlas OpenGL tests" ON)
option(ATLAS_BUILD_GUI_TEST "Build Atlas GUI tests" ON)

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
include("${ATLAS_SOURCE_DIR}/cmake/SourceGroups.cmake")

#================================
# Clang targets.
#================================
include("${ATLAS_SOURCE_DIR}/cmake/Clang.cmake")

#================================
# Find Packages.
#================================
include("${ATLAS_SOURCE_DIR}/cmake/Packages.cmake")


#================================
# Set compiler flags.
#================================
if (MSVC)
    set(ATLAS_COMPILER_FLAGS /W4 /WX /MP)
    set(ATLAS_DEBUG_FLAGS "$<$<CONFIG:DEBUG>:/ZI>")
endif()

#================================
# Core module.
#================================
add_library(atlas-core INTERFACE)
target_include_directories(atlas-core INTERFACE "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-core INTERFACE fmt::fmt)
target_compile_features(atlas-core INTERFACE cxx_std_17)
target_compile_options(atlas-core INTERFACE "${ATLAS_COMPILER_FLAGS}")
target_compile_options(atlas-core INTERFACE "${ATLAS_DEBUG_FLAGS}")
if (MSVC)
    add_custom_target(_atlas-core SOURCES ${ATLAS_INCLUDE_CORE_GROUP})
endif()
add_library(atlas::core ALIAS atlas-core)

#================================
# Math module.
#================================
add_library(atlas-math INTERFACE)
target_include_directories(atlas-math INTERFACE "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-math INTERFACE glm atlas-core)
target_compile_features(atlas-math INTERFACE cxx_std_17)
target_compile_options(atlas-math INTERFACE "${ATLAS_COMPILER_FLAGS}")
target_compile_options(atlas-math INTERFACE "${ATLAS_DEBUG_FLAGS}")
if (MSVC)
    add_custom_target(_atlas-math SOURCES ${ATLAS_INCLUDE_CORE_GROUP})
endif()
add_library(atlas::math ALIAS atlas-math)

#================================
# GLX module.
#================================
add_library(atlas-glx ${ATLAS_INCLUDE_GLX_GROUP} ${ATLAS_SOURCE_GLX_GROUP})
target_include_directories(atlas-glx PUBLIC "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-glx PUBLIC OpenGL::GL glfw gl3w::gl3w atlas-core)
target_compile_features(atlas-glx PUBLIC cxx_std_17)
target_compile_options(atlas-glx PUBLIC "${ATLAS_COMPILER_FLAGS}")
target_compile_options(atlas-glx PUBLIC "${ATLAS_DEBUG_FLAGS}")
if (MSVC)
    target_compile_definitions(atlas-glx PUBLIC -DNOMINMAX)
endif()
add_library(atlas::glx ALIAS atlas-glx)

#================================
# GUI module.
#================================
add_library(atlas-gui ${ATLAS_INCLUDE_GUI_GROUP} ${ATLAS_SOURCE_GUI_GROUP})
target_include_directories(atlas-gui PUBLIC "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-gui PUBLIC atlas-glx atlas-core)
target_compile_features(atlas-gui PUBLIC cxx_std_17)
target_compile_options(atlas-gui PUBLIC "${ATLAS_COMPILER_FLAGS}")
target_compile_options(atlas-gui PUBLIC "${ATLAS_DEBUG_FLAGS}")
add_library(atlas::gui ALIAS atlas-gui)

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
        atlas-glx
        atlas-gui
        Catch2::Catch2)
    target_compile_features(atlas-test PUBLIC cxx_std_17)
    target_compile_options(atlas-test PUBLIC ${ATLAS_COMPILER_FLAGS})
    target_compile_options(atlas-test PUBLIC ${ATLAS_DEBUG_FLAGS})
    if (ATLAS_BUILD_GL_TEST)
        if (MSVC)
            target_compile_definitions(atlas-test PUBLIC -DATLAS_BUILD_GL_TESTS)
        endif()
    endif()
    if (ATLAS_BUILD_GUI_TEST)
        if (MSVC)
            target_compile_definitions(atlas-test PUBLIC -DATLAS_BUILD_GUI_TESTS)
        endif()
    endif()

    include(CTest)
    include(Catch)
    catch_discover_tests(atlas-test)
endif()
