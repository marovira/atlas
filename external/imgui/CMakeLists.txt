project(imgui)

cmake_minimum_required(VERSION 3.0)

set(IMGUI_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(IMGUI_BINARY_DIR "${PROJECT_BINARY_DIR}")

include_directories("${IMGUI_SOURCE_DIR}/include/imgui")

set(IMGUI_SOURCE_LIST
    "${IMGUI_SOURCE_DIR}/source/imgui/imgui.cpp"
    "${IMGUI_SOURCE_DIR}/source/imgui/imgui_draw.cpp")
set(IMGUI_INCLUDE_LIST
    "${IMGUI_SOURCE_DIR}/include/imgui/imgui.h"
    "${IMGUI_SOURCE_DIR}/include/imgui/imgui_internal.h"
    "${IMGUI_SOURCE_DIR}/include/imgui/imconfig.h")

source_group("include" FILES)
source_group("include\\imgui" FILES ${IMGUI_INCLUDE_LIST})
source_group("source" FILES)
source_group("source\\imgui" FILES ${IMGUI_SOURCE_LIST})

add_library(imgui ${IMGUI_SOURCE_LIST} ${IMGUI_INCLUDE_LIST})
set_target_properties(imgui PROPERTIES FOLDER "imgui")
