set(ATLAS_TEST_DATA_ROOT ${ATLAS_TEST_ROOT}/test_data)

add_subdirectory(${ATLAS_TEST_DATA_ROOT}/glx)
add_subdirectory(${ATLAS_TEST_DATA_ROOT}/hlr)

set(ATLAS_TEST_DATA
    ${ATLAS_GLX_TEST_DATA_LIST}
    PARENT_SCOPE)
