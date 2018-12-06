set(ATLAS_INCLUDE_ROOT_LIST
    "${ATLAS_INCLUDE_ROOT}/Atlas.hpp"
    )
configure_file("${ATLAS_INCLUDE_ROOT}/Atlas.hpp.in" ${ATLAS_INCLUDE_ROOT_LIST})

# Add the lower include directories
add_subdirectory("${ATLAS_INCLUDE_ROOT}/core")

# Now wrap each variable for the source group settings.
set(ATLAS_INCLUDE_ROOT_GROUP ${ATLAS_INCLUDE_ROOT_LIST})
set(ATLAS_INCLUDE_CORE_GROUP ${ATLAS_INCLUDE_CORE_LIST} PARENT_SCOPE)

# Now assemble the final list.
set(ATLAS_INCLUDE_LIST
    ${ATLAS_INCLUDE_ROOT_LIST}
    ${ATLAS_INCLUDE_CORE_LIST}
    PARENT_SCOPE)
