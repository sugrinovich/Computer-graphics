cmake_minimum_required(VERSION 3.31)
project(Lab_1_B)

set(CMAKE_CXX_STANDARD 23)

find_package(raylib REQUIRED)

add_executable(Lab_1_B main.cpp)

target_link_libraries(Lab_1_B PRIVATE raylib)