#include <unordered_map>
#include <cmath>

#include "parseStructs.hpp"
#include "../interpretation/checks.hpp"
#include "../content/defines.hpp"
#include "../parsing.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;


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

    bool isImplicit = false;

    // left side parsing
    if (parsedLine[left.first] == "implicit"){
        isImplicit = true;
        left.first++;
        #ifdef PYTHON___DEBUG
        std::cout << "Added implicit to left\n";
        #endif
    }
    if (left.second - left.first > 2){
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
        else if (parsedLine[left.first] == "num"){
            #ifdef PYTHON___DEBUG
            std::cout << "Added int to left\n";
            #endif
            target = std::unique_ptr<ParseStruct>(new ParseNum(parsedLine[left.first + 1]));
        }
    }
    else if (left.second - left.first == 1){
        // existing variable check
        if (IsVariable(parsedLine[left.first])){
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

    if (isImplicit){
        from = std::unique_ptr <ParseStruct> (new ParseImplicit(right));
        return;
    }

    from = std::unique_ptr <ParseStruct> (ParseMathematicalOperation(right));
}
const uint8_t ParseAssign::type() const {
    return ParseStruct::operatorAssign;
}
void ParseAssign::run() const{

    Variable* var = nullptr;

    switch (target->type()){
        case ParseStruct::variableVariable:
            var = reinterpret_cast <ParseVariable*>(target->getPointer())->run();
            break;
        case ParseStruct::variableNum:
            var = reinterpret_cast <ParseNum*>(target->getPointer())->run();
            break;
        case ParseStruct::variableString:
            var = reinterpret_cast <ParseString*>(target->getPointer())->run();
            break;

        default:
            InterpreterException("Wrong assignment target!");
            return;
    }

    #ifdef PYTHON___DEBUG
    std::cout << " was assigned value ";
    #endif

    switch (from->type()){
        case ParseStruct::variableValue:

            switch (var->type()){
                case Variable::typeNum:
                    reinterpret_cast <VariableNum*>(var->getPointer())->value = std::stold(reinterpret_cast <ParseValue*>(from->getPointer())->run());
                    break;
                case Variable::typeString:
                    reinterpret_cast <VariableString*>(var->getPointer())->value = reinterpret_cast <ParseValue*>(from->getPointer())->run();
                    break;
            }
            break;
        case ParseStruct::variableVariable:
            switch (var->type()){
                case Variable::typeNum:
                    reinterpret_cast <VariableNum*>(var->getPointer())->value = reinterpret_cast <VariableNum*> (reinterpret_cast <ParseVariable*> (from->getPointer())->run())->value;
                    break;
                case Variable::typeString:
                    reinterpret_cast <VariableString*>(var->getPointer())->value = reinterpret_cast <VariableString*> (reinterpret_cast <ParseVariable*> (from->getPointer())->run())->value;
                    break;
            }
            break;
        case ParseStruct::keywordImplicit:
            switch (var->type()){
                case Variable::typeNum:
                    reinterpret_cast <VariableNum*>(var->getPointer())->value = std::stold(reinterpret_cast<ParseImplicit*>(from.get())->run(ParseStruct::variableNum));
                    break;
                case Variable::typeString:
                    reinterpret_cast <VariableString*>(var->getPointer())->value = reinterpret_cast<ParseImplicit*>(from.get())->run(ParseStruct::variableString);
                    break;
            }
            break;
        case ParseStruct::operatorPlus:
            switch (var->type()){
                case Variable::typeNum:
                    reinterpret_cast <VariableNum*>(var->getPointer())->value = std::stold(reinterpret_cast <ParsePlus*> (from->getPointer())->run());
                    break;
                case Variable::typeString:
                    reinterpret_cast <VariableString*>(var->getPointer())->value = reinterpret_cast <ParsePlus*> (from->getPointer())->run();
                    break;
            }
            break;
        case ParseStruct::operatorMinus:
            switch (var->type()){
                case Variable::typeNum:
                    reinterpret_cast <VariableNum*>(var->getPointer())->value = std::stold(reinterpret_cast <ParseMinus*> (from->getPointer())->run());
                    break;
                case Variable::typeString:
                    InterpreterException("Cannot subtract strings!");
                    return;
            }
            break;
        case ParseStruct::operatorFunction:
            switch (var->type()){
                case Variable::typeNum:
                    reinterpret_cast <VariableNum*>(var->getPointer())->value = std::stold(reinterpret_cast <ParseFunction*> (from->getPointer())->run());
                    break;
                case Variable::typeString:
                    reinterpret_cast <VariableString*>(var->getPointer())->value = reinterpret_cast <ParseFunction*> (from->getPointer())->run();
                    return;
            }
            break;
        case ParseStruct::operatorMultiply:
            switch (var->type()){
                case Variable::typeNum:
                    reinterpret_cast <VariableNum*>(var->getPointer())->value = std::stold(reinterpret_cast <ParseMultiply*> (from->getPointer())->run());
                    break;
                case Variable::typeString:
                    InterpreterException("Cannot multiply strings!");
                    return;
            }
            break;
        case ParseStruct::operatorDivide:
            switch (var->type()){
                case Variable::typeNum:
                    reinterpret_cast <VariableNum*>(var->getPointer())->value = std::stold(reinterpret_cast <ParseDivide*> (from->getPointer())->run());
                    break;
                case Variable::typeString:
                    InterpreterException("Cannot divide strings!");
                    return;
            }
            break;
        case ParseStruct::operatorPower:
            switch (var->type()){
                case Variable::typeNum:
                    reinterpret_cast <VariableNum*>(var->getPointer())->value = std::stold(reinterpret_cast <ParsePower*> (from->getPointer())->run());
                    break;
                case Variable::typeString:
                    InterpreterException("Cannot power-ize strings!");
                    return;
            }
            break;

        default:
            InterpreterException("Wrong assignment origin!");
           return;
    }

    #ifdef PYTHON___DEBUG
    std::cout << " making its value: ";
    if (var->type() == Variable::typeNum) {
        std::cout << reinterpret_cast<VariableNum*>(var->getPointer())->value << "\n";
    }
    else if (var->type() == Variable::typeString) {
        std::cout << reinterpret_cast<VariableString*>(var->getPointer())->value << "\n";
    }
    #endif
}

ParsePlus::ParsePlus(const std::pair <unsigned int, unsigned int> left, const std::pair <unsigned int, unsigned int> right, uint8_t type) {
    operationType = type;
    this->left = std::unique_ptr <ParseStruct> (ParseMathematicalOperation(left));
    this->right = std::unique_ptr <ParseStruct> (ParseMathematicalOperation(right));
}
[[nodiscard]] const uint8_t ParsePlus::type() const {
    return ParseStruct::operatorPlus;
}
std::string ParsePlus::run() const{

    #ifdef PYTHON___DEBUG
    std::cout << "(";
    #endif

    // add variable type check

    if (operationType == ParseStruct::variableNum){

        long double value = 0;
        if (left->type() == ParseStruct::variableVariable){
            value = reinterpret_cast <VariableNum*> (reinterpret_cast <ParseVariable*> (left->getPointer())->run())->value;
        }
        else if (left->type() == ParseStruct::variableValue){
            value = std::stold(reinterpret_cast <ParseValue*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorPlus){
            value = std::stold(reinterpret_cast <ParsePlus*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorMinus){
            value = std::stold(reinterpret_cast <ParseMinus*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorMultiply){
            value = std::stold(reinterpret_cast <ParseMultiply*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorDivide){
            value = std::stold(reinterpret_cast <ParseDivide*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorPower){
            value = std::stold(reinterpret_cast <ParsePower*> (left->getPointer())->run());
        }
        else {
            InterpreterException("Wrong operator left side integer in condition!");
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << " + ";
        #endif

        if (right->type() == ParseStruct::variableVariable){
            value += reinterpret_cast <VariableNum*> (reinterpret_cast <ParseVariable*> (right->getPointer())->run())->value;
        }
        else if (right->type() == ParseStruct::variableValue){
            value += std::stold(reinterpret_cast <ParseValue*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorPlus){
            value += std::stold(reinterpret_cast <ParsePlus*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorMinus){
            value += std::stold(reinterpret_cast <ParseMinus*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorMultiply){
            value += std::stold(reinterpret_cast <ParseMultiply*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorDivide){
            value += std::stold(reinterpret_cast <ParseDivide*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorPower){
            value += std::stold(reinterpret_cast <ParsePower*> (right->getPointer())->run());
        }
        else {
            InterpreterException("Wrong operator in right side integer condition (plus)!");
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << ")";
        #endif

        return std::to_string(value);
    }
    else if (operationType == ParseStruct::variableString) {
        std::string value = "";
        if (left->type() == ParseStruct::variableVariable){
            value = reinterpret_cast <VariableString*> (reinterpret_cast <ParseVariable*> (left->getPointer())->run())->value;
        }
        else if (left->type() == ParseStruct::variableValue){
            value = reinterpret_cast <ParseValue*> (left->getPointer())->run();
        }
        else if (left->type() == ParseStruct::operatorPlus){
            value = reinterpret_cast <ParsePlus*> (left->getPointer())->run();
        }
        else {
            InterpreterException("Wrong operator in left side string condition!");
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << " + ";
        #endif

        if (right->type() == ParseStruct::variableVariable){
            value += reinterpret_cast <VariableString*> (reinterpret_cast <ParseVariable*> (right->getPointer())->run())->value;
        }
        else if (right->type() == ParseStruct::variableValue){
            value += reinterpret_cast <ParseValue*> (right->getPointer())->run();
        }
        else if (right->type() == ParseStruct::operatorPlus){
            value += reinterpret_cast <ParsePlus*> (right->getPointer())->run();
        }
        else {
            InterpreterException("Wrong operator in right side string condition!");
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << ")";
        #endif

        return value;
    }
    else {
        InterpreterException("Wrong variable type in plus!");
    }
    return "0";
}

ParseMinus::ParseMinus(const std::pair <unsigned int, unsigned int> left, const std::pair <unsigned int, unsigned int> right, uint8_t type) {
    operationType = type;
    this->left = std::unique_ptr <ParseStruct> (ParseMathematicalOperation(left));
    this->right = std::unique_ptr <ParseStruct> (ParseMathematicalOperation(right));
}
[[nodiscard]] const uint8_t ParseMinus::type() const {
    return ParseStruct::operatorMinus;
}
std::string ParseMinus::run() const{

    #ifdef PYTHON___DEBUG
    std::cout << "(";
    #endif

    if (operationType == ParseStruct::variableNum){

        long double value = 0;
        if (left->type() == ParseStruct::variableVariable){
            value = reinterpret_cast <VariableNum*> (reinterpret_cast <ParseVariable*> (left->getPointer())->run())->value;
        }
        else if (left->type() == ParseStruct::variableValue){
            value = std::stold(reinterpret_cast <ParseValue*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorPlus){
            value = std::stold(reinterpret_cast <ParsePlus*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorMinus){
            value = std::stold(reinterpret_cast <ParseMinus*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorMultiply){
            value = std::stold(reinterpret_cast <ParseMultiply*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorDivide){
            value = std::stold(reinterpret_cast <ParseDivide*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorPower){
            value = std::stold(reinterpret_cast <ParsePower*> (left->getPointer())->run());
        }
        else {
            InterpreterException("Wrong operator left side integer in condition!");
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << " - ";
        #endif

        if (right->type() == ParseStruct::variableVariable){
            value -= reinterpret_cast <VariableNum*> (reinterpret_cast <ParseVariable*> (right->getPointer())->run())->value;
        }
        else if (right->type() == ParseStruct::variableValue){
            value -= std::stold(reinterpret_cast <ParseValue*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorPlus){
            value -= std::stold(reinterpret_cast <ParsePlus*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorMinus){
            value -= std::stold(reinterpret_cast <ParseMinus*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorMultiply){
            value -= std::stold(reinterpret_cast <ParseMultiply*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorDivide){
            value -= std::stold(reinterpret_cast <ParseDivide*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorPower){
            value -= std::stold(reinterpret_cast <ParsePower*> (right->getPointer())->run());
        }
        else {
            InterpreterException("Wrong operator in right side integer condition (minus)!");
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << ")";
        #endif
        return std::to_string(value);
    }
    else {
        InterpreterException("Wrong variable type in minus!");
    }
    return "0";
}

ParseMultiply::ParseMultiply(const std::pair <unsigned int, unsigned int> left, const std::pair <unsigned int, unsigned int> right, uint8_t type) {
    operationType = type;
    this->left = std::unique_ptr <ParseStruct> (ParseMathematicalOperation(left));
    this->right = std::unique_ptr <ParseStruct> (ParseMathematicalOperation(right));
}
[[nodiscard]] const uint8_t ParseMultiply::type() const {
    return ParseStruct::operatorMultiply;
}
std::string ParseMultiply::run() const{

    #ifdef PYTHON___DEBUG
    std::cout << "(";
    #endif

    if (operationType == ParseStruct::variableNum){

        long double value = 0;
        if (left->type() == ParseStruct::variableVariable){
            value = reinterpret_cast <VariableNum*> (reinterpret_cast <ParseVariable*> (left->getPointer())->run())->value;
        }
        else if (left->type() == ParseStruct::variableValue){
            value = std::stold(reinterpret_cast <ParseValue*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorPlus){
            value = std::stold(reinterpret_cast <ParsePlus*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorMinus){
            value = std::stold(reinterpret_cast <ParseMinus*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorMultiply){
            value = std::stold(reinterpret_cast <ParseMultiply*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorDivide){
            value = std::stold(reinterpret_cast <ParseDivide*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorPower){
            value = std::stold(reinterpret_cast <ParsePower*> (left->getPointer())->run());
        }
        else {
            InterpreterException("Wrong operator left side integer in condition!");
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << " * ";
        #endif

        if (right->type() == ParseStruct::variableVariable){
            value *= reinterpret_cast <VariableNum*> (reinterpret_cast <ParseVariable*> (right->getPointer())->run())->value;
        }
        else if (right->type() == ParseStruct::variableValue){
            value *= std::stold(reinterpret_cast <ParseValue*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorPlus){
            value *= std::stold(reinterpret_cast <ParsePlus*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorMinus){
            value *= std::stold(reinterpret_cast <ParseMinus*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorMultiply){
            value *= std::stold(reinterpret_cast <ParseMultiply*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorDivide){
            value *= std::stold(reinterpret_cast <ParseDivide*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorPower){
            value *= std::stold(reinterpret_cast <ParsePower*> (right->getPointer())->run());
        }
        else {
            InterpreterException("Wrong operator in right side integer condition (multiply)!" + std::to_string(right->type()));
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << ")";
        #endif
        return std::to_string(value);
    }
    else {
        InterpreterException("Wrong variable type in multiply!");
    }
    return "0";
}

ParseDivide::ParseDivide(const std::pair <unsigned int, unsigned int> left, const std::pair <unsigned int, unsigned int> right, uint8_t type) {
    operationType = type;
    this->left = std::unique_ptr <ParseStruct> (ParseMathematicalOperation(left));
    this->right = std::unique_ptr <ParseStruct> (ParseMathematicalOperation(right));
}
[[nodiscard]] const uint8_t ParseDivide::type() const {
    return ParseStruct::operatorDivide;
}
std::string ParseDivide::run() const{

    #ifdef PYTHON___DEBUG
    std::cout << "(";
    #endif

    if (operationType == ParseStruct::variableNum){

        long double value = 0;
        if (left->type() == ParseStruct::variableVariable){
            value = reinterpret_cast <VariableNum*> (reinterpret_cast <ParseVariable*> (left->getPointer())->run())->value;
        }
        else if (left->type() == ParseStruct::variableValue){
            value = std::stold(reinterpret_cast <ParseValue*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorPlus){
            value = std::stold(reinterpret_cast <ParsePlus*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorMinus){
            value = std::stold(reinterpret_cast <ParseMinus*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorMultiply){
            value = std::stold(reinterpret_cast <ParseMultiply*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorDivide){
            value = std::stold(reinterpret_cast <ParseDivide*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorPower){
            value = std::stold(reinterpret_cast <ParsePower*> (left->getPointer())->run());
        }
        else {
            InterpreterException("Wrong operator left side integer in condition!");
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << " / ";
        #endif

        if (right->type() == ParseStruct::variableVariable){
            value /= reinterpret_cast <VariableNum*> (reinterpret_cast <ParseVariable*> (right->getPointer())->run())->value;
        }
        else if (right->type() == ParseStruct::variableValue){
            value /= std::stold(reinterpret_cast <ParseValue*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorPlus){
            value /= std::stold(reinterpret_cast <ParsePlus*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorMinus){
            value /= std::stold(reinterpret_cast <ParseMinus*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorMultiply){
            value /= std::stold(reinterpret_cast <ParseMultiply*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorDivide){
            value /= std::stold(reinterpret_cast <ParseDivide*> (right->getPointer())->run());
        }
        else if (right->type() == ParseStruct::operatorPower){
             value /= std::stold(reinterpret_cast <ParsePower*> (right->getPointer())->run());
        }
        else {
            InterpreterException("Wrong operator in right side integer condition (divide)!");
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << ")";
        #endif
        return std::to_string(value);
    }
    else {
        InterpreterException("Wrong variable type in divide!");
    }
    return "0";
}

ParsePower::ParsePower(const std::pair <unsigned int, unsigned int> left, const std::pair <unsigned int, unsigned int> right, uint8_t type) {
    operationType = type;
    this->left = std::unique_ptr <ParseStruct> (ParseMathematicalOperation(left));
    this->right = std::unique_ptr <ParseStruct> (ParseMathematicalOperation(right));
}
[[nodiscard]] const uint8_t ParsePower::type() const {
    return ParseStruct::operatorPower;
}
std::string ParsePower::run() const{

    #ifdef PYTHON___DEBUG
    std::cout << "(";
    #endif

    if (operationType == ParseStruct::variableNum){

        long double value = 0;
        if (left->type() == ParseStruct::variableVariable){
            value = reinterpret_cast <VariableNum*> (reinterpret_cast <ParseVariable*> (left->getPointer())->run())->value;
        }
        else if (left->type() == ParseStruct::variableValue){
            value = std::stold(reinterpret_cast <ParseValue*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorPlus){
            value = std::stold(reinterpret_cast <ParsePlus*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorMinus){
            value = std::stold(reinterpret_cast <ParseMinus*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorMultiply){
            value = std::stold(reinterpret_cast <ParseMultiply*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorDivide){
            value = std::stold(reinterpret_cast <ParseDivide*> (left->getPointer())->run());
        }
        else if (left->type() == ParseStruct::operatorPower){
            value = std::stold(reinterpret_cast <ParsePower*> (left->getPointer())->run());
        }
        else {
            InterpreterException("Wrong operator left side integer in condition!");
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << " ^ ";
        #endif

        if (right->type() == ParseStruct::variableVariable){
            value = (long double)(powl(value, reinterpret_cast <VariableNum*> (reinterpret_cast <ParseVariable*> (right->getPointer())->run())->value));
        }
        else if (right->type() == ParseStruct::variableValue){
            value = (long double)(powl(value, std::stold(reinterpret_cast <ParseValue*> (right->getPointer())->run())));
        }
        else if (right->type() == ParseStruct::operatorPlus){
            value = (long double)(powl(value, std::stold(reinterpret_cast <ParsePlus*> (right->getPointer())->run())));
        }
        else if (right->type() == ParseStruct::operatorMinus){
            value = (long double)(powl(value, std::stold(reinterpret_cast <ParseMinus*> (right->getPointer())->run())));
        }
        else if (right->type() == ParseStruct::operatorMultiply){
            value = (long double)(powl(value, std::stold(reinterpret_cast <ParseMultiply*> (right->getPointer())->run())));
        }
        else if (right->type() == ParseStruct::operatorDivide){
            value = (long double)(powl(value, std::stold(reinterpret_cast <ParseDivide*> (right->getPointer())->run())));
        }
        else if (right->type() == ParseStruct::operatorPower){
            value = (long double)(powl(value, std::stold(reinterpret_cast <ParsePower*> (right->getPointer())->run())));
        }
        else {
            InterpreterException("Wrong operator in right side integer condition (power)!");
            return "0";
        }

        #ifdef PYTHON___DEBUG
        std::cout << ")";
        #endif
        return std::to_string(value);
    }
    else {
        InterpreterException("Wrong variable type in power!");
    }
    return "0";
}