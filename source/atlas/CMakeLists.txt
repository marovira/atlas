# Add the lower include directories
add_subdirectory("${ATLAS_SOURCE_ROOT}/glx")
add_subdirectory("${ATLAS_SOURCE_ROOT}/gui")
add_subdirectory("${ATLAS_SOURCE_ROOT}/hlr")

# Now wrap each variable for the source group settings.
set(ATLAS_SOURCE_GLX_GROUP ${ATLAS_SOURCE_GLX_LIST} PARENT_SCOPE)
set(ATLAS_SOURCE_GUI_GROUP ${ATLAS_SOURCE_GUI_LIST} PARENT_SCOPE)
set(ATLAS_SOURCE_HLR_GROUP ${ATLAS_SOURCE_HLR_LIST} PARENT_SCOPE)

set(ATLAS_SOURCE_FILES
    ${ATLAS_SOURCE_GLX_LIST}
    ${ATLAS_SOURCE_GUI_LIST}
    ${ATLAS_SOURCE_HLR_LIST}
    PARENT_SCOPE)
