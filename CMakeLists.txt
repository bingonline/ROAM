cmake_minimum_required(VERSION 2.6)
project(ROAMing)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} src)
file(GLOB_RECURSE sources src/*cpp src/*c)

add_executable(ROAM ${sources})
target_link_libraries(ROAM ${SDL2_LIBRARY} ${OPENGL_LIBRARY})
