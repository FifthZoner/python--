#include "function.hpp"
#include "../exceptions.hpp"
#include "../interpretation/interpreter.hpp"
#include "../parseStructDefinitions/parseStructs.hpp"
#include "../checks.hpp"
#include "../functionStack.hpp"


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

extern int innerLevel;
std::vector <FunctionVariable> newFunctionVariables;
uint8_t newFunctionReturnType = 0;
std::pair <unsigned long long, unsigned long long> newFunctionRange;
std::string newFunctionName;

void ParseNewFunction(std::pair<unsigned int, unsigned int> range){
    if (range.second - range.first < 4){
        // should never happen
        ParserException("Wrong amount of tokens passed to form a function!");
        return;
    }

    if (!globalLevels.empty() or !functionStack.empty() or innerLevel != 0){
        ParserException("Cannot define function inside something else!");
        return;
    }

    if (IsFunction(parsedLine[range.first + 1])){
        isDefiningFunction = false;
        ParserException("Function redefinition!");
        return;
    }

    newFunctionName = parsedLine[range.first + 1];

    // bracket check, I think it's not always called by ParseMathematicalSomethingSomething()
    if (!AreBracketsValid(range)){
        return;
    }
    // return type
    if (parsedLine[range.first] == "void"){
        newFunctionReturnType = Variable::none;
    }
    else if (parsedLine[range.first] == "int"){
        newFunctionReturnType = Variable::typeInt;
    }
    else if (parsedLine[range.first] == "string"){
        newFunctionReturnType = Variable::typeString;
    }
    else {
        // should never happen
        ParserException("Cannot get function return type!");
    }
    newFunctionVariables.clear();

    std::pair <unsigned int, unsigned int> variableRange (range.first + 3, range.first + 3);
    for (unsigned int n = range.first + 3; n < range.second; n++) {
        if (parsedLine[n] == ")" or parsedLine[n] == ",") {
            // end of argument
            variableRange.second = n;

            if (variableRange.second - variableRange. first > 3) {
                ParserException("Wrong function argument syntax!");
                return;
            }
            if (variableRange.second - variableRange. first == 2){
                // <type> <token>
                if (parsedLine[variableRange.first] == "int"){
                    newFunctionVariables.emplace_back(Variable::typeInt, parsedLine[variableRange.first + 1], false);
                }
                else if (parsedLine[variableRange.first] == "string"){
                    newFunctionVariables.emplace_back(Variable::typeString, parsedLine[variableRange.first + 1], false);
                }
                else {
                    ParserException("Wrong function argument type!");
                    return;
                }
            }
            else if (variableRange.second - variableRange. first == 2){
                // implicit <type> <token>
                if (parsedLine[variableRange.first] != "implicit"){
                    ParserException("Wrong function argument type!");
                    return;
                }
                if (parsedLine[variableRange.first + 1] == "int"){
                    newFunctionVariables.emplace_back(Variable::typeInt, parsedLine[variableRange.first + 2], true);
                }
                else if (parsedLine[variableRange.first + 1] == "string"){
                    newFunctionVariables.emplace_back(Variable::typeString, parsedLine[variableRange.first + 2], true);
                }
                else {
                    ParserException("Wrong function argument type!");
                    return;
                }
            }

            variableRange.first = n + 1;
        }
    }
    isDefiningFunction = true;
    newFunctionRange.first = interpreterStream->lines.size();
}

void PushNewFunction(unsigned long long endIndex){
    newFunctionRange.second = endIndex;
    functions[newFunctionName] = std::unique_ptr <Function> (new FunctionCustom(newFunctionRange, newFunctionReturnType, newFunctionVariables));
}