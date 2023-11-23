#include "parseStructs.hpp"
#include "checks.hpp"
#include "defines.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::vector <std::string> parsedLine;

ParseAssign::ParseAssign(std::pair<unsigned int, unsigned int> left, std::pair<unsigned int, unsigned int> right){
    #ifdef PYTHON___DEBUG
    std::cout << "Parsing assign operator with left input:\n";
    for (unsigned int n = left.first; n < left.second; n++){
        std::cout << parsedLine[n] << " ";
    }
    std::cout << "\nRight side input:\n";
    for (unsigned int n = right.first; n < right.second; n++){
        std::cout << parsedLine[n] << " ";
    }
    std::cout << "\n";
    #endif

    // left side parsing
    if (parsedLine[left.first] == "implicit"){
        #ifdef PYTHON___DEBUG
        std::cout << "Added implicit to left\n";
        #endif
        target = std::unique_ptr<ParseStruct>(new ParseImplicit(std::pair<unsigned int, unsigned int>(left.first + 1, left.second)));
    }
    else if (left.second - left.first > 2){
        ParserException("Left side is wrong!");
        return;
    }
    if (left.second - left.first == 2){
        if (parsedLine[left.first] == "string"){
            #ifdef PYTHON___DEBUG
            std::cout << "Added string to left\n";
            #endif
            target = std::unique_ptr<ParseStruct>(new ParseString(parsedLine[left.first + 1]));
        }
        else if (parsedLine[left.first] == "int"){
            #ifdef PYTHON___DEBUG
            std::cout << "Added int to left\n";
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
            ParserException("Left side is not a variable!");
            return;
        }
    }
    else {
        ParserException("Error that I forgot what it means!");
        return;
    }


    if (right.second - left.first == 0){
        ParserException("No values on right side!");
        return;
    }

    if (right.second - right.first == 1){
        // one operator on right
        if (IsGlobalVariable(parsedLine[right.first])){
        #ifdef PYTHON___DEBUG
            std::cout << "Added global variable to right\n";
        #endif
            from = std::unique_ptr<ParseStruct>(new ParseVariable(parsedLine[right.first]));
        }
        else {
        #ifdef PYTHON___DEBUG
            std::cout << "Added value to right\n";
        #endif
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
void ParseAssign::run(){

    Variable* var = nullptr;

    switch (target->type()){
        case ParseStruct::variableVariable:
            var = reinterpret_cast <ParseVariable*>(target->getPointer())->run();
            break;
        case ParseStruct::variableInt:
            var = reinterpret_cast <ParseInt*>(target->getPointer())->run();
            break;

        default:
            InterpreterException("Wrong assignment target!");
            break;
    }

    #ifdef PYTHON___DEBUG
    std::cout << " was assigned value: ";
    #endif

}