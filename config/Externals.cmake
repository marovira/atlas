#================================
# External dependencies.
#================================

if (ATLAS_BUILD_DOCS)
    # Skip the graph stuff from Doxygen.
    set(DOXYGE_SKIP_DOT TRUE)
    find_package(Doxygen)
endif()

find_package(GLEW REQUIRED)
if( NOT GLEW_FOUND)
    message(FATAL_ERROR "Cannot build Atlas without GLEW. Please see the wiki for information on how to install GLEW on your platform.")
endif()
