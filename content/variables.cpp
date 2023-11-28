#include <unordered_map>

#include "variables.hpp"
#include "../exceptions.hpp"
#include "../checks.hpp"

std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

const uint8_t Variable::type() const {
    return Variable::none;
}
std::string Variable::convert(const uint8_t type) const {
    InterpreterException("Conversion of base class, should never happen!");
    return "";
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
            return "";
        case Variable::typeString:
            return value;
        default:
            InterpreterException("Invalid type to convert to!");
            return "";
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
            return "";
    }
}

Variable* GetVariable(const std::string& token){
    if (IsLocalVariable(token)){
        // nothing for now
        ParserException("Local variables are not implemented, how did you even get this error?");
        return nullptr;
    }
    else if (IsGlobalVariable(token)){
        return globalVariables[token].get();
    }
    else {
        // should generally never be reached
        ParserException("Variable of given token does not exist!");
        return nullptr;
    }
}