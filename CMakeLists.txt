cmake_minimum_required(VERSION 3.10)
project(GUI)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/Source)
file(GLOB_RECURSE SOURCE_CPP ${SOURCE_DIR}/*.cpp)
include_directories(${SOURCE_DIR})

add_compile_definitions(WIN32 _WINDOWS _UNICODE UNICODE)

add_subdirectory(Samples/Borderless Borderless)
add_subdirectory(Samples/ClipChild  ClipChild)
add_subdirectory(Samples/Controls   Controls)
add_subdirectory(Samples/D2D        D2D)
add_subdirectory(Samples/DDC        DDC)
add_subdirectory(Samples/GDI        GDI)
add_subdirectory(Samples/Layered    Layered)
add_subdirectory(Samples/Ruler      Ruler)