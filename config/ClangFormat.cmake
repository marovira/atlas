#================================
# Clang-format setup.
#================================
find_program(CLANG_FORMAT_EXE 
    NAMES "clang-format"
    DOC "Path to clang-format executable"
    )
if (NOT CLANG_FORMAT_EXE)
    message(STATUS "clang-format not found.")
else()
    message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")
    set(ATLAS_FILES 
        ${ATLAS_HEADER_FILES}
        ${ATLAS_SOURCE_FILES}
        )
    add_custom_target(clang-format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${ATLAS_FILES}
        )
endif()
