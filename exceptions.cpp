#include <iostream>

#include "exceptions.hpp"
#include "defines.hpp"

extern bool exceptionHappened;

void ParserException(const std::string& content){
    exceptionHappened = true;
    #ifdef PYTHON___DEBUG
    std::cout << "\n";
    #endif
    std::cout << "PARSING EXCEPTION: " << content << "\n";
}

void InterpreterException(const std::string& content){
    exceptionHappened = true;
    #ifdef PYTHON___DEBUG
    std::cout << "\n";
    #endif
    std::cout << "INTERPRETER EXCEPTION: " << content << "\n";
}