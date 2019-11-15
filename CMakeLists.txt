#================================
# Project setup.
#================================
cmake_minimum_required(VERSION 3.12)
project(atlas VERSION 3.0.0 LANGUAGES CXX C)

if (APPLE)
    message(ERROR "Support for Apple has been deprecated")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ATLAS_VERSION_MAJOR "3")
set(ATLAS_VERSION_MINOR "0")
set(ATLAS_VERSION_PATCH "0")
set(ATLAS_VERSION_EXTRA "")
set(ATLAS_VERSION "${ATLAS_VERSION_MAJOR}.${ATLAS_VERSION_MINOR}")
set(ATLAS_VERSION_FULL 
    "${ATLAS_VERSION}.${ATLAS_VERSION_PATCH}${ATLAS_VERSION_EXTRA}")

if (POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()


#================================
# Option variables.
#================================
option(ATLAS_BUILD_TESTS "Build Atlas test" ON)
option(ATLAS_BUILD_GL_TEST "Build Atlas OpenGL tests" ON)
option(ATLAS_BUILD_GUI_TEST "Build Atlas GUI tests" ON)
option(ATLAS_BUILD_EXAMPLES "Build Atlas examples" ON)

#================================
# Directory variables.
#================================
# Set up the directory roots for Atlas sources.
set(ATLAS_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(ATLAS_BINARY_DIR "${PROJECT_BINARY_DIR}")
set(ATLAS_INCLUDE_ROOT "${ATLAS_SOURCE_DIR}/include/atlas")
set(ATLAS_SOURCE_ROOT "${ATLAS_SOURCE_DIR}/source/atlas")
set(ATLAS_TEST_ROOT "${ATLAS_SOURCE_DIR}/test")
set(ATLAS_CMAKE_ROOT "${ATLAS_SOURCE_DIR}/cmake")
set(ATLAS_EXTERNAL_ROOT "${ATLAS_SOURCE_DIR}/external")

#================================
# Add subdirectories.
#================================
add_subdirectory("${ATLAS_EXTERNAL_ROOT}/imgui")
add_subdirectory("${ATLAS_EXTERNAL_ROOT}/stb")
add_subdirectory("${ATLAS_INCLUDE_ROOT}")
add_subdirectory("${ATLAS_SOURCE_ROOT}")

#================================
# Source groups.
#================================
source_group("include" FILES)
source_group("include\\atlas" FILES ${ATLAS_INCLUDE_ROOT_GROUP})
source_group("include\\atlas\\core" FILES ${ATLAS_INCLUDE_CORE_GROUP})
source_group("include\\atlas\\math" FILES ${ATLAS_INCLUDE_MATH_GROUP})
source_group("include\\atlas\\glx" FILES ${ATLAS_INCLUDE_GLX_GROUP})
source_group("include\\atlas\\gui" FILES ${ATLAS_INCLUDE_GUI_GROUP})
source_group("include\\atlas\\hlr" FILES ${ATLAS_INCLUDE_HLR_GROUP})
source_group("include\\atlas\\utils" FILES ${ATLAS_INCLUDE_UTILS_GROUP})

source_group("source" FILES)
source_group("source\\atlas" FILES)
source_group("source\\atlas\\glx" FILES ${ATLAS_SOURCE_GLX_GROUP})
source_group("source\\atlas\\gui" FILES ${ATLAS_SOURCE_GUI_GROUP})
source_group("source\\atlas\\hlr" FILES ${ATLAS_SOURCE_HLR_GROUP})
source_group("source\\atlas\\utils" FILES ${ATLAS_SOURCE_UTILS_GROUP})

#================================
# Clang targets.
#================================
find_program(CLANG_FORMAT_EXE 
    NAMES "clang-format"
    DOC "Path to clang-format executable"
    )
if (NOT CLANG_FORMAT_EXE)
    message(STATUS "clang-format not found.")
else()
    message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")
    set(ATLAS_FILES 
        ${ATLAS_HEADER_FILES}
        ${ATLAS_SOURCE_FILES}
        )
    add_custom_target(clang-format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${ATLAS_FILES}
        )
endif()

find_program(CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable"
    )
if (NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(ATLAS_FILES
        ${ATLAS_HEADER_FILES}
        ${ATLAS_SOURCE_FILES}
        )
    add_custom_target(clang-tidy
        COMMAND ${CLANG_TIDY_EXE} ${ATLAS_FILES} 
        -config=""
        -- 
        -std=c++17
        -I${ATLAS_INCLUDE_ROOT}
        )
endif()

#================================
# Find Packages.
#================================
include(FetchContent)

# Declare fetch content for all packages.
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 5.3.0)
FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum
    GIT_TAG v0.6.3
    )
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git 
    GIT_TAG 0.9.9.5)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git 
    GIT_TAG 3.3)
FetchContent_Declare(
    gl3w
    GIT_REPOSITORY https://github.com/marovira/gl3w.git)
FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/syoyo/tinyobjloader.git
    GIT_TAG v2.0-rc1)

find_package(fmt QUIET)
find_package(magic_enum QUIET)
find_package(glm QUIET)
find_package(glfw3 QUIET)
find_package(gl3w QUIET)
find_package(tinyobjloader QUIET)
find_package(OpenGL REQUIRED QUIET)

if (NOT fmt_FOUND AND NOT fmt_POPULATED)
    FetchContent_Populate(fmt)
    add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})
endif()

if (NOT magic_enum_FOUND AND NOT magic_enum_POPULATED)
    FetchContent_Populate(magic_enum)
    add_subdirectory(${magic_enum_SOURCE_DIR} ${magic_enum_BINARY_DIR})
endif()

if (NOT glm_FOUND AND NOT glm_POPULATED)
    FetchContent_Populate(glm)
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()

if (NOT glfw3_FOUND AND NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

if (NOT gl3w_FOUND AND NOT gl3w_POPULATED)
    FetchContent_Populate(gl3w)
    add_subdirectory(${gl3w_SOURCE_DIR} ${gl3w_BINARY_DIR})
endif()

set(TINYOBJLOADER_LIB "tinyobjloader::tinyobjloader")
if (NOT tinyobjloader_FOUND AND NOT tinyobjloader_POPULATED)
    FetchContent_Populate(tinyobjloader)
    add_subdirectory(${tinyobjloader_SOURCE_DIR} ${tinyobjloader_BINARY_DIR})
    set(TINYOBJLOADER_LIB "tinyobjloader")
endif()

#================================
# Compiler flags.
#================================
if (MSVC)
    set(ATLAS_COMPILER_FLAGS /W4 /WX /MP)
    set(ATLAS_DEBUG_FLAGS "$<$<CONFIG:DEBUG>:/ZI>")
    set(ATLAS_COMPILE_DEFINITIONS -DNOMINMAX)
else()
    set(ATLAS_COMPILER_FLAGS -Wall -Wextra -Werror)
    set(ATLAS_DEBUG_FLAGS "$<$<CONFIG:DEBUG>:-g>")
endif()

#================================
# Core module.
#================================
add_library(atlas-core INTERFACE)
target_include_directories(atlas-core INTERFACE "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-core INTERFACE fmt::fmt magic_enum::magic_enum)
if (NOT MSVC)
    target_link_libraries(atlas-core INTERFACE stdc++fs)
endif()
target_compile_features(atlas-core INTERFACE cxx_std_17)
target_compile_options(atlas-core INTERFACE "${ATLAS_COMPILER_FLAGS}")
target_compile_options(atlas-core INTERFACE "${ATLAS_DEBUG_FLAGS}")
target_compile_definitions(atlas-core INTERFACE ${ATLAS_COMPILE_DEFINITIONS})
if (MSVC)
    add_custom_target(_atlas-core SOURCES ${ATLAS_INCLUDE_CORE_GROUP})
    set_target_properties(_atlas-core PROPERTIES FOLDER "atlas")
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
target_compile_definitions(atlas-math INTERFACE ${ATLAS_COMPILE_DEFINITIONS})
if (MSVC)
    add_custom_target(_atlas-math SOURCES ${ATLAS_INCLUDE_CORE_GROUP})
    set_target_properties(_atlas-math PROPERTIES FOLDER "atlas")
endif()
add_library(atlas::math ALIAS atlas-math)

#================================
# GLX module.
#================================
add_library(atlas-glx ${ATLAS_INCLUDE_GLX_GROUP} ${ATLAS_SOURCE_GLX_GROUP})
target_include_directories(atlas-glx PUBLIC "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-glx PUBLIC OpenGL::GL glfw gl3w::gl3w atlas-core)
add_library(atlas::glx ALIAS atlas-glx)
set_target_properties(atlas-glx PROPERTIES FOLDER "atlas")

#================================
# GUI module.
#================================
add_library(atlas-gui ${ATLAS_INCLUDE_GUI_GROUP} ${ATLAS_SOURCE_GUI_GROUP})
target_include_directories(atlas-gui PUBLIC "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-gui PUBLIC imgui atlas-glx atlas-core)
add_library(atlas::gui ALIAS atlas-gui)
set_target_properties(atlas-gui PROPERTIES FOLDER "atlas")

#================================
# HLR module.
#================================
add_library(atlas-hlr ${ATLAS_INCLUDE_HLR_GROUP} ${ATLAS_SOURCE_HLR_GROUP})
target_include_directories(atlas-hlr PUBLIC "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-hlr PUBLIC atlas-core)
add_library(atlas::hlr ALIAS atlas-hlr)
set_target_properties(atlas-hlr PROPERTIES FOLDER "atlas")

#================================
# Utils module.
#================================
add_library(atlas-utils ${ATLAS_INCLUDE_UTILS_GROUP}
    ${ATLAS_SOURCE_UTILS_GROUP})
target_include_directories(atlas-utils PUBLIC "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas-utils PUBLIC atlas-math ${TINYOBJLOADER_LIB}
    fmt::fmt stb)
add_library(atlas::utils ALIAS atlas-utils)
set_target_properties(atlas-utils PROPERTIES FOLDER "atlas")

#================================
# Main Atlas library.
#================================
add_library(atlas ${ATLAS_SOURCE_ROOT_GROUP})
target_include_directories(atlas PUBLIC "${ATLAS_SOURCE_DIR}/include")
target_link_libraries(atlas PUBLIC atlas-core atlas-math atlas-glx atlas-gui
    atlas-hlr atlas-utils)
add_library(atlas::atlas ALIAS atlas)
set_target_properties(atlas PROPERTIES FOLDER "atlas")

#================================
# Build the tests.
#================================
if (ATLAS_BUILD_TESTS)
    FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v2.9.2)

    find_package(Catch2 QUIET)
    if (NOT Catch2_FOUND AND NOT catch2_POPULATED)
        FetchContent_Populate(catch2)
        add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
        set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
            "${catch2_SOURCE_DIR}/contrib")
    endif()

    add_subdirectory("${ATLAS_TEST_ROOT}")
    add_executable(atlas-test ${ATLAS_TEST_LIST})
    target_link_libraries(atlas-test 
        atlas-core 
        atlas-math 
        atlas-glx
        atlas-gui
        atlas-hlr
        atlas-utils
        Catch2::Catch2)
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
    set_target_properties(atlas-test PROPERTIES FOLDER "tests")

    include(CTest)
    include(Catch)
    catch_discover_tests(atlas-test)
endif()

if (ATLAS_BUILD_EXAMPLES)
    set(ATLAS_EXAMPLES_ROOT "${ATLAS_SOURCE_DIR}/examples")
    add_subdirectory(${ATLAS_EXAMPLES_ROOT})
endif()
