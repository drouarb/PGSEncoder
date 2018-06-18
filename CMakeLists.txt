cmake_minimum_required(VERSION 3.9)
project(PGSEncoder)

set(CMAKE_CXX_STANDARD 11)

add_executable(PGSEncoder src/main.cpp src/PGSStructures.h src/BigEdianTypes.h)