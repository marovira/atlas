function(create_expected_header FILE_LIST OUTPUT_NUM_FILES OUTPUT_FILES
        OUTPUT_NAMES)
    list(LENGTH ${FILE_LIST} NUM_FILES)

    MATH(EXPR n "${NUM_FILES}-1")
    set(i 0)
    foreach(DATA_FILE ${${FILE_LIST}})
        # First create the strings with the filenames.
        if (${i} EQUAL ${n})
            set(NAME_STRING "    \"${DATA_FILE}\"")
        else()
            set(NAME_STRING "    \"${DATA_FILE}\",")
        endif()
        list(APPEND DATA_FILES "${NAME_STRING}")

        # Now create the names that go in the enum.
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

    string(REPLACE ";" "\n" TMP_FILES "${DATA_FILES}")
    string(REPLACE ";" "\n" TMP_NAMES "${DATA_NAMES}")

    set(${OUTPUT_NUM_FILES} ${NUM_FILES} PARENT_SCOPE)
    set(${OUTPUT_FILES} ${TMP_FILES} PARENT_SCOPE)
    set(${OUTPUT_NAMES} ${TMP_NAMES} PARENT_SCOPE)
endfunction()
