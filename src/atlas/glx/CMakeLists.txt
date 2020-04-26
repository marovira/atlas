set(ATLAS_GLX_ROOT ${ATLAS_SOURCE_ROOT}/atlas/glx)

set(ATLAS_INCLUDE_GLX_LIST
    ${ATLAS_GLX_ROOT}/Buffer.hpp
    ${ATLAS_GLX_ROOT}/Context.hpp
    ${ATLAS_GLX_ROOT}/ErrorCallback.hpp
    ${ATLAS_GLX_ROOT}/GLSL.hpp
    PARENT_SCOPE)

set(ATLAS_SOURCE_GLX_LIST
    ${ATLAS_GLX_ROOT}/GLSL.cpp
    ${ATLAS_GLX_ROOT}/Context.cpp
    ${ATLAS_GLX_ROOT}/ErrorCallback.cpp
    PARENT_SCOPE)
