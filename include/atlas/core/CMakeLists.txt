set(ATLAS_INCLUDE_CORE_LIST
    "${ATLAS_INCLUDE_ROOT}/core/Platform.hpp"
    "${ATLAS_INCLUDE_ROOT}/core/Enu.hpp"
    "${ATLAS_INCLUDE_ROOT}/core/Float.hpp"
    "${ATLAS_INCLUDE_ROOT}/core/Functional.hpp"
    "${ATLAS_INCLUDE_ROOT}/core/Timer.hpp"
    PARENT_SCOPE)

if (MSVC)
    list(APPEND ATLAS_INCLUDE_CORE_LIST
        "${ATLAS_INCLUDE_ROOT}/core/WinDebug.hpp")
endif()
