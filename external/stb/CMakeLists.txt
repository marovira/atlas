project(stb)

cmake_minimum_required(VERSION 3.0)

set(STB_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(STB_BINARY_DIR "${PROJECT_BINARY_DIR}")

include_directories("${STB_SOURCE_DIR}/include/stb")

set(STB_SOURCE_LIST
    "${STB_SOURCE_DIR}/source/stb/stb_image.cpp")
set(STB_INCLUDE_LIST
    "${STB_SOURCE_DIR}/include/stb/stb_image.h")

source_group("include" FILES)
source_group("include\\stb" FILES ${STB_INCLUDE_LIST})
source_group("source" FILES)
source_group("source\\stb" FILES ${STB_SOURCE_LIST})

if (MSVC)
    add_definitions(
        -wd4244 # conversion results in possible loss of data.
        )
endif()

add_library(stb ${STB_SOURCE_LIST} ${STB_INCLUDE_LIST})
set_target_properties(stb PROPERTIES FOLDER "stb")
