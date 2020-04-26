set(ATLAS_HLR_ROOT ${ATLAS_SOURCE_ROOT}/atlas/hlr)

set(ATLAS_INCLUDE_HLR_LIST
    ${ATLAS_HLR_ROOT}/cr.h
    ${ATLAS_HLR_ROOT}/load_library.hpp
    PARENT_SCOPE)

set(ATLAS_SOURCE_HLR_LIST
    ${ATLAS_HLR_ROOT}/load_library.cpp
    PARENT_SCOPE)
