cmake_minimum_required(VERSION 3.26)
project(python__)

set(CMAKE_CXX_STANDARD 23)

add_executable(python__ main.cpp
        console.hpp
        console.cpp
        parsing.hpp
        parsing.cpp
        parseStructs.hpp
        parseStructs.cpp
        interpreter.hpp
        interpreter.cpp
        checks.cpp
        variables.cpp
        variables.hpp
        defines.hpp
        checks.hpp
        parseStructDefinitions/parseVariables.cpp
        parseStructDefinitions/parseOperators.cpp
        exceptions.cpp
        exceptions.hpp
        parseStructDefinitions/parseEquation.cpp
)
