#include <iostream>
#include "parseStructs.hpp"
#include "../parsing.hpp"
#include "../interpretation/runtime.hpp"
#include "../interpretation/checks.hpp"
#include "../interpretation/interpreter.hpp"
#include "../interpretation/functionStack.hpp"
#include "../content/pmmstdlib/pmmstdlib.hpp"

enum CompareType {
    equal, greater, lesser, notEqual, greaterEqual, lesserEqual, none
};

ParseCompare::ParseCompare(std::pair <unsigned int, unsigned int> range) {
    // first a search for the operator is needed
    compareType = CompareType::none;
    for (unsigned int n = range.first; n < range.second; n++){
        if (CheckTokenType(parsedLine[n]) == ParseStruct::operatorCompare){
            if (parsedLine[n] == "=="){
                compareType = CompareType::equal;
            }
            else if (parsedLine[n] == "!="){
                compareType = CompareType::notEqual;
            }
            else if (parsedLine[n] == ">="){
                compareType = CompareType::greaterEqual;
            }
            else if (parsedLine[n] == "<="){
                compareType = CompareType::lesserEqual;
            }
            else if (parsedLine[n] == ">"){
                compareType = CompareType::greater;
            }
            else if (parsedLine[n] == "<"){
                compareType = CompareType::lesser;
            }
            left = std::unique_ptr<ParseStruct>(ParseMathematicalOperation(std::pair <unsigned int, unsigned int> (range.first, n)));
            right = std::unique_ptr<ParseStruct>(ParseMathematicalOperation(std::pair <unsigned int, unsigned int> (n + 1, range.second)));
            return;
        }
    }
    ParserException("Could not find any correct operator in comparison!");
}

[[nodiscard]] const uint8_t ParseCompare::type() const {
    return ParseStruct::operatorCompare;
}

[[nodiscard]] bool ParseCompare::run() const {
    std::string valueLeft, valueRight;
    valueLeft = RunValueReturning(left.get(), Variable::none);
    valueRight = RunValueReturning(right.get(), Variable::none);
    if (IsConvertibleToNum(valueLeft) and IsConvertibleToNum(valueRight)){
        // int comparison
        switch (compareType){
            case CompareType::equal:
                return std::stold(valueLeft) == std::stold(valueRight);
            case CompareType::notEqual:
                return std::stold(valueLeft) != std::stold(valueRight);
            case CompareType::greater:
                return std::stold(valueLeft) > std::stold(valueRight);
            case CompareType::lesser:
                return std::stold(valueLeft) < std::stold(valueRight);
            case CompareType::greaterEqual:
                return std::stold(valueLeft) >= std::stold(valueRight);
            case CompareType::lesserEqual:
                return std::stold(valueLeft) <= std::stold(valueRight);
        }
    }
    // string comparison
    switch (compareType) {
        case CompareType::equal:
            return valueLeft == valueRight;
        case CompareType::notEqual:
            return valueLeft != valueRight;
        case CompareType::greater:
            return valueLeft.length() > valueRight.length();
        case CompareType::lesser:
            return valueLeft.length() < valueRight.length();
        case CompareType::greaterEqual:
            return valueLeft.length() >= valueRight.length();
        case CompareType::lesserEqual:
            return valueLeft.length() <= valueRight.length();
    }
    return false;
}


ParseIf::ParseIf(std::pair <unsigned int, unsigned int> range, unsigned long long recall){
    condition = std::unique_ptr <ParseStruct> (new ParseCompare(range));
    this->recall = recall;
}

const uint8_t ParseIf::type() const {
    return ParseStruct::keywordIf;
}

bool ParseIf::run() const {
    bool result = reinterpret_cast <ParseCompare*> (condition.get())->run();
    if (functionStack.empty()){
        globalLevels.emplace_back(recall, result);
    }
    else {
        functionStack.top().levels.emplace_back(recall, result);
    }
    return result;
}

ParseReturn::ParseReturn(std::pair <unsigned int, unsigned int> range) {
    if (parsedLine[range.first] != "return"){
        ParserException("Wrong keyword in return!");
        return;
    }
    if (functionStack.empty()){
        InterpreterException("Parsing return statement outside function!");
    }
    if (range.second - range.first > 1) {
        value = std::unique_ptr <ParseStruct> (ParseMathematicalOperation({range.first + 1, range.second}));
    }
    else {
        // valueless return
        value = nullptr;
    }
}

const uint8_t ParseReturn::type() const {
    return ParseStruct::keywordReturn;
}

void ParseReturn::run() {
    if (value == nullptr) {
        // just to be safe
        returnValueString = "0";
    }
    if (functionStack.empty()){
        InterpreterException("Running return statement outside function!");
    }
    returnValueString = RunValueReturning(value.get(), Variable::none);
}

ParseWhile::ParseWhile(std::pair <unsigned int, unsigned int> range, unsigned long long recall) {
    condition = std::unique_ptr <ParseStruct> (new ParseCompare(range));
    this->recall = recall;
}

const uint8_t ParseWhile::type() const {
    return ParseStruct::keywordWhile;
}

bool ParseWhile::run() const {
    bool result = reinterpret_cast <ParseCompare*> (condition.get())->run();
    if (functionStack.empty()){
        globalLevels.emplace_back(recall, result);
    }
    else {
        functionStack.top().levels.emplace_back(recall, result);
    }
    return result;
}