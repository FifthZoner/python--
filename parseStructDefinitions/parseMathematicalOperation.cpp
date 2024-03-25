#include "parseStructs.hpp"
#include "../parsing.hpp"
#include "../interpretation/checks.hpp"
#include "../content/defines.hpp"

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
        if (IsVariable(parsedLine[range.first])) {
            return new ParseVariable(parsedLine[range.first]);
        }
        else {
            // value
            return new ParseValue(parsedLine[range.first]);
        }
    }
    // function check
    if (IsFunction(parsedLine[range.first]) and parsedLine[range.first + 1] == "(" and parsedLine[range.second - 1] == ")"){
        return new ParseFunction(range);
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
                ParserException("Mathematical condition invalid due to brackets!");
                return new ParseStruct();
            }
            bracketLevel--;
        }
    }
    if (bracketLevel != 0) {
        ParserException("Mathematical condition invalid due to brackets!");
        return new ParseStruct();
    }

    // checking condition types, if all aren't string or all aren't int, exception
    uint8_t type;
    if (IsConvertibleToString(parsedLine[range.first])){
        type = ParseStruct::variableString;
    }
    else if (IsConvertibleToNum(parsedLine[range.first])){
        type = ParseStruct::variableNum;
    }
    else if (IsVariable(parsedLine[range.first])){
        auto* temp = GetVariable(parsedLine[range.first]);
        if (temp == nullptr){
            ParserException("Cannot get variable for math!");
            return nullptr;
        }
        if (temp->type() == Variable::typeNum){
            type = ParseStruct::variableNum;
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
        else if (IsConvertibleToNum(parsedLine[n])){
            if (type != ParseStruct::variableNum){
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
            if (temp->type() == Variable::typeNum and type != ParseStruct::variableNum){
                ParserException("Types mismatch in math, num!");
                return nullptr;
            }
            else if (temp->type() == Variable::typeString and type != ParseStruct::variableString){
                ParserException("Types mismatch in math, string!");
                return nullptr;
            }
        }
        else if (!IsFunction(parsedLine[n]) and parsedLine[n] != "(" and parsedLine[n] != ")" and parsedLine[n] != "+" and parsedLine[n] != "-" and parsedLine[n] != "*" and parsedLine[n] != "/" and parsedLine[n] != "^"){
            ParserException("Cannot get token type! " + parsedLine[n]);
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
    ParserException("Parsing of mathematical condition failed, something is seriously wrong!");
    return new ParseStruct();
}