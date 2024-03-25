#include "functionStack.hpp"

#include <iostream>

extern std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

InstanceLevel::InstanceLevel(uint64_t recallLine, bool isRunning, std::string switchValue) {
    this->recallLine = recallLine;
    this->isRunning = isRunning;
    this->switchValue = switchValue;
}

#include <iostream>

Variable* NewVariable(std::string& token, long double value){
    if (!functionStack.empty()){
        if (!functionStack.top().levels.empty()){
            // leveled local variable
            functionStack.top().levels.back().variables[token] = std::unique_ptr <Variable> (new VariableNum(value));
            return functionStack.top().levels.back().variables[token].get();
        }
        else {
            // local function-wide variable
            functionStack.top().variables[token] = std::unique_ptr <Variable> (new VariableNum(value));
            return functionStack.top().variables[token].get();
        }
    }
    else if (!globalLevels.empty()){
        // leveled global variable
        globalLevels.back().variables[token] = std::unique_ptr <Variable> (new VariableNum(value));
        return globalLevels.back().variables[token].get();
    }
    else {
        // global variable
        globalVariables[token] = std::unique_ptr <Variable> (new VariableNum(value));
        return globalVariables[token].get();
    }
}

Variable* NewVariable(std::string& token, std::string& value){
    if (!functionStack.empty()){
        if (!functionStack.top().levels.empty()){
            // leveled local variable
            functionStack.top().levels.back().variables[token] = std::unique_ptr <Variable> (new VariableString(value));
            return functionStack.top().levels.back().variables[token].get();
        }
        else {
            // local function-wide variable
            functionStack.top().variables[token] = std::unique_ptr <Variable> (new VariableString(value));
            return functionStack.top().variables[token].get();
        }
    }
    else if (!globalLevels.empty()){
        // leveled global variable
        globalLevels.back().variables[token] = std::unique_ptr <Variable> (new VariableString(value));
        return globalLevels.back().variables[token].get();
    }
    else {
        // global variable
        globalVariables[token] = std::unique_ptr <Variable> (new VariableString(value));
        return globalVariables[token].get();
    }
}