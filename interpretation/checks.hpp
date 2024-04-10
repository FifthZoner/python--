#ifndef PYTHON___CHECKS_HPP
#define PYTHON___CHECKS_HPP

#include <cstdint>
#include <string>

#include "../parseStructDefinitions/parseStructs.hpp"

bool IsConvertibleToNum(const std::string& token);

bool IsConvertibleToString(const std::string& token);

bool IsVariable(const std::string& token);

bool IsArray(const std::string& token);

bool IsFunction(const std::string& token);

bool DoesReturnValue(const std::string& token);

bool AreBracketsValid(std::pair <unsigned int, unsigned int> range);

std::string CleanString(std::string input);

#endif //PYTHON___CHECKS_HPP
