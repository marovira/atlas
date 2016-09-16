#================================
# External dependencies.
#================================
if (ATLAS_BUILD_DOCS)
    # Skip the graph stuff from Doxygen.
    set(DOXYGE_SKIP_DOT TRUE)
    find_package(Doxygen)
endif()

# Since we are no longer using GLEW, we need to manually include the 
# OpenGL library ourselves.
find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
    message(STATUS "Atlas requires OpenGL to run.")
endif()
