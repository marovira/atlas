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

# Make sure Zeus does not make the clang-format target and disable the
# unit tests for it.
set(ZEUS_CLANG_FORMAT_TARGET OFF CACHE INTERNAL "")
set(ZEUS_BUILD_TESTS OFF CACHE INTERNAL "")

#================================
# Directory variables.
#================================
set(ATLAS_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(ATLAS_SOURCE_ROOT ${ATLAS_SOURCE_DIR}/src)
set(ATLAS_TEST_ROOT ${ATLAS_SOURCE_DIR}/test)
set(ATLAS_CMAKE_ROOT ${ATLAS_SOURCE_DIR}/cmake)
set(ATLAS_EXTERNAL_ROOT ${ATLAS_SOURCE_DIR}/external)

#================================
# Add subdirectories.
#================================
add_subdirectory(${ATLAS_EXTERNAL_ROOT}/imgui)
add_subdirectory(${ATLAS_EXTERNAL_ROOT}/stb)
add_subdirectory(${ATLAS_SOURCE_ROOT})

#================================
# Source groups.
#================================
source_group("include" FILES)
source_group("include\\atlas" FILES ${ATLAS_INCLUDE_ROOT_GROUP})
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
    zeus
    GIT_REPOSITORY https://github.com/marovira/zeus
    GIT_TAG d0e476695f86e0f4be6740404f1be17b9dededff
    )

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git 
    GIT_TAG 0.9.9.7
    )

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git 
    GIT_TAG 3.3.1
    )

FetchContent_Declare(
    gl3w
    GIT_REPOSITORY https://github.com/marovira/gl3w.git
    )

FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/syoyo/tinyobjloader.git
    GIT_TAG d7d53217c30a7970596d1b93f97a5fd97b7cf1a5
    )

find_package(zeus QUIET)
find_package(glm QUIET)
find_package(glfw3 QUIET)
find_package(gl3w QUIET)
find_package(tinyobjloader QUIET)
find_package(OpenGL REQUIRED QUIET)

if (NOT zeus_FOUND AND NOT zeus_POPULATED)
    FetchContent_Populate(zeus)
    add_subdirectory(${zeus_SOURCE_DIR} ${zeus_BINARY_DIR})
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
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL
        "GNU")
    set(ATLAS_COMMON_FLAGS -Wall -Wextra -pedantic -Werror)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(ATLAS_COMPILER_FLAGS ${ATLAS_COMMON_FLAGS})
    else()
        set(ATLAS_COMPILER_FLAGS ${ATLAS_COMMON_FLAGS} -stdlib=libstdc++)
    endif()
    set(ATLAS_DEBUG_FLAGS "$<$<CONFIG:DEBUG>:-g>")
endif()

#================================
# Math module.
#================================
add_library(atlas-math INTERFACE)
target_include_directories(atlas-math INTERFACE ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas-math INTERFACE glm zeus::zeus)
target_compile_features(atlas-math INTERFACE cxx_std_17)
target_compile_options(atlas-math INTERFACE "${ATLAS_COMPILER_FLAGS}")
target_compile_options(atlas-math INTERFACE "${ATLAS_DEBUG_FLAGS}")
target_compile_definitions(atlas-math INTERFACE ${ATLAS_COMPILE_DEFINITIONS})
if (MSVC)
    add_custom_target(_atlas-math SOURCES ${ATLAS_INCLUDE_MATH_GROUP})
    set_target_properties(_atlas-math PROPERTIES FOLDER "atlas")
endif()
add_library(atlas::math ALIAS atlas-math)

#================================
# GLX module.
#================================
add_library(atlas-glx ${ATLAS_INCLUDE_GLX_GROUP} ${ATLAS_SOURCE_GLX_GROUP})
target_include_directories(atlas-glx PUBLIC ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas-glx PUBLIC OpenGL::GL glfw gl3w::gl3w zeus::zeus)
add_library(atlas::glx ALIAS atlas-glx)
set_target_properties(atlas-glx PROPERTIES FOLDER "atlas")

#================================
# GUI module.
#================================
add_library(atlas-gui ${ATLAS_INCLUDE_GUI_GROUP} ${ATLAS_SOURCE_GUI_GROUP})
target_include_directories(atlas-gui PUBLIC ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas-gui PUBLIC imgui atlas-glx zeus::zeus)
add_library(atlas::gui ALIAS atlas-gui)
set_target_properties(atlas-gui PROPERTIES FOLDER "atlas")

#================================
# HLR module.
#================================
add_library(atlas-hlr ${ATLAS_INCLUDE_HLR_GROUP} ${ATLAS_SOURCE_HLR_GROUP})
target_include_directories(atlas-hlr PUBLIC ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas-hlr PUBLIC zeus::zeus)
add_library(atlas::hlr ALIAS atlas-hlr)
set_target_properties(atlas-hlr PROPERTIES FOLDER "atlas")

#================================
# Utils module.
#================================
add_library(atlas-utils ${ATLAS_INCLUDE_UTILS_GROUP}
    ${ATLAS_SOURCE_UTILS_GROUP})
target_include_directories(atlas-utils PUBLIC ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas-utils PUBLIC atlas-math ${TINYOBJLOADER_LIB}
    zeus::zeus stb)
add_library(atlas::utils ALIAS atlas-utils)
set_target_properties(atlas-utils PROPERTIES FOLDER "atlas")

#================================
# Main Atlas library.
#================================
add_library(atlas ${ATLAS_SOURCE_ROOT_GROUP})
target_include_directories(atlas PUBLIC ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas PUBLIC zeus::zeus atlas-math atlas-glx atlas-gui
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
        GIT_TAG v2.11.1)

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
