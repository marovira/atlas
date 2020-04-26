set(ATLAS_GLX_TEST_DATA_ROOT ${ATLAS_TEST_DATA_ROOT}/glx)

set(ATLAS_GLX_TEST_DATA_LIST
    ${ATLAS_GLX_TEST_DATA_ROOT}/glx_empty_file.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/glx_single_line.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/glx_simple_file.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/glx_simple_file_comments.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/glx_single_include.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/glx_multiple_includes.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/glx_nested_include.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/glx_circular_include.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/glx_header_comments.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/glx_c_comments.glsl

    ${ATLAS_GLX_TEST_DATA_ROOT}/nested_include.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/uniform_bindings.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/uniform_matrices.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/circular_include_a.glsl
    ${ATLAS_GLX_TEST_DATA_ROOT}/circular_include_b.glsl
    PARENT_SCOPE)
