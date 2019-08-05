option(ATLAS_DOWNLOAD_DEPENDENCIES "Download dependencies for Atlas" OFF)

if (ATLAS_DOWNLOAD_DEPENDENCIES)
    set(ATLAS_LIBRARY_ROOT "${ATLAS_SOURCE_DIR}/lib")
    if (NOT EXISTS ${ATLAS_LIBRARY_ROOT})
        execute_process(COMMAND mkdir lib WORKING_DIRECTORY ${ATLAS_SOURCE_DIR})
    endif()

    if (NOT EXISTS "${ATLAS_LIBRARY_ROOT}/fmt")
        execute_process(COMMAND git clone https://github.com/fmtlib/fmt.git fmt
            WORKING_DIRECTORY ${ATLAS_LIBRARY_ROOT} OUTPUT_QUIET)
        execute_process(COMMAND git checkout 5.3.0 WORKING_DIRECTORY
            "${ATLAS_LIBRARY_ROOT}/fmt" OUTPUT_QUIET)
    endif()

    if (NOT EXISTS "${ATLAS_LIBRARY_ROOT}/glm")
        execute_process(COMMAND git clone https://github.com/g-truc/glm.git glm
            WORKING_DIRECTORY ${ATLAS_LIBRARY_ROOT} OUTPUT_QUIET)
        execute_process(COMMAND git checkout 0.9.9.5 WORKING_DIRECTORY
            "${ATLAS_LIBRARY_ROOT}/glm" OUTPUT_QUIET)
    endif()

    if (NOT EXISTS "${ATLAS_LIBRARY_ROOT}/glfw")
        execute_process(COMMAND git clone https://github.com/glfw/glfw.git glfw
            WORKING_DIRECTORY ${ATLAS_LIBRARY_ROOT} OUTPUT_QUIET)
        execute_process(COMMAND git checkout 3.3 WORKING_DIRECTORY
            "${ATLAS_LIBRARY_ROOT}/glfw" OUTPUT_QUIET)
    endif()

    if (NOT EXISTS "${ATLAS_LIBRARY_ROOT}/gl3w")
        execute_process(COMMAND git clone https://github.com/marovira/gl3w.git
            gl3w WORKING_DIRECTORY ${ATLAS_LIBRARY_ROOT} OUTPUT_QUIET)
    endif()

    add_subdirectory("${ATLAS_LIBRARY_ROOT}/fmt")
    add_subdirectory("${ATLAS_LIBRARY_ROOT}/glm")
    add_subdirectory("${ATLAS_LIBRARY_ROOT}/glfw")
    add_subdirectory("${ATLAS_LIBRARY_ROOT}/gl3w")
else()
    find_package(fmt REQUIRED)
    find_package(glm REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(gl3w REQUIRED)
endif()

# Regardless, OpenGL is a dependency and is already installed, so just pull it
# in.
find_package(OpenGL REQUIRED)
