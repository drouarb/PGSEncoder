cmake_minimum_required(VERSION 3.0)
project(PGSEncoder)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

add_executable(PGSEncoder src/main.cpp src/PGSStructures.h src/BigEdianTypes.h src/PNGImage.cpp src/PNGImage.h src/ColorTools.h src/Palette.cpp src/Palette.h)

target_link_libraries(PGSEncoder ${PNG_LIBRARY})

install (TARGETS PGSEncoder DESTINATION bin)