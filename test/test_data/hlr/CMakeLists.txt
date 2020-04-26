set(ATLAS_HLR_DATA_ROOT ${ATLAS_TEST_DATA_ROOT}/hlr)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(test_lib SHARED ${ATLAS_HLR_DATA_ROOT}/test_lib.cpp)
target_link_libraries(test_lib PRIVATE atlas-hlr)
target_include_directories(test_lib PUBLIC ${ATLAS_HLR_DATA_ROOT})
if (MSVC)
    target_compile_definitions(test_lib PRIVATE ATLAS_WINDOWS_EXPORT)
endif()
set_target_properties(test_lib PROPERTIES FOLDER "tests")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)

set(LIBRARY_HEADER "#pragma once\n\nchar const* test_library_path =
\"$<TARGET_FILE:test_lib>\"\;")
if (MSVC)
    file(GENERATE OUTPUT "${ATLAS_HLR_DATA_ROOT}/test_library_debug_path.hpp"
        CONTENT ${LIBRARY_HEADER} CONDITION $<CONFIG:DEBUG>)
    file(GENERATE OUTPUT "${ATLAS_HLR_DATA_ROOT}/test_library_release_path.hpp"
        CONTENT ${LIBRARY_HEADER} CONDITION $<CONFIG:RELEASE>)
else()
    file(GENERATE OUTPUT "${ATLAS_HLR_DATA_ROOT}/test_library_path.hpp" CONTENT
        ${LIBRARY_HEADER})
endif()
