#================================
# External dependencies.
#================================

if (ATLAS_BUILD_DOCS)
    # Skip the graph stuff from Doxygen.
    set(DOXYGE_SKIP_DOT TRUE)
    find_package(Doxygen)
endif()

