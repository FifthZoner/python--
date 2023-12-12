cmake_minimum_required(VERSION 3.26)
project(python__)

set(CMAKE_CXX_STANDARD 23)

add_executable(python__ main.cpp
        parsing.hpp
        parsing.cpp
        parseStructDefinitions/parseStructs.hpp
        parseStructDefinitions/parseStructs.cpp
        interpretation/interpreter.hpp
        interpretation/interpreter.cpp
        interpretation/checks.cpp
        content/variables.cpp
        content/variables.hpp
        content/defines.hpp
        interpretation/checks.hpp
        parseStructDefinitions/parseVariables.cpp
        parseStructDefinitions/parseOperators.cpp
        interpretation/exceptions.cpp
        interpretation/exceptions.hpp
        parseStructDefinitions/parseMathematicalOperation.cpp
        interpretation/interpreterInterface.cpp
        interpretation/interpreterInterface.hpp
        content/function.cpp
        content/function.hpp
        parseStructDefinitions/parseImplicit.cpp
        content/pmmstdlib/printing.cpp
        content/pmmstdlib/pmmstdlib.hpp
        content/pmmstdlib/pmmstdlib.cpp
        parseStructDefinitions/parseFunction.cpp
        interpretation/runtime.cpp
        interpretation/runtime.hpp
        content/pmmstdlib/input.cpp
        parseStructDefinitions/parseLogics.cpp
        interpretation/functionStack.hpp
        interpretation/functionStack.cpp
        content/pmmstdlib/math.cpp
)
