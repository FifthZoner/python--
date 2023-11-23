#include "parseStructs.hpp"
#include "defines.hpp"
#include "checks.hpp"

extern std::vector <std::string> parsedLine;

const uint8_t ParseStruct::type() const {
    return ParseStruct::none;
}

ParseStruct* ParseStruct::getPointer(){
    return this;
}

ParseImplicit::ParseImplicit(std::pair<unsigned int, unsigned int> range){

}
const uint8_t ParseImplicit::type() const{
    return ParseStruct::keywordImplicit;
}
