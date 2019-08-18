function(create_library_header HEADER_FILE HEADER_STRING)
    get_filename_component(HEADER_DIR ${HEADER_FILE} DIRECTORY)
    get_filename_component(HEADER_NAME ${HEADER_FILE} NAME_WE)
    get_filename_component(HEADER_EXT ${HEADER_FILE} EXT)

    if (MSVC)
        set(DEBUG_HEADER "${HEADER_DIR}/${HEADER_NAME}Debug${HEADER_EXT}")
        set(RELEASE_HEADER "${HEADER_DIR}/${HEADER_NAME}Release${HEADER_EXT}")

        file(GENERATE OUTPUT ${DEBUG_HEADER} CONTENT ${HEADER_STRING} CONDITION
            $<CONFIG:DEBUG>)
        file(GENERATE OUTPUT ${RELEASE_HEADER} CONTENT ${HEADER_STRING}
            CONDITION $<CONFIG:RELEASE>)
    else()
        file(GENERATE OUTPUT ${HEADER_FILE} CONTENT ${HEADER_STRING})
    endif()
endfunction()
