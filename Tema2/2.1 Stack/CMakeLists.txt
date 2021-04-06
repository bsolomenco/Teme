#rm -rf .tmp   ; cmake -B .tmp && cmake --build .tmp --target install --config Release
#rd /Q /S .tmp & cmake -B .tmp && cmake --build .tmp --target install --config Release

cmake_minimum_required(VERSION 3.20)
project(Stack
    VERSION 0.0.1
    DESCRIPTION "Stack (list based)"
)

include(../../Ns.cmake)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)

file(GLOB_RECURSE Src CONFIGURE_DEPENDS
    src/*.cpp
    src/*.hpp
)

add_executable(${PROJECT_NAME} ${Src})

Ns_install(${PROJECT_NAME} ${CMAKE_SOURCE_DIR})
Ns_VisualStudioSpecific(${PROJECT_NAME} "" "")
