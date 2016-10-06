# Traverse the lower source directories.
add_subdirectory("${ATLAS_SOURCE_ROOT}/core")
add_subdirectory("${ATLAS_SOURCE_ROOT}/gl")
add_subdirectory("${ATLAS_SOURCE_ROOT}/utils")
add_subdirectory("${ATLAS_SOURCE_ROOT}/tools")

# Wrap each of the variables coming from the folders and pass them up.
set(ATLAS_SOURCE_CORE_LIST_PARENT ${ATLAS_SOURCE_CORE_LIST} PARENT_SCOPE)
set(ATLAS_SOURCE_GL_LIST_PARENT ${ATLAS_SOURCE_GL_LIST} PARENT_SCOPE)
set(ATLAS_SOURCE_UTILS_LIST_PARENT ${ATLAS_SOURCE_UTILS_LIST} PARENT_SCOPE)
set(ATLAS_SOURCE_TOOLS_LIST_PARENT ${ATLAS_SOURCE_TOOLS_LIST} PARENT_SCOPE)

# Now assemble the final list and pass it up.
set(ATLAS_SOURCE_LIST
    ${ATLAS_SOURCE_CORE_LIST}
    ${ATLAS_SOURCE_GL_LIST}
    ${ATLAS_SOURCE_UTILS_LIST}
    ${ATLAS_SOURCE_TOOLS_LIST}
    PARENT_SCOPE)
