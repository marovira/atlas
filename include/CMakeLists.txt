set(ATLAS_INCLUDE_LIST
    "${ATLAS_INCLUDE_ROOT}/Macros.hpp"
    "${ATLAS_INCLUDE_ROOT}/Platform.hpp"
    "${ATLAS_INCLUDE_ROOT}/Constants.hpp"
    "${ATLAS_INCLUDE_ROOT}/Float.hpp"
    "${ATLAS_INCLUDE_ROOT}/Math.hpp"
    "${ATLAS_INCLUDE_ROOT}/Log.hpp"
    "${ATLAS_INCLUDE_ROOT}/Timer.hpp"
    "${ATLAS_INCLUDE_ROOT}/RandomGenerator.hpp"
    "${ATLAS_INCLUDE_ROOT}/Vector.hpp"
    "${ATLAS_INCLUDE_ROOT}/Matrix.hpp"
    "${ATLAS_INCLUDE_ROOT}/Atlas.hpp"
    "${ATLAS_INCLUDE_ROOT}/Solvers.hpp"
    "${ATLAS_INCLUDE_ROOT}/GLShader.hpp"
    "${ATLAS_INCLUDE_ROOT}/GLHeader.hpp"
    "${ATLAS_INCLUDE_ROOT}/GLShaderInfo.hpp")

source_group("include" FILES)
source_group("include\\atlas" FILES ${ATLAS_INCLUDE_LIST})
