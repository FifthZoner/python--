#include "parseStructs.hpp"
#include "checks.hpp"
#include "defines.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::vector <std::string> parsedLine;

ParseAssign::ParseAssign(std::pair<unsigned int, unsigned int> left, std::pair<unsigned int, unsigned int> right){
    // left side parsing
    if (parsedLine[left.first] == "implicit"){
        #ifdef PYTHON___DEBUG
        std::cout << "Added implicit\n";
        #endif
        target = std::unique_ptr<ParseStruct>(new ParseImplicit(std::pair<unsigned int, unsigned int>(left.first + 1, left.second)));
    }
    else if (left.second - left.first > 2){
        // error
    }
    if (left.second - left.first == 2){
        if (parsedLine[left.first] == "string"){
            #ifdef PYTHON___DEBUG
            std::cout << "Added string\n";
            #endif
            target = std::unique_ptr<ParseStruct>(new ParseString(parsedLine[left.first + 1]));
        }
        else if (parsedLine[left.first] == "int"){
            #ifdef PYTHON___DEBUG
            std::cout << "Added int\n";
            #endif
            target = std::unique_ptr<ParseStruct>(new ParseInt(parsedLine[left.first + 1]));
        }
    }
    else if (left.second - left.first == 1){
        // existing variable check
        if (IsGlobalVariable(parsedLine[left.first])){
            target = std::unique_ptr<ParseStruct>(new ParseVariable(parsedLine[left.first]));
        }
        else {
            // error
        }
    }
    else {
        // error
    }


    if (right.second - left.first == 0){
        // error
    }

    if (right.second - right.first == 1){
        // one operator on right
        if (IsGlobalVariable(parsedLine[right.first])){
            from = std::unique_ptr<ParseStruct>(new ParseVariable(parsedLine[right.first]));
        }
        else {
            from = std::unique_ptr<ParseStruct>(new ParseValue(parsedLine[right.first]));
        }
    }
    else {
        // TODO: Add more complex equations here
    }
}
const uint8_t ParseAssign::type() const {
    return ParseStruct::operatorAssign;
}