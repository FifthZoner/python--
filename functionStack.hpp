#ifndef PYTHON___FUNCTION_STACK_HPP
#define PYTHON___FUNCTION_STACK_HPP

#include <stack>
#include <unordered_map>
#include <vector>

#include "content/function.hpp"
#include "content/variables.hpp"

Variable* NewVariable(std::string& token, long long value);
Variable* NewVariable(std::string& token, std::string& value);

struct InstanceLevel {
    std::unordered_map <std::string, std::unique_ptr <Variable>> variables;
    uint64_t recallLine;
    explicit InstanceLevel(uint64_t recallLine);
};

struct FunctionInstance {
    std::vector <InstanceLevel> levels;
    std::unordered_map <std::string, std::unique_ptr <Variable>> variables;
    uint64_t currentLine;
    FunctionInstance(std::vector <std::string>& arguments, uint64_t startingLine, Function* function);
};

inline std::stack <FunctionInstance> functionStack;
inline std::vector <InstanceLevel> globalLevels;

#endif //PYTHON___FUNCTION_STACK_HPP