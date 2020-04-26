set(ATLAS_UTILS_ROOT ${ATLAS_SOURCE_ROOT}/atlas/utils)

set(ATLAS_INCLUDE_UTILS_LIST
    ${ATLAS_UTILS_ROOT}/cameras.hpp
    ${ATLAS_UTILS_ROOT}/load_obj_file.hpp
    PARENT_SCOPE)

set(ATLAS_SOURCE_UTILS_LIST
    ${ATLAS_UTILS_ROOT}/tinyobjloader.cpp
    ${ATLAS_UTILS_ROOT}/load_obj_file.cpp
    ${ATLAS_UTILS_ROOT}/cameras.cpp
    PARENT_SCOPE)
