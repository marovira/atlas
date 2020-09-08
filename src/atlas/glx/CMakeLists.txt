set(ATLAS_GLX_ROOT ${ATLAS_SOURCE_ROOT}/atlas/glx)

set(ATLAS_INCLUDE_GLX_LIST
    ${ATLAS_GLX_ROOT}/buffer.hpp
    ${ATLAS_GLX_ROOT}/context.hpp
    ${ATLAS_GLX_ROOT}/error_callback.hpp
    ${ATLAS_GLX_ROOT}/glsl.hpp
    PARENT_SCOPE)

set(ATLAS_SOURCE_GLX_LIST
    ${ATLAS_GLX_ROOT}/glsl.cpp
    ${ATLAS_GLX_ROOT}/context.cpp
    ${ATLAS_GLX_ROOT}/error_callback.cpp
    ${ATLAS_GLX_ROOT}/assert.cpp
    PARENT_SCOPE)
