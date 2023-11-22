#include "parseStructs.hpp"

extern std::vector <std::string> parsedLine;

const uint8_t ParseStruct::type() const {
    return ParseStruct::none;
}

ParseAssign::ParseAssign(std::pair<unsigned int, unsigned int> left, std::pair<unsigned int, unsigned int> right){
    // left - destination, should be 1 value
    if (left.second - left.first != 1){
        // TODO: wrong left side error
    }

    if (right.second - left.first < 1){
        // TODO: wrong right side error
    }

    if (right.second - right.first == 1){
        // one operator on right
        // TODO: check if it's a value or token and add it to AST tree
    }
}
const uint8_t ParseAssign::type() const {
    return ParseStruct::operatorAssign;
}


ParseValue::ParseValue(std::pair<unsigned int, unsigned int> range){

}
const uint8_t ParseValue::type() const{
    return ParseStruct::variableValue;
}

ParseUnknown::ParseUnknown(std::pair<unsigned int, unsigned int> range){

}
const uint8_t ParseUnknown::type() const{
    return ParseStruct::variableUnknown;
}
