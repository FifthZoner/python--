#ifndef PYTHON___CHECKS_HPP
#define PYTHON___CHECKS_HPP

#include <cstdint>
#include <string>

#include "parseStructDefinitions/parseStructs.hpp"

bool IsConvertibleToInt(const std::string& token);

bool IsConvertibleToString(const std::string& token);

bool IsGlobalVariable(const std::string& token);

bool IsLocalVariable(const std::string& token);

bool IsVariable(const std::string& token);

bool IsFunction(const std::string& token);

bool DoesReturnValue(const std::string& token);

bool AreBracketsValid(std::pair <unsigned int, unsigned int> range);

#endif //PYTHON___CHECKS_HPP
