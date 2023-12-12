#include "parseStructs.hpp"
#include "../content/defines.hpp"
#include "../interpretation/checks.hpp"

const uint8_t ParseStruct::type() const {
    return ParseStruct::none;
}

ParseStruct* ParseStruct::getPointer(){
    return this;
}