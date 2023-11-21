#include "parseStructs.hpp"

const uint8_t ParseStruct::type() const {
    return ParseStruct::none;
}


ParseAssign::ParseAssign(std::string left, std::string right){

}
const uint8_t ParseAssign::type() const {
    return ParseStruct::operatorAssign;
}


ParseValue::ParseValue(std::string string){

}
const uint8_t ParseValue::type() const{
    return ParseStruct::variableValue;
}

ParseUnknown::ParseUnknown(std::string string){

}
const uint8_t ParseUnknown::type() const{
    return ParseStruct::variableUnknown;
}
