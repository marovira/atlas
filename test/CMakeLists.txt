# Data files.
set(ATLAS_TEST_DATA_ROOT "${ATLAS_TEST_ROOT}/data")
set(ATLAS_TEST_DATA
    "${ATLAS_TEST_DATA_ROOT}/glx_empty_file.glsl"
    "${ATLAS_TEST_DATA_ROOT}/glx_single_line.glsl"
    "${ATLAS_TEST_DATA_ROOT}/glx_simple_file.glsl"
    "${ATLAS_TEST_DATA_ROOT}/glx_simple_file_comments.glsl"
    "${ATLAS_TEST_DATA_ROOT}/glx_single_include.glsl"
    "${ATLAS_TEST_DATA_ROOT}/glx_multiple_includes.glsl"
    "${ATLAS_TEST_DATA_ROOT}/glx_nested_include.glsl"
    "${ATLAS_TEST_DATA_ROOT}/glx_circular_include.glsl"

    "${ATLAS_TEST_DATA_ROOT}/nested_include.glsl"
    "${ATLAS_TEST_DATA_ROOT}/uniform_bindings.glsl"
    "${ATLAS_TEST_DATA_ROOT}/uniform_matrices.glsl"
    "${ATLAS_TEST_DATA_ROOT}/circular_include_a.glsl"
    "${ATLAS_TEST_DATA_ROOT}/circular_include_b.glsl"
    )

set(ATLAS_EXPECTED_DATA_ROOT "${ATLAS_TEST_ROOT}/expected")
set(ATLAS_EXPECTED_DATA
    "${ATLAS_EXPECTED_DATA_ROOT}/glx_single_line.expected"
    "${ATLAS_EXPECTED_DATA_ROOT}/glx_simple_file.expected"
    "${ATLAS_EXPECTED_DATA_ROOT}/glx_simple_file_comments.expected"
    "${ATLAS_EXPECTED_DATA_ROOT}/glx_single_include.expected"
    "${ATLAS_EXPECTED_DATA_ROOT}/glx_multiple_includes.expected"
    "${ATLAS_EXPECTED_DATA_ROOT}/glx_nested_include.expected"
    "${ATLAS_EXPECTED_DATA_ROOT}/glx_circular_include.expected"
    )

include("${ATLAS_CMAKE_ROOT}/CreateDataHeader.cmake")
create_expected_header(ATLAS_EXPECTED_DATA ATLAS_NUM_EXPECTED_FILES
    ATLAS_EXPECTED_FILES ATLAS_EXPECTED_NAMES "_expected")
create_expected_header(ATLAS_TEST_DATA ATLAS_NUM_DATA_FILES
    ATLAS_TEST_DATA_FILES ATLAS_TEST_DATA_NAMES)

# Finally, create the config file and add it to the list.
set(ATLAS_TEST_HEADER "${ATLAS_TEST_ROOT}/TestDataPaths.hpp")
configure_file("${ATLAS_TEST_ROOT}/TestDataPaths.hpp.in" ${ATLAS_TEST_HEADER})

set(ATLAS_EXPECTED_HEADER "${ATLAS_TEST_ROOT}/TestExpectedPaths.hpp")
configure_file("${ATLAS_TEST_ROOT}/TestExpectedPaths.hpp.in"
    ${ATLAS_EXPECTED_HEADER})

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(test_lib SHARED ${ATLAS_TEST_ROOT}/test_lib.cpp)
target_link_libraries(test_lib PRIVATE atlas-hlr)
if (MSVC)
    target_compile_definitions(test_lib PRIVATE ATLAS_WINDOWS_EXPORT)
endif()
set_target_properties(test_lib PROPERTIES FOLDER "tests")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)

set(LIBRARY_HEADER "#pragma once\n\nchar const* TestLibraryPath =
\"$<TARGET_FILE:test_lib>\"\;")
if (MSVC)
    file(GENERATE OUTPUT "${ATLAS_TEST_ROOT}/TestLibraryDebugPath.hpp" CONTENT
        ${LIBRARY_HEADER} CONDITION $<CONFIG:DEBUG>)
    file(GENERATE OUTPUT "${ATLAS_TEST_ROOT}/TestLibraryReleasePath.hpp" CONTENT
        ${LIBRARY_HEADER} CONDITION $<CONFIG:RELEASE>)
else()
    file(GENERATE OUTPUT "${ATLAS_TEST_ROOT}/TestLibraryPath.hpp" CONTENT
        ${LIBRARY_HEADER})
endif()

set(ATLAS_TEST_LIST
    "${ATLAS_TEST_ROOT}/test_main.cpp"
    "${ATLAS_TEST_ROOT}/atlas_core_platform.cpp"
    "${ATLAS_TEST_ROOT}/atlas_core_float.cpp"
    "${ATLAS_TEST_ROOT}/atlas_core_functional.cpp"
    "${ATLAS_TEST_ROOT}/atlas_core_timer.cpp"
    "${ATLAS_TEST_ROOT}/atlas_math_coordinates.cpp"
    "${ATLAS_TEST_ROOT}/atlas_math_ray.cpp"
    "${ATLAS_TEST_ROOT}/atlas_glx_glsl.cpp"
    "${ATLAS_TEST_ROOT}/atlas_glx_context.cpp"
    "${ATLAS_TEST_ROOT}/atlas_gui_gui.cpp"
    "${ATLAS_TEST_ROOT}/atlas_hlr_loadlibrary.cpp"
    ${ATLAS_TEST_HEADER}
    ${ATLAS_EXPECTED_HEADER}
    PARENT_SCOPE)

