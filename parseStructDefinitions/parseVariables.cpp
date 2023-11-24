#include <unordered_map>

#include "../parseStructs.hpp"
#include "../checks.hpp"
#include "../defines.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

ParseValue::ParseValue(const std::string& value){
    this->value = value;

    // checking if it's a string or not
    if (value.length() > 1 and value.starts_with('\"') and value.ends_with('\"')){
        valueType = ParseStruct::variableString;
    }
    else if (IsConvertibleToInt(value)){
        valueType = ParseStruct::variableInt;
    }
    else {
        ParserException("Value somehow could not be created!");
    }
}
const uint8_t ParseValue::type() const{
    return ParseStruct::variableValue;
}
std::string ParseValue::run() const{
    #ifdef PYTHON___DEBUG
    std::cout << "\"" << value;
    #endif
    return value;
}

ParseInt::ParseInt(const std::string& token){
    this->token = token;
}
const uint8_t ParseInt::type() const{
    return ParseStruct::variableInt;
}
Variable* ParseInt::run() const{
    #ifdef PYTHON___DEBUG
    std::cout << "New variable \"int " << token;
    #endif

    // TODO: create a variable here and return it back
    globalVariables[token] = std::unique_ptr<Variable>(new VariableInt(0));
    return globalVariables[token]->getPointer();
}

ParseString::ParseString(const std::string& token){
    this->token = token;
}
const uint8_t ParseString::type() const{
    return ParseStruct::variableString;
}

ParseVariable::ParseVariable(const std::string& token){
    this->token = token;
}
const uint8_t ParseVariable::type() const{
    return ParseStruct::variableVariable;
}
Variable* ParseVariable::run() const{

    // TODO: add global/local check here
    if (!IsGlobalVariable(token)){
        InterpreterException("Variable \"" + token + "\" does not exist!");
    }


    #ifdef PYTHON___DEBUG
    if (globalVariables[token]->type() == Variable::typeInt){
        std::cout << "\"int ";
    }
    else if (globalVariables[token]->type() == Variable::typeString){
        std::cout << "\"string ";
    }
    std::cout << token;
    #endif

    return globalVariables[token]->getPointer();
}