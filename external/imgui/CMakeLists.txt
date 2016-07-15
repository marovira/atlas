project(imgui)

cmake_minimum_required(VERSION 3.0)

set(IMGUI_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(IMGUI_BINARY_DIR "${PROJECT_BINARY_DIR}")

set(IMGUI_SOURCE_LIST
    "${IMGUI_SOURCE_DIR}/imgui/imgui.cpp"
    "${IMGUI_SOURCE_DIR}/imgui/imgui_draw.cpp")
set(IMGUI_INCLUDE_LIST
    "${IMGUI_SOURCE_DIR}/imgui/imgui.h"
    "${IMGUI_SOURCE_DIR}/imgui/imgui_internal.h"
    "${IMGUI_SOURCE_DIR}/imgui/imconfig.h")

source_group("include" FILES ${IMGUI_INCLUDE_LIST})
source_group("source" FILES ${IMGUI_SOURCE_LIST})

add_library(imgui ${IMGUI_SOURCE_LIST} ${IMGUI_INCLUDE_LIST})
set_target_properties(imgui PROPERTIES FOLDER "imgui")
