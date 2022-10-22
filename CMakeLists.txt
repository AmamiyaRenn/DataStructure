cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)

project(DS)
add_executable(DS main.cpp)