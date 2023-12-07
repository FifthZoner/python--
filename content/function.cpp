#include "function.hpp"
#include "../exceptions.hpp"
#include "../interpretation/interpreter.hpp"
#include "../parseStructDefinitions/parseStructs.hpp"
#include "../checks.hpp"


FunctionVariable::FunctionVariable(const uint8_t type, const std::string &name, bool isImplicit) {
    this->type = type;
    this->name = name;
    this->isImplicit = isImplicit;
}

uint8_t Function::type() const {
    return Function::none;
}

std::string Function::run(std::vector <std::string>& arguments) {
    return "0";
}

uint8_t Function::returnType() const {
    return Variable::none;
}

FunctionBinding::FunctionBinding(void (*functionWrapper)(std::vector <std::string>&), uint8_t returnType, std::vector <FunctionVariable> variables){
    this->functionWrapper = functionWrapper;
    this->returnTypeValue = returnType;
    this->variables = std::move(variables);
}
uint8_t FunctionBinding::type() const{
    return Function::binding;
}
uint8_t FunctionBinding::returnType() const{
    return returnTypeValue;
}
std::string FunctionBinding::run(std::vector <std::string>& arguments){

    // correctness of arguments is to be checked at the ParseFunction level to allow for easy implicit handling
    functionWrapper(arguments);
    if (returnTypeValue != Variable::none){
        if (returnTypeValue == Variable::typeInt or returnTypeValue == Variable::typeString){
            return returnValueString;
        }
        InterpreterException("Wrong return type of function!");
    }
    return "0";
}

FunctionCustom::FunctionCustom(std::pair <uint64_t, uint64_t> range, uint8_t returnType, std::vector <FunctionVariable> variables) {
    this->range = range;
    this->returnTypeValue = returnType;
    this->variables = std::move(variables);
}

[[nodiscard]] uint8_t FunctionCustom::type() const {
    return Function::custom;
}

[[nodiscard]] uint8_t FunctionCustom::returnType() const {
    return returnTypeValue;
}

std::string FunctionCustom::run(std::vector <std::string>& arguments) {
    // correctness of arguments is to be checked at the ParseFunction level to allow for easy implicit handling
    for (uint64_t n = range.first; n < range.second; n++){
        if (RunLine(interpreterStream->lines[n], n) == RunLineOutput::returned){
            break;
        }
    }
    if (returnTypeValue != Variable::none){
        if (returnTypeValue == Variable::typeInt or returnTypeValue == Variable::typeString){
            return returnValueString;
        }
        InterpreterException("Wrong return type of function!");
    }
    return "0";
}

void ParseNewFunction(std::pair<unsigned int, unsigned int> range){
    isDefiningFunction = true;
    if (range.second - range.first < 4){
        // should never happen
        isDefiningFunction = false;
        ParserException("Wrong amount of tokens passed to form a function!");
        return;
    }

    if (IsFunction(parsedLine[range.first + 1])){
        isDefiningFunction = false;
        ParserException("Wrong beginning of a function!");
        return;
    }

    // bracket check, I think it's not always called by ParseMathematicalSomethingSomething()
    if (!AreBracketsValid(range)){
        isDefiningFunction = false;
        return;
    }
    uint8_t returnType = 0;
    // return type
    if (parsedLine[range.first] == "void"){
        returnType = Variable::none;
    }
    else if (parsedLine[range.first] == "int"){
        returnType = Variable::typeInt;
    }
    else if (parsedLine[range.first] == "string"){
        returnType = Variable::typeString;
    }
    else {
        isDefiningFunction = false;
        // should never happen
        ParserException("CAnnot get function return type!");
    }

    std::vector <FunctionVariable> variables;

}