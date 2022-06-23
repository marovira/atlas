#================================
# Project setup.
#================================
cmake_minimum_required(VERSION 3.22)
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
if (ATLAS_BUILD_TESTS)
    add_subdirectory(${ATLAS_TEST_ROOT})
endif()

#================================
# Source groups.
#================================
source_group("include" FILES)
source_group("include\\atlas" FILES ${ATLAS_INCLUDE_ROOT_GROUP})
source_group("include\\atlas\\math" FILES ${ATLAS_INCLUDE_MATH_GROUP})
source_group("include\\atlas\\glx" FILES ${ATLAS_INCLUDE_GLX_GROUP})
source_group("include\\atlas\\gui" FILES ${ATLAS_INCLUDE_GUI_GROUP})
source_group("include\\atlas\\gui\\widgets" FILES
    ${ATLAS_INCLUDE_GUI_WIDGETS_GROUP})
source_group("include\\atlas\\hlr" FILES ${ATLAS_INCLUDE_HLR_GROUP})
source_group("include\\atlas\\utils" FILES ${ATLAS_INCLUDE_UTILS_GROUP})

source_group("source" FILES)
source_group("source\\atlas" FILES)
source_group("source\\atlas\\glx" FILES ${ATLAS_SOURCE_GLX_GROUP})
source_group("source\\atlas\\gui" FILES ${ATLAS_SOURCE_GUI_GROUP})
source_group("source\\atlas\\gui\\widgets" FILES
    ${ATLAS_SOURCE_GUI_WIDGETS_GROUP})
source_group("source\\atlas\\hlr" FILES ${ATLAS_SOURCE_HLR_GROUP})
source_group("source\\atlas\\utils" FILES ${ATLAS_SOURCE_UTILS_GROUP})

#================================
# Find Packages.
#================================
include(FetchContent)

# Declare fetch content for all packages.
# Zeus always set to latest version.
FetchContent_Declare(
    zeus
    GIT_REPOSITORY https://github.com/marovira/zeus
    GIT_TAG 8167f1b3257d1ef99448304b3710bf0145218121
    )

# Move to latest version that has the INSTALL target
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git 
    GIT_TAG cc98465e3508535ba8c7f6208df934c156a018dc
    )

# Version 3.3.7
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git 
    GIT_TAG 45ce5ddd197d5c58f50fdd3296a5131c894e5527
    )

FetchContent_Declare(
    gl3w
    GIT_REPOSITORY https://github.com/marovira/gl3w.git
    GIT_TAG d5a85780ea09a5a872c7e71877298a96834fa23c
    )

# Latest version
FetchContent_Declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/syoyo/tinyobjloader.git
    GIT_TAG 0ed6c38f20c63b996fbb9fa949569b2acb213a3d
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
# Math module.
#================================
add_library(atlas_math INTERFACE)
target_include_directories(atlas_math INTERFACE ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas_math INTERFACE glm::glm zeus::zeus)
target_compile_features(atlas_math INTERFACE cxx_std_20)
if (MSVC)
    add_custom_target(_atlas_math SOURCES ${ATLAS_INCLUDE_MATH_GROUP})
    set_target_properties(_atlas_math PROPERTIES FOLDER "atlas")
endif()
add_library(atlas::math ALIAS atlas_math)

#================================
# GLX module.
#================================
add_library(atlas_glx ${ATLAS_INCLUDE_GLX_GROUP} ${ATLAS_SOURCE_GLX_GROUP})
target_include_directories(atlas_glx PUBLIC ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas_glx PUBLIC OpenGL::GL glfw gl3w::gl3w zeus::zeus)
add_library(atlas::glx ALIAS atlas_glx)
set_target_properties(atlas_glx PROPERTIES FOLDER "atlas")

#================================
# GUI module.
#================================
add_library(atlas_gui 
    ${ATLAS_INCLUDE_GUI_GROUP} 
    ${ATLAS_INCLUDE_GUI_WIDGETS_GROUP}
    ${ATLAS_SOURCE_GUI_GROUP}
    ${ATLAS_SOURCE_GUI_WIDGETS_GROUP}
    )
target_include_directories(atlas_gui PUBLIC ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas_gui PUBLIC imgui atlas_glx zeus::zeus)
add_library(atlas::gui ALIAS atlas_gui)
set_target_properties(atlas_gui PROPERTIES FOLDER "atlas")

#================================
# HLR module.
#================================
add_library(atlas_hlr ${ATLAS_INCLUDE_HLR_GROUP} ${ATLAS_SOURCE_HLR_GROUP})
target_include_directories(atlas_hlr PUBLIC ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas_hlr PUBLIC zeus::zeus)
add_library(atlas::hlr ALIAS atlas_hlr)
set_target_properties(atlas_hlr PROPERTIES FOLDER "atlas")

#================================
# Utils module.
#================================
add_library(atlas_utils ${ATLAS_INCLUDE_UTILS_GROUP}
    ${ATLAS_SOURCE_UTILS_GROUP})
target_include_directories(atlas_utils PUBLIC ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas_utils PUBLIC atlas_math ${TINYOBJLOADER_LIB}
    zeus::zeus stb)
add_library(atlas::utils ALIAS atlas_utils)
set_target_properties(atlas_utils PROPERTIES FOLDER "atlas")

#================================
# Main Atlas library.
#================================
add_library(atlas ${ATLAS_SOURCE_ROOT_GROUP})
target_include_directories(atlas PUBLIC ${ATLAS_SOURCE_ROOT})
target_link_libraries(atlas PUBLIC zeus::zeus atlas_math atlas_glx atlas_gui
    atlas_hlr atlas_utils)
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

    source_group("include" FILES ${ATLAS_TEST_HEADER_GROUP})

    source_group("source" FILES ${ATLAS_TEST_TOP_GROUP})
    source_group("source\\glx" FILES ${ATLAS_TEST_GLX_GROUP})
    source_group("source\\gui" FILES ${ATLAS_TEST_GUI_GROUP})
    source_group("source\\hlr" FILES ${ATLAS_TEST_HLR_GROUP})
    source_group("source\\math" FILES ${ATLAS_TEST_MATH_GROUP})

    add_executable(atlas_test ${ATLAS_TEST_LIST})
    target_include_directories(atlas_test PRIVATE ${ATLAS_TEST_ROOT})
    target_link_libraries(atlas_test 
        atlas_math 
        atlas_glx
        atlas_gui
        atlas_hlr
        atlas_utils
        Catch2::Catch2)
    if (ATLAS_BUILD_GL_TEST)
        if (MSVC)
            target_compile_definitions(atlas_test PUBLIC -DATLAS_BUILD_GL_TESTS)
        endif()
    endif()
    if (ATLAS_BUILD_GUI_TEST)
        if (MSVC)
            target_compile_definitions(atlas_test PUBLIC -DATLAS_BUILD_GUI_TESTS)
        endif()
    endif()
    set_target_properties(atlas_test PROPERTIES FOLDER "atlas")

    include(CTest)
    include(Catch)
    catch_discover_tests(atlas_test)
endif()
