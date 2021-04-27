project(imgui)

set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(IMGUI_SOURCE_LIST
    "${IMGUI_SOURCE_DIR}/imgui.cpp"
    "${IMGUI_SOURCE_DIR}/imgui_demo.cpp"
    "${IMGUI_SOURCE_DIR}/imgui_draw.cpp"
    "${IMGUI_SOURCE_DIR}/imgui_tables.cpp"
    "${IMGUI_SOURCE_DIR}/imgui_widgets.cpp"
    )
set(IMGUI_INCLUDE_LIST
    "${IMGUI_SOURCE_DIR}/imconfig.h"
    "${IMGUI_SOURCE_DIR}/imgui.h"
    "${IMGUI_SOURCE_DIR}/imgui_internal.h"
    "${IMGUI_SOURCE_DIR}/imstb_rectpack.h"
    "${IMGUI_SOURCE_DIR}/imstb_textedit.h"
    "${IMGUI_SOURCE_DIR}/imstb_truetype.h"
    )

source_group("include" FILES ${IMGUI_INCLUDE_LIST})
source_group("source" FILES ${IMGUI_SOURCE_LIST})

add_library(imgui ${IMGUI_SOURCE_LIST} ${IMGUI_INCLUDE_LIST})
target_include_directories(imgui PUBLIC ${IMGUI_SOURCE_DIR})
target_compile_features(imgui PUBLIC cxx_std_17)
set_target_properties(imgui PROPERTIES FOLDER "imgui")
