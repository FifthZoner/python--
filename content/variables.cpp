#include <unordered_map>

#include "variables.hpp"
#include "../interpretation/exceptions.hpp"
#include "../interpretation/checks.hpp"
#include "../interpretation/functionStack.hpp"

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
VariableString::VariableString(std::vector <std::string>& values) {
    this->values = std::move(values);
    isArray = true;
}
const uint8_t VariableString::type() const {
    return Variable::typeString;
}
std::string VariableString::convert(const uint8_t type) const {
    switch (type){
        case Variable::typeNum:
            if (IsConvertibleToNum(value)){
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


VariableNum::VariableNum(const long double value) {
    this->value = value;
}
VariableNum::VariableNum(std::vector <long double>& values) {
    this->values = std::move(values);
    isArray = true;
}
const uint8_t VariableNum::type() const {
    return Variable::typeNum;
}
std::string VariableNum::convert(const uint8_t type) const {
    switch (type){
        case Variable::typeNum:
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
            for (long double n = functionStack.top().levels.size() - 1; n > -1; n--){
                if (auto found = functionStack.top().levels[n].variables.find(token); found != functionStack.top().levels[n].variables.end()) {
                    return functionStack.top().levels[n].variables[token].get();
                }
            }
        }
        if (auto found = functionStack.top().variables.find(token); found !=  functionStack.top().variables.end()) {
            return functionStack.top().variables[token].get();
        }
    }
    if (!globalLevels.empty()){
        for (long double n = globalLevels.size() - 1; n > -1; n--) {
            if (auto found = globalLevels[n].variables.find(token); found !=  globalLevels[n].variables.end()) {
                return globalLevels[n].variables[token].get();
            }
        }

    }
    if (auto found = globalVariables.find(token); found !=  globalVariables.end()) {
        return globalVariables[token].get();
    }

    ParserException("Variable of given token does not exist!");
    return nullptr;
}