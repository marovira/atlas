set(ATLAS_INCLUDE_LIST
    "${ATLAS_INCLUDE_ROOT}/atlas/Macros.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/Platform.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/Constants.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/Float.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/Math.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/Log.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/Timer.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/RandomGenerator.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/Vector.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/Matrix.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/Atlas.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/Solvers.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/GLShader.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/GLHeader.hpp"
    "${ATLAS_INCLUDE_ROOT}/atlas/GLShaderInfo.hpp")

source_group("include" FILES)
source_group("include\\atlas" FILES ${ATLAS_INCLUDE_LIST})
