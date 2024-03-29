#include <unordered_map>

#include "parseStructs.hpp"
#include "../interpretation/checks.hpp"
#include "../content/defines.hpp"
#include "../interpretation/functionStack.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

ParseValue::ParseValue(const std::string& value){
    this->value = value;

    // checking if it's a string or not

    if (IsConvertibleToNum(value)){
        valueType = ParseStruct::variableNum;
    }
    else if (value.length() > 0){
        this->value = value.substr(1, value.size() - 2);
        valueType = ParseStruct::variableString;
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
    std::cout << value;
    #endif
    return value;
}

ParseNum::ParseNum(const std::string& token){
    this->token = token;
    if (IsVariable(token)){
        ParserException("Variable redefinition!");
    }
}
const uint8_t ParseNum::type() const{
    return ParseStruct::variableNum;
}
Variable* ParseNum::run() {
    #ifdef PYTHON___DEBUG
    std::cout << "New variable \"num " << token << "\"";
    #endif

    return NewVariable(token, 0);
}

ParseString::ParseString(const std::string& token){
    this->token = token;
    if (IsVariable(token)){
        ParserException("Variable redefinition!");
    }
}
const uint8_t ParseString::type() const{
    return ParseStruct::variableString;
}
Variable* ParseString::run() {
    #ifdef PYTHON___DEBUG
    std::cout << "New variable \"string " << token << "\"";
    #endif
    std::string temp = "0";
    return NewVariable(token, temp);
}

ParseVariable::ParseVariable(const std::string& token){
    this->token = token;
}
const uint8_t ParseVariable::type() const{
    return ParseStruct::variableVariable;
}
Variable* ParseVariable::run() const{

    // TODO: add local here
    if (IsVariable(token)){
        #ifdef PYTHON___DEBUG
        if (GetVariable(token)->type() == Variable::typeNum){
            std::cout << "\"num ";
        }
        else if (GetVariable(token)->type() == Variable::typeString){
            std::cout << "\"string ";
        }
        std::cout << token << "\"";
        #endif

        return GetVariable(token);
    }

    InterpreterException("Variable \"" + token + "\" does not exist!");
    return nullptr;
}