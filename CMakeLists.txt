cmake_minimum_required(VERSION 3.9)
project(PGSEncoder)

set(CMAKE_CXX_STANDARD 11)

find_package(PNG REQUIRED)
include_directories("/usr/local/include/libpng16")

add_executable(PGSEncoder src/main.cpp src/PGSStructures.h src/BigEdianTypes.h src/PNGImage.cpp src/PNGImage.h src/ColorTools.h src/Palette.cpp src/Palette.h)

target_link_libraries(PGSEncoder "/usr/local/lib/libpng.dylib")