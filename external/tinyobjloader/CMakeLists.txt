project(tinyobjloader)

cmake_minimum_required(VERSION 3.0)

set(TINY_OBJ_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
set(TINY_OBJ_BINARY_DIR "${PROJECT_BINARY_DIR}")

include_directories(
    "${TINY_OBJ_SOURCE_DIR}/include/tinyobj")
    #"${TINY_OBJ_SOURCE_DIR}/include/tinyobj/experimental")

set(TINY_OBJ_SOURCE_LIST
    "${TINY_OBJ_SOURCE_DIR}/source/tinyobj/tiny_obj_loader.cpp")
    #"${TINY_OBJ_SOURCE_DIR}/source/tinyobj/experimental/ltalloc.cc")

set(TINY_OBJ_INCLUDE_LIST
    "${TINY_OBJ_SOURCE_DIR}/include/tinyobj/tiny_obj_loader.h")
    #"${TINY_OBJ_SOURCE_DIR}/include/tinyobj/experimental/ltalloc.h"
    #"${TINY_OBJ_SOURCE_DIR}/include/tinyobj/experimental/ltalloc.hpp"
    #"${TINY_OBJ_SOURCE_DIR}/include/tinyobj/experimental/tinyobj_loader_opt.h")

source_group("include" FILES)
source_grouP("include\\tinyobj" FILES ${TINY_OBJ_INCLUDE_LIST})
source_group("source" FILES)
source_group("source\\tinyobj" FILES ${TINY_OBJ_SOURCE_LIST})

add_library(tinyobjloader ${TINY_OBJ_SOURCE_LIST} ${TINY_OBJ_INCLUDE_LIST})
set_target_properties(tinyobjloader PROPERTIES FOLDER "tinyojbloader")
