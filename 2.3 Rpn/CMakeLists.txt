#rm -rf .tmp   ; cmake -B .tmp && cmake --build .tmp --target install --config Release
#rd /Q /S .tmp & cmake -B .tmp && cmake --build .tmp --target install --config Release

cmake_minimum_required(VERSION 3.20)
project(Rpn
    VERSION 0.0.1
    DESCRIPTION "Arithmetic expression parser & evaluater using Reverse Polish Notation"
)

include(../Ns.cmake)

file(GLOB_RECURSE Src CONFIGURE_DEPENDS
    src/*.cpp
    src/*.hpp
)

set(CmnDir ../src)
set(Cmn
    ${CmnDir}/C_isatty.h
)

add_executable(${PROJECT_NAME} ${Src} ${Cmn})
target_include_directories(${PROJECT_NAME} BEFORE PRIVATE ${CmnDir})

Ns_install(${PROJECT_NAME} ${CMAKE_SOURCE_DIR})
Ns_VisualStudioSpecific(${PROJECT_NAME} "" "")