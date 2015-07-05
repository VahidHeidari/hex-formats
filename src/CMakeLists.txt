cmake_minimum_required(VERSION 2.8)
project(hex-formats)
add_definitions(-g -Wall -Wextra -std=c++11 -save-temps)
add_executable(hex-formats main.cpp intel-hex.cpp)
