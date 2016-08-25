cmake_minimum_required(VERSION 2.8)

project(gl3w)

include_directories(include)

add_library(gl3w src/gl3w.c)
set_target_properties(gl3w PROPERTIES FOLDER "gl3w")

if (UNIX)
  target_link_libraries(gl3w dl)
endif()

install(TARGETS gl3w ARCHIVE DESTINATION lib)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include)
