cmake_minimum_required(VERSION 3.31)
project(Test)

set(CMAKE_CXX_STANDARD 23)

find_package(raylib REQUIRED)

add_executable(Test main.cpp)

target_link_libraries(Test PRIVATE raylib)