#include <unordered_map>

#include "variables.hpp"
#include "../exceptions.hpp"
#include "../checks.hpp"
#include "../functionStack.hpp"

std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

const uint8_t Variable::type() const {
    return Variable::none;
}
std::string Variable::convert(const uint8_t type) const {
    InterpreterException("Conversion of base class, should never happen!");
    return "0";
}
Variable* Variable::getPointer(){
    return this;
}

VariableString::VariableString(const std::string& value) {
    this->value = value;
}
const uint8_t VariableString::type() const {
    return Variable::typeString;
}
std::string VariableString::convert(const uint8_t type) const {
    switch (type){
        case Variable::typeInt:
            if (IsConvertibleToInt(value)){
                return value;
            }
            InterpreterException("This string is inconvertible to integer!");
            return "0";
        case Variable::typeString:
            return value;
        default:
            InterpreterException("Invalid type to convert to!");
            return "0";
    }
}


VariableInt::VariableInt(const long long value) {
    this->value = value;
}
const uint8_t VariableInt::type() const {
    return Variable::typeInt;
}
std::string VariableInt::convert(const uint8_t type) const {
    switch (type){
        case Variable::typeInt:
            return std::to_string(value);
        case Variable::typeString:
            return std::to_string(value);
        default:
            InterpreterException("Invalid type to convert to!");
            return "0";
    }
}

Variable* GetVariable(const std::string& token){

    if (!IsVariable(token)){
        ParserException("Variable of given token does not exist!");
        return nullptr;
    }

    if (!functionStack.empty()){
        if (!functionStack.top().levels.empty()){
            if (auto found = functionStack.top().levels.back().variables.find(token); found !=  functionStack.top().levels.back().variables.end()) {
                return functionStack.top().levels.back().variables[token].get();
            }
        }
        if (auto found = functionStack.top().variables.find(token); found !=  functionStack.top().variables.end()) {
            return functionStack.top().variables[token].get();
        }
    }
    if (!globalLevels.empty()){
        if (auto found = globalLevels.back().variables.find(token); found !=  globalLevels.back().variables.end()) {
            return globalLevels.back().variables[token].get();
        }
    }
    if (auto found = globalVariables.find(token); found !=  globalVariables.end()) {
        return globalVariables[token].get();
    }
}