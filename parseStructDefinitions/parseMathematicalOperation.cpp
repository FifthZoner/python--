#include "parseStructs.hpp"
#include "../parsing.hpp"
#include "../interpretation/checks.hpp"
#include "../content/defines.hpp"
#include "../interpretation/runtime.hpp"
#include <cmath>

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

    // array values, iterate over all of this mess and return it
    if (parsedLine[range.first] == "[" and parsedLine[range.second - 1] == "]") {
        unsigned int bracketLevel = 0;
        unsigned int start = range.first + 1;
        std::vector <std::string> values;
        for (unsigned int n = range.first + 1; n < range.second - 1; n += 1) {
            // iterate but miss every ,
            // also first take all values
            if (parsedLine[n] == "(" or parsedLine[n] == "[") {
                bracketLevel++;
            }
            else if (parsedLine[n] == ")" or parsedLine[n] == "]") {
                if (bracketLevel == 0) {
                    ParserException("Too many closing brackets in array value expression!");
                }
                bracketLevel--;
            }

            if (parsedLine[n] == "," and bracketLevel == 0) {
                // take all the values prior and put them through this function again
                std::unique_ptr <ParseStruct> temp =
                        std::unique_ptr <ParseStruct> (ParseMathematicalOperation(
                                std::pair<unsigned int, unsigned int> (start, n)));
                values.push_back(RunValueReturning(temp.get(), ParseStruct::variableString));
                start = n + 1;
            }

            if (n == range.second - 2) {
                if (bracketLevel != 0) {
                    ParserException("Too few closing brackets in array value expression!");
                }
                // take all the values prior and put them through this function again
                std::unique_ptr <ParseStruct> temp =
                        std::unique_ptr <ParseStruct> (ParseMathematicalOperation(
                                std::pair<unsigned int, unsigned int> (start, n + 1)));
                values.push_back(RunValueReturning(temp.get(), ParseStruct::variableString));
            }
        }
        return new ParseValue(values);
    }

    if (range.second - range.first == 3 and IsArray(parsedLine[range.first])
        and parsedLine[range.first + 1] == ":"){
        if (parsedLine[range.first + 2] == "size") {
            Variable* arr = GetVariable(parsedLine[range.first]);
            if (arr->type() == Variable::typeNum) {
                return new ParseValue(std::to_string(reinterpret_cast<VariableNum*>(arr)->values.size()));
            }
            else {
                return new ParseValue(std::to_string(reinterpret_cast<VariableString*>(arr)->values.size()));
            }
        }
    }

    // the value is an array index
    if (range.second - range.first >= 4 and IsArray(parsedLine[range.first])
    and parsedLine[range.first + 1] == "[" and parsedLine[range.second - 1] == "]") {
        std::unique_ptr <ParseStruct> temp =
                std::unique_ptr <ParseStruct> (ParseMathematicalOperation(
                        std::pair<unsigned int, unsigned int> (range.first + 2, range.second - 1)));
        std::string indexString = RunValueReturning(temp.get(), Variable::typeNum);
        if (not IsConvertibleToNum(indexString)) {
            ParserException("Array index number cannot be a string!");
        }
        long long index = std::floor(std::stold(indexString));
        long long arraySize = 0;
        Variable* var = GetVariable(parsedLine[range.first]);
        if (var->type() == Variable::typeNum) {
            arraySize = reinterpret_cast<VariableNum*>(var)->values.size();
        }
        else {
            arraySize = reinterpret_cast<VariableString*>(var)->values.size();
        }
        if (index >= arraySize or index < -arraySize) {
            ParserException("Given index exceeds array size!");
        }
        if (index < 0) {
            index = arraySize + index;
        }
        if (var->type() == Variable::typeNum) {
            return new ParseValue(std::to_string(reinterpret_cast<VariableNum*>(var)->values[index].value));
        }
        else {
            return new ParseValue(reinterpret_cast<VariableString*>(var)->values[index].value);
        }
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
    uint8_t  type = Variable::typeNum;

    bool isFirst = true;
    bracketLevel = 0;
    for (unsigned int n = range.first; n < range.second; n++){
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
        if (bracketLevel == 0) {
            if (IsFunction(parsedLine[n])) {
                continue;
            }
            else if (IsVariable(parsedLine[n])){
                auto* temp = GetVariable(parsedLine[n]);
                if (temp == nullptr){
                    ParserException("Cannot get variable for math!");
                    return nullptr;
                }
                if (temp->type() == Variable::typeNum){
                    if (isFirst) {
                        type = Variable::typeNum;
                        isFirst = false;
                    }
                    else if (type != Variable::typeNum){
                        ParserException("Types mismatch in math, num!");
                    }
                }
                else if (temp->type() == Variable::typeString){
                    if (isFirst) {
                        type = Variable::typeString;
                        isFirst = false;
                    }
                    else if (type != ParseStruct::variableString) {
                        ParserException("Types mismatch in math, string!");
                    }
                }
            }
            else if (IsConvertibleToNum(parsedLine[n])){
                if (isFirst) {
                    type = Variable::typeNum;
                    isFirst = false;
                }
                else if (type != ParseStruct::variableNum){
                    ParserException("Wrong type in math parsing : " + parsedLine[n]);
                    return nullptr;
                }
            }
            else if (IsConvertibleToString(parsedLine[n])){
                if (isFirst) {
                    type = Variable::typeString;
                    isFirst = false;
                }
                else if (type != ParseStruct::variableString){
                    ParserException("Wrong type in math parsing : " + parsedLine[n]);
                    return nullptr;
                }
            }
            else if (!IsFunction(parsedLine[n]) and parsedLine[n] != "("
                     and parsedLine[n] != ")" and parsedLine[n] != "+" and parsedLine[n] != "-"
                     and parsedLine[n] != "*" and parsedLine[n] != "/" and parsedLine[n] != "^"
                     and parsedLine[n] != "[" and parsedLine[n] != "]" and parsedLine[n] != ","){
                ParserException("Cannot get token type! " + parsedLine[n]);
                return nullptr;
            }

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