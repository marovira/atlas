get_filename_component(EXAMPLE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

set(EXAMPLE_ROOT "${ATLAS_EXAMPLES_ROOT}/${EXAMPLE_NAME}")
set(SOURCE_LIST
    "${EXAMPLE_ROOT}/main.cpp"
    )
set(INCLUDE_LIST
    "${EXAMPLE_ROOT}/UniformLocations.glsl"
    )

set(SHADER_INCLUDE "${EXAMPLE_ROOT}/ShaderPaths.hpp")
configure_file("${EXAMPLE_ROOT}/ShaderPaths.hpp.in" ${SHADER_INCLUDE})
list(APPEND INCLUDE_LIST ${SHADER_INCLUDE})

source_group("source" FILES ${SOURCE_LIST})
source_group("include" FILES ${INCLUDE_LIST})

add_executable(${EXAMPLE_NAME} ${SOURCE_LIST} ${INCLUDE_LIST})
target_include_directories(${EXAMPLE_NAME} PUBLIC ${EXAMPLE_ROOT})
target_link_libraries(${EXAMPLE_NAME} PUBLIC atlas::atlas)
set_target_properties(${EXAMPLE_NAME} PROPERTIES FOLDER "examples")
