add_custom_target(atlas_docs ALL ${DOXYGEN_EXECUTABLE}
    WORKING_DIRECTORY ${ATLAS_BINARY_DIR}/docs
    COMMENT "Generating HTML documentation" VERBATIM)
set_target_properties(atlas_docs PROPERTIES FOLDER "atlas")
