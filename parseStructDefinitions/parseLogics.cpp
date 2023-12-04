#include "parseStructs.hpp"
#include "../parsing.hpp"
#include "../interpretation/runtime.hpp"
#include "../checks.hpp"
#include "../interpretation/interpreter.hpp"
#include "../functionStack.hpp"

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
    if (IsConvertibleToInt(valueLeft) and IsConvertibleToInt(valueRight)){
        // int comparison
        switch (compareType){
            case CompareType::equal:
                return std::stoll(valueLeft) == std::stoll(valueRight);
            case CompareType::notEqual:
                return std::stoll(valueLeft) != std::stoll(valueRight);
            case CompareType::greater:
                return std::stoll(valueLeft) > std::stoll(valueRight);
            case CompareType::lesser:
                return std::stoll(valueLeft) < std::stoll(valueRight);
            case CompareType::greaterEqual:
                return std::stoll(valueLeft) >= std::stoll(valueRight);
            case CompareType::lesserEqual:
                return std::stoll(valueLeft) <= std::stoll(valueRight);
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
}


ParseIf::ParseIf(std::pair <unsigned int, unsigned int> range){
    condition = std::unique_ptr <ParseStruct> (new ParseCompare(range));
}

const uint8_t ParseIf::type() const {
    return ParseStruct::keywordIf;
}

void ParseIf::run() const {
    bool result = reinterpret_cast <ParseCompare*> (condition.get())->run();
    if (functionStack.empty()){
        globalLevels.emplace_back(interpreterStream->lines.size(), result);
    }
    else {
        functionStack.top().levels.emplace_back(interpreterStream->lines.size(), result);
    }
}

ParseWhile::ParseWhile(std::pair <unsigned int, unsigned int> range) {
    condition = std::unique_ptr <ParseStruct> (new ParseCompare(range));
}

const uint8_t ParseWhile::type() const {
    return ParseStruct::keywordWhile;
}
void ParseWhile::run() const {
    bool result = reinterpret_cast <ParseCompare*> (condition.get())->run();
    if (functionStack.empty()){
        globalLevels.emplace_back(interpreterStream->lines.size() - 1, result);
    }
    else {
        functionStack.top().levels.emplace_back(interpreterStream->lines.size() - 1, result);
    }
}