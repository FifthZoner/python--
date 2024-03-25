#ifndef PYTHON___FUNCTION_STACK_HPP
#define PYTHON___FUNCTION_STACK_HPP

#include <stack>
#include <unordered_map>
#include <vector>

#include "../content/function.hpp"
#include "../content/variables.hpp"

Variable* NewVariable(std::string& token, long double value);
Variable* NewVariable(std::string& token, std::string& value);

struct InstanceLevel {
    std::unordered_map <std::string, std::unique_ptr <Variable>> variables;
    uint64_t recallLine;
    bool isRunning = false;
    std::string switchValue;
    InstanceLevel(uint64_t recallLine, bool isRunning, std::string switchValue = "");
};

struct FunctionInstance {
    std::vector <InstanceLevel> levels;
    std::unordered_map <std::string, std::unique_ptr <Variable>> variables;
};

inline std::stack <FunctionInstance> functionStack;
inline std::vector <InstanceLevel> globalLevels;

#endif //PYTHON___FUNCTION_STACK_HPP