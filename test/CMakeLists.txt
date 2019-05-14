# Data files.
set(ATLAS_TEST_DATA_ROOT "${ATLAS_TEST_ROOT}/data")
set(ATLAS_TEST_DATA
    "${ATLAS_TEST_DATA_ROOT}/glx_empty_file.glsl"
    "${ATLAS_TEST_DATA_ROOT}/glx_simple_file.glsl"
    "${ATLAS_TEST_DATA_ROOT}/glx_single_line.glsl"
    "${ATLAS_TEST_DATA_ROOT}/glx_single_include.glsl"
    )

set(ATLAS_EXPECTED_DATA_ROOT "${ATLAS_TEST_ROOT}/expected")
set(ATLAS_EXPECTED_DATA
    "${ATLAS_EXPECTED_DATA_ROOT}/glx_single_line.expected"
    "${ATLAS_EXPECTED_DATA_ROOT}/glx_simple_file.expected"
    "${ATLAS_EXPECTED_DATA_ROOT}/glx_single_include.expected"
    )

# Grab the size of the lists.
list(LENGTH ATLAS_TEST_DATA ATLAS_NUM_DATA_FILES)
list(LENGTH ATLAS_EXPECTED_DATA ATLAS_NUM_EXPECTED_FILES)

# Compute the index of the final file.
MATH(EXPR n "${ATLAS_NUM_DATA_FILES}-1")

# Loop over the data files.
set(i 0)
foreach(DATA_FILE ${ATLAS_TEST_DATA})
    # First create the strings with the filenames.
    if (${i} EQUAL ${n})
        set(NAME_STRING "    \"${DATA_FILE}\"")
    else()
        set(NAME_STRING "    \"${DATA_FILE}\",")
    endif()
    list(APPEND DATA_FILES "${NAME_STRING}")

    # Now let's create the names that go in the enum
    get_filename_component(FILE_NAME ${DATA_FILE} NAME_WE)
    if (${i} EQUAL 0)
        set(NAME_STRING "    ${FILE_NAME} = 0,")
    elseif(${i} EQUAL ${n})
        set(NAME_STRING "    ${FILE_NAME}")
    else()
        set(NAME_STRING "    ${FILE_NAME},")
    endif()
    list(APPEND DATA_NAMES "${NAME_STRING}")

    MATH(EXPR i "${i}+1")
endforeach()

# Now loop over the expected files, resetting the variables.
MATH(EXPR n "${ATLAS_NUM_EXPECTED_FILES}-1")
set(i 0)
foreach(EXPECTED_FILE ${ATLAS_EXPECTED_DATA})
    # Create the strings with the filenames
    if (${i} EQUAL ${n})
        set(NAME_STRING "    \"${EXPECTED_FILE}\"")
    else()
        set(NAME_STRING "    \"${EXPECTED_FILE}\",")
    endif()
    list(APPEND EXPECTED_FILES "${NAME_STRING}")

    # Now the names for the enum
    get_filename_component(FILE_NAME ${EXPECTED_FILE} NAME_WE)
    if (${i} EQUAL 0)
        set(NAME_STRING "    ${FILE_NAME}_expected = 0,")
    elseif(${i} EQUAL ${n})
        set(NAME_STRING "    ${FILE_NAME}_expected")
    else()
        set(NAME_STRING "    ${FILE_NAME}_expected,")
    endif()
    list(APPEND EXPECTED_NAMES "${NAME_STRING}")

    MATH(EXPR i "${i}+1")
endforeach()

# Strip the semi-colons of the lists.
string(REPLACE ";" "\n" ATLAS_TEST_DATA_FILES "${DATA_FILES}")
string(REPLACE ";" "\n" ATLAS_TEST_DATA_NAMES "${DATA_NAMES}")
string(REPLACE ";" "\n" ATLAS_EXPECTED_FILES "${EXPECTED_FILES}")
string(REPLACE ";" "\n" ATLAS_EXPECTED_NAMES "${EXPECTED_NAMES}")

# Finally, create the config file and add it to the list.
set(ATLAS_TEST_HEADER "${ATLAS_TEST_ROOT}/TestDataPaths.hpp")
configure_file("${ATLAS_TEST_ROOT}/TestDataPaths.hpp.in" ${ATLAS_TEST_HEADER})

set(ATLAS_EXPECTED_HEADER "${ATLAS_TEST_ROOT}/TestExpectedPaths.hpp")
configure_file("${ATLAS_TEST_ROOT}/TestExpectedPaths.hpp.in"
    ${ATLAS_EXPECTED_HEADER})

set(ATLAS_TEST_LIST
    "${ATLAS_TEST_ROOT}/test_main.cpp"
    "${ATLAS_TEST_ROOT}/atlas_core_platform.cpp"
    "${ATLAS_TEST_ROOT}/atlas_core_enum.cpp"
    "${ATLAS_TEST_ROOT}/atlas_core_float.cpp"
    "${ATLAS_TEST_ROOT}/atlas_core_functional.cpp"
    "${ATLAS_TEST_ROOT}/atlas_core_timer.cpp"
    "${ATLAS_TEST_ROOT}/atlas_math_coordinates.cpp"
    "${ATLAS_TEST_ROOT}/atlas_math_ray.cpp"
    "${ATLAS_TEST_ROOT}/atlas_glx_gl.cpp"
    "${ATLAS_TEST_ROOT}/atlas_glx_glsl.cpp"
    ${ATLAS_TEST_HEADER}
    ${ATLAS_EXPECTED_HEADER}
    PARENT_SCOPE)
