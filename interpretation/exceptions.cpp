#include <iostream>

#include "exceptions.hpp"
#include "../content/defines.hpp"


void ParserException(const std::string& content){
    #ifdef PYTHON___DEBUG
    std::cout << "\n";
    #endif
    std::cout << "PARSING EXCEPTION: " << content << "\n";
    throw PMMException();
}

void InterpreterException(const std::string& content){
    #ifdef PYTHON___DEBUG
    std::cout << "\n";
    #endif
    std::cout << "INTERPRETER EXCEPTION: " << content << "\n";
    throw PMMException();
}

char *PMMException::what() {
    return nullptr;
}
