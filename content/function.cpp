#include "function.hpp"
#include "../exceptions.hpp"


FunctionVariable::FunctionVariable(const uint8_t type, const std::string &name, bool isImplicit) {
    this->type = type;
    this->name = name;
    this->isImplicit = isImplicit;
}

uint8_t Function::type() const {
    return Function::none;
}

std::string Function::run(std::vector <std::string>& arguments) {
    return "";
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
    return "";
}