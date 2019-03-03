# Add the lower include directories
add_subdirectory("${ATLAS_SOURCE_ROOT}/gl")
add_subdirectory("${ATLAS_SOURCE_ROOT}/glx")

# Now wrap each variable for the source group settings.
set(ATLAS_SOURCE_GL_GROUP ${ATLAS_SOURCE_GL_LIST} PARENT_SCOPE)
set(ATLAS_SOURCE_GLX_GROUP ${ATLAS_SOURCE_GLX_LIST} PARENT_SCOPE)
