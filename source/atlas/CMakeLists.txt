# Add the lower include directories
add_subdirectory("${ATLAS_SOURCE_ROOT}/glex")

# Now wrap each variable for the source group settings.
set(ATLAS_SOURCE_GLEX_GROUP ${ATLAS_SOURCE_GLEX_LIST} PARENT_SCOPE)

# Now assemble the final list.
set(ATLAS_SOURCE_LIST
    ${ATLAS_SOURCE_GLEX_LIST}
    PARENT_SCOPE)
