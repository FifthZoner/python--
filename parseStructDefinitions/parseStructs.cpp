#include "parseStructs.hpp"
#include "../defines.hpp"
#include "../checks.hpp"

const uint8_t ParseStruct::type() const {
    return ParseStruct::none;
}

ParseStruct* ParseStruct::getPointer(){
    return this;
}