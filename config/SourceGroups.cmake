#================================
# Source groups configurations.
#================================
source_group("include" FILES)
source_group("include\\atlas" FILES ${ATLAS_INCLUDE_ROOT_LIST_PARENT})
source_group("include\\atlas\\core" FILES ${ATLAS_INCLUDE_CORE_LIST_PARENT})
source_group("include\\atlas\\gl" FILES ${ATLAS_INCLUDE_GL_LIST_PARENT})
source_group("include\\atlas\\math" FILES ${ATLAS_INCLUDE_MATH_LIST_PARENT})
source_group("include\\atlas\\utils" FILES ${ATLAS_INCLUDE_UTILS_LIST_PARENT})
source_group("include\\atlas\\tools" FILES ${ATLAS_INCLUDE_TOOLS_LIST_PARENT})

source_group("source" FILES)
source_group("source\\atlas\\core" FILES ${ATLAS_SOURCE_CORE_LIST_PARENT})
source_group("source\\atlas\\gl" FILES ${ATLAS_SOURCE_GL_LIST_PARENT})
source_group("source\\atlas\\utils" FILES ${ATLAS_SOURCE_UTILS_LIST_PARENT})
source_group("source\\atlas\\tools" FILES ${ATLAS_SOURCE_TOOLS_LIST_PARENT})
