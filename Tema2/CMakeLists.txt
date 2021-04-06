#rm -rf .tmp   ; cmake -B .tmp && cmake --build .tmp --target install --config Release
#rd /Q /S .tmp & cmake -B .tmp && cmake --build .tmp --target install --config Release

cmake_minimum_required(VERSION 3.20)
project(Teme
    VERSION 1.0.0
    DESCRIPTION "Teme"
)

include(../Ns.cmake)

add_subdirectory("2.1 Stack")
add_subdirectory("2.2 ParensMatch")
add_subdirectory("2.3 Rpn")
add_subdirectory("2.8 Connex")
add_subdirectory("2.9 Maze")

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)
