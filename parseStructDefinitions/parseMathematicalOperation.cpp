#include "parseStructs.hpp"
#include "../parsing.hpp"
#include "../checks.hpp"
#include "../defines.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

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
    // function check
    std::cout << "Function check\n";
    if (IsFunction(parsedLine[range.first]) and parsedLine[range.first + 1] == "(" and parsedLine[range.second - 1] == ")"){
        return new ParseFunction(range);
    }
    std::cout << "not a function!\n";
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
    else if (IsVariable(parsedLine[range.first])){
        auto* temp = GetVariable(parsedLine[range.first]);
        if (temp == nullptr){
            ParserException("Cannot get variable for math!");
            return nullptr;
        }
        if (temp->type() == Variable::typeInt){
            type = ParseStruct::variableInt;
        }
        else {
            type = ParseStruct::variableString;
        }
    }
    else {
        ParserException("Cannot get token type!");
        return nullptr;
    }

    for (unsigned int n = range.first + 1; n < range.second; n++){
        if (IsConvertibleToString(parsedLine[n])){
            if (type != ParseStruct::variableString){
                ParserException("Wrong type in math parsing!");
                return nullptr;
            }
        }
        else if (IsConvertibleToInt(parsedLine[n])){
            if (type != ParseStruct::variableInt){
                ParserException("Wrong type in math parsing!");
                return nullptr;
            }
        }
        else if (IsVariable(parsedLine[n])){
            auto* temp = GetVariable(parsedLine[n]);
            if (temp == nullptr){
                ParserException("Cannot get variable for math!");
                return nullptr;
            }
            if (temp->type() == Variable::typeInt and type != ParseStruct::variableInt){
                ParserException("Types mismatch in math!");
                return nullptr;
            }
            else if (type != ParseStruct::variableString){
                ParserException("Types mismatch in math!");
                return nullptr;
            }
        }
        else if (parsedLine[n] != "(" and parsedLine[n] != ")" and parsedLine[n] != "+" and parsedLine[n] != "-" and parsedLine[n] != "*" and parsedLine[n] != "/" and parsedLine[n] != "^"){
            ParserException("Cannot get token type!");
            return nullptr;
        }

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