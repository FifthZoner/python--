#ifndef PYTHON___RUNTIME_HPP
#define PYTHON___RUNTIME_HPP

#include <string>

#include "../parseStructDefinitions/parseStructs.hpp"

std::string RunValueReturning(ParseStruct* runnable, uint8_t type);
std::vector <std::string> RunArrayValueReturning(ParseStruct* runnable);

#endif //PYTHON___RUNTIME_HPP
