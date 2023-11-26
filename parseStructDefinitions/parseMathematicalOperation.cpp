#include "parseStructs.hpp"
#include "../parsing.hpp"
#include "../checks.hpp"
#include "../defines.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::vector <std::string> parsedLine;

ParseStruct* ParseMathematicalOperation(std::pair <unsigned int, unsigned int> range){

    #ifdef PYTHON___DEBUG
    std::cout << "Math: ";
    for (unsigned int n = range.first; n < range.second; n++) {
        std::cout << parsedLine[n] << " ";
    }
    std::cout << "\n";
    #endif

    // variable / value handling
    if (range.second - range.first == 1){
        if (IsGlobalVariable(parsedLine[range.first]) or IsLocalVariable(parsedLine[range.first])) {
            return new ParseVariable(parsedLine[range.first]);
        }
        else {
            // value
            return new ParseValue(parsedLine[range.first]);
        }
    }

    // checking for brackets

    // this is the case when brackets take the whole equation;
    if (parsedLine[range.first] == "(" and parsedLine[range.second - 1] == ")"){
        return ParseMathematicalOperation(std::pair <unsigned int, unsigned int> (range.first + 1, range.second - 1));
    }

    unsigned int bracketLevel = 0;
    for (unsigned int n = range.first; n < range.second; n++) {
        if (parsedLine[n] == "("){
            bracketLevel++;
        }
        else if (parsedLine[n] == ")") {
            if (bracketLevel == 0){
                ParserException("Mathematical operation invalid due to brackets!");
                return new ParseStruct();
            }
            bracketLevel--;
        }
    }
    if (bracketLevel != 0) {
        ParserException("Mathematical operation invalid due to brackets!");
        return new ParseStruct();
    }

    // checking operation types, if all aren't string or all aren't int, exception
    uint8_t type;
    if (IsConvertibleToString(parsedLine[range.first])){
        type = ParseStruct::variableString;
    }
    else if (IsConvertibleToInt(parsedLine[range.first])){
        type = ParseStruct::variableInt;
    }
    else {
        ParserException("Cannot get variable type!");
        return nullptr;
    }

    for (unsigned int n = range.first + 2; n < range.second; n += 2){

    }

    // checking for operators from the last in order to first that is:
    // + -> - -> * -> / -> ^ -> (), - must be second to allow for this specific implementation

    std::pair <long, long> current(range.first, range.second);

    // example: 1 + 5 - 6 + 5 * 5 / 6 + 3
    // -> (1 + 5 - 6 + 5 * 5 / 6) + (3)
    // -> ((1 + 5 - 6) + (5 * 5 / 6)) + (3)
    // -> (((1) + (5 - 6)) + (5 * 5 / 6)) + (3)
    for (unsigned int n = current.second - 1; n >= current.first; n--){
        if (parsedLine[n] == "("){
            bracketLevel++;
        }
        else if (parsedLine[n] == ")") {
            bracketLevel--;
        }
        else if (parsedLine[n] == "+" and bracketLevel == 0){
            return new ParsePlus(std::pair <unsigned int, unsigned int> (current.first, n), std::pair <unsigned int, unsigned int> (n + 1, current.second), type);
        }
    }

    // (((1) + (5 - 6)) + (5 * 5 / 6)) + (3)
    // -> (((1) + ((5) - (6))) + (5 * 5 / 6)) + (3)
    for (unsigned int n = current.second - 1; n >= current.first; n--){
        if (parsedLine[n] == "("){
            bracketLevel++;
        }
        else if (parsedLine[n] == ")") {
            bracketLevel--;
        }
        else if (parsedLine[n] == "-" and bracketLevel == 0){
            return new ParseMinus(std::pair <unsigned int, unsigned int> (current.first, n), std::pair <unsigned int, unsigned int> (n + 1, current.second), type);
        }
    }

    // (((1) + ((5) - (6))) + (5 * 5 / 6)) + (3)
    // -> (((1) + ((5) - (6))) + ((5) * (5 / 6))) + (3)
    for (unsigned int n = current.second - 1; n >= current.first; n--){
        if (parsedLine[n] == "("){
            bracketLevel++;
        }
        else if (parsedLine[n] == ")") {
            bracketLevel--;
        }
        else if (parsedLine[n] == "*" and bracketLevel == 0){
            return new ParseMultiply(std::pair <unsigned int, unsigned int> (current.first, n), std::pair <unsigned int, unsigned int> (n + 1, current.second), type);
        }
    }

    // (((1) + ((5) - (6))) + ((5) * (5 / 6))) + (3)
    // -> (((1) + ((5) - (6))) + ((5) * ((5) / (6)))) + (3)
    for (unsigned int n = current.second - 1; n >= current.first; n--){
        if (parsedLine[n] == "("){
            bracketLevel++;
        }
        else if (parsedLine[n] == ")") {
            bracketLevel--;
        }
        else if (parsedLine[n] == "/" and bracketLevel == 0){
            return new ParseDivide(std::pair <unsigned int, unsigned int> (current.first, n), std::pair <unsigned int, unsigned int> (n + 1, current.second), type);
        }
    }

    for (unsigned int n = current.second - 1; n >= current.first; n--){
        if (parsedLine[n] == "("){
            bracketLevel++;
        }
        else if (parsedLine[n] == ")") {
            bracketLevel--;
        }
        else if (parsedLine[n] == "^" and bracketLevel == 0){
            return new ParsePower(std::pair <unsigned int, unsigned int> (current.first, n), std::pair <unsigned int, unsigned int> (n + 1, current.second), type);
        }
    }

    // brackets are handled last, so they will be executed via the first and last check at the beginning

    // this should never get here
    ParserException("Parsing of mathematical operation failed, something is seriously wrong!");
    return new ParseStruct();
}