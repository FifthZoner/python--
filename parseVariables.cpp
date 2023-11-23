#include "parseStructs.hpp"
#include "checks.hpp"
#include "defines.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

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
        // error
    }
}
const uint8_t ParseValue::type() const{
    return ParseStruct::variableValue;
}

ParseInt::ParseInt(const std::string& token){
    this->token = token;
}
const uint8_t ParseInt::type() const{
    return ParseStruct::variableInt;
}
Variable* ParseInt::run(){
    #ifdef PYTHON___DEBUG
    std::cout << "New variable: int " << token;
    #endif

    // TODO: create a variable here and return it back
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
Variable* ParseVariable::run(){

    // TODO: add global/local check here

    #ifdef PYTHON___DEBUG
    std::cout << "Variable: ";
    #endif

    #ifdef PYTHON___DEBUG
    std::cout << token;
    #endif

}