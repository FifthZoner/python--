cmake_minimum_required(VERSION 3.26)
project(python__)

set(CMAKE_CXX_STANDARD 23)

add_executable(python__ main.cpp
        console.hpp
        console.cpp
        parsing.hpp
        parsing.cpp)
