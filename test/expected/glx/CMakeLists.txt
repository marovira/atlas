set(ATLAS_GLX_EXPECTED_ROOT ${ATLAS_EXPECTED_ROOT}/glx)

set(ATLAS_EXPECTED_GLX_LIST
    ${ATLAS_GLX_EXPECTED_ROOT}/glx_single_line.expected
    ${ATLAS_GLX_EXPECTED_ROOT}/glx_simple_file.expected
    ${ATLAS_GLX_EXPECTED_ROOT}/glx_simple_file_comments.expected
    ${ATLAS_GLX_EXPECTED_ROOT}/glx_single_include.expected
    ${ATLAS_GLX_EXPECTED_ROOT}/glx_multiple_includes.expected
    ${ATLAS_GLX_EXPECTED_ROOT}/glx_nested_include.expected
    ${ATLAS_GLX_EXPECTED_ROOT}/glx_circular_include.expected
    ${ATLAS_GLX_EXPECTED_ROOT}/glx_header_comments.expected
    ${ATLAS_GLX_EXPECTED_ROOT}/glx_c_comments.expected
    PARENT_SCOPE)
