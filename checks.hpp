#ifndef PYTHON___CHECKS_HPP
#define PYTHON___CHECKS_HPP

#include <string>

bool IsConvertibleToInt(const std::string& token);

bool IsConvertibleToString(const std::string& token);

bool IsGlobalVariable(const std::string& token);

bool IsLocalVariable(const std::string& token);

bool IsVariable(const std::string& token);

bool IsFunction(const std::string& token);

#endif //PYTHON___CHECKS_HPP
