set(ATLAS_TEST_TOP_LIST
    ${ATLAS_TEST_ROOT}/test_main.cpp
    )

# Add all tests.
add_subdirectory(${ATLAS_TEST_ROOT}/glx)
add_subdirectory(${ATLAS_TEST_ROOT}/gui)
add_subdirectory(${ATLAS_TEST_ROOT}/hlr)
add_subdirectory(${ATLAS_TEST_ROOT}/math)

# Grab all of the test data and expected files so we can create the lists for
# the data headers.
add_subdirectory(${ATLAS_TEST_ROOT}/expected)
add_subdirectory(${ATLAS_TEST_ROOT}/test_data)

# Now make the lists with all the paths.
include(${ATLAS_CMAKE_ROOT}/CreateDataHeader.cmake)
create_expected_header(ATLAS_EXPECTED_DATA ATLAS_NUM_EXPECTED_FILES
    ATLAS_EXPECTED_FILES ATLAS_EXPECTED_NAMES "_expected")
create_expected_header(ATLAS_TEST_DATA ATLAS_NUM_DATA_FILES
    ATLAS_TEST_DATA_FILES ATLAS_TEST_DATA_NAMES)

# Finally, create the test headers.
set(ATLAS_TEST_HEADER ${ATLAS_TEST_ROOT}/test_data_paths.hpp)
configure_file(${ATLAS_TEST_ROOT}/test_data_paths.hpp.in ${ATLAS_TEST_HEADER})

set(ATLAS_EXPECTED_HEADER ${ATLAS_TEST_ROOT}/test_expected_paths.hpp)
configure_file(${ATLAS_TEST_ROOT}/test_expected_paths.hpp.in
    ${ATLAS_EXPECTED_HEADER})

set(ATLAS_TEST_TOP_GROUP ${ATLAS_TEST_TOP_LIST} PARENT_SCOPE)
set(ATLAS_TEST_GLX_GROUP ${ATLAS_TEST_GLX_LIST} PARENT_SCOPE)
set(ATLAS_TEST_GUI_GROUP ${ATLAS_TEST_GUI_LIST} PARENT_SCOPE)
set(ATLAS_TEST_HLR_GROUP ${ATLAS_TEST_HLR_LIST} PARENT_SCOPE)
set(ATLAS_TEST_MATH_GROUP ${ATLAS_TEST_MATH_LIST} PARENT_SCOPE)

set(ATLAS_TEST_HEADER_GROUP
    ${ATLAS_TEST_HEADER}
    ${ATLAS_EXPECTED_HEADER}
    PARENT_SCOPE)

set(ATLAS_TEST_LIST
    ${ATLAS_TEST_TOP_LIST}
    ${ATLAS_TEST_GLX_LIST}
    ${ATLAS_TEST_GUI_LIST}
    ${ATLAS_TEST_HLR_LIST}
    ${ATLAS_TEST_MATH_LIST}
    ${ATLAS_TEST_HEADER}
    ${ATLAS_EXPECTED_HEADER}
    PARENT_SCOPE)

