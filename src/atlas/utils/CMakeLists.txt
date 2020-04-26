set(ATLAS_UTILS_ROOT ${ATLAS_SOURCE_ROOT}/atlas/utils)

set(ATLAS_INCLUDE_UTILS_LIST
    ${ATLAS_UTILS_ROOT}/Cameras.hpp
    ${ATLAS_UTILS_ROOT}/LoadObjFile.hpp
    PARENT_SCOPE)

set(ATLAS_SOURCE_UTILS_LIST
    ${ATLAS_UTILS_ROOT}/tinyobjloader.cpp
    ${ATLAS_UTILS_ROOT}/LoadObjFile.cpp
    ${ATLAS_UTILS_ROOT}/Cameras.cpp
    PARENT_SCOPE)
