project(stb)

set(STB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

set(STB_SOURCE_LIST
    "${STB_SOURCE_DIR}/stb_image.cpp"
    "${STB_SOURCE_DIR}/stb_image_resize.cpp"
    "${STB_SOURCE_DIR}/stb_image_write.cpp"
    )
set(STB_INCLUDE_LIST
    "${STB_SOURCE_DIR}/stb_image.h"
    "${STB_SOURCE_DIR}/stb_image_resize.h"
    "${STB_SOURCE_DIR}/stb_image_write.h"
    )

source_group("include" FILES ${STB_INCLUDE_LIST})
source_group("source" FILES ${STB_SOURCE_LIST})

add_library(stb ${STB_SOURCE_LIST} ${IMGUI_INCLUDE_LIST})
target_include_directories(stb PUBLIC ${STB_SOURCE_DIR})
target_compile_features(stb PUBLIC cxx_std_17)
set_target_properties(stb PROPERTIES FOLDER "stb")
