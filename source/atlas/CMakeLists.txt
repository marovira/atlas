# Add the lower include directories
add_subdirectory("${ATLAS_SOURCE_ROOT}/glx")

# Now wrap each variable for the source group settings.
set(ATLAS_SOURCE_GLX_GROUP ${ATLAS_SOURCE_GLX_LIST} PARENT_SCOPE)

# Now assemble the final list.
set(ATLAS_SOURCE_LIST
    ${ATLAS_SOURCE_GLX_LIST}
    PARENT_SCOPE)
