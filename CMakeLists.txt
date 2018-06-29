cmake_minimum_required(VERSION 3.9)
project(PGSEncoder)

set(CMAKE_CXX_STANDARD 11)

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

SET(CMAKE_CXX_FLAGS "-O2 -std=c++11")

add_executable(PGSEncoder src/main.cpp src/PGSStructures.h src/BigEdianTypes.h)

target_link_libraries(PGSEncoder ${FREETYPE_LIBRARIES})