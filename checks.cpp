#include <unordered_map>

#include "checks.hpp"
#include "variables.hpp"
#include "defines.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

bool IsConvertibleToInt(const std::string& token) {
    #ifdef PYTHON___DEBUG
    //std::cout << "Checking if: \"" << token << "\" is convertible to int:\n";
    #endif
    for (auto current : token) {
        if ((current > '9' or current < '0') and current != '-'){
            #ifdef PYTHON___DEBUG
            //std::cout << "It is not!\n";
            #endif
            return false;
        }
    }
    #ifdef PYTHON___DEBUG
    //std::cout << "It is!\n";
    #endif
    return true;
}

bool IsGlobalVariable(const std::string& token) {
    #ifdef PYTHON___DEBUG
    //std::cout << "Checking if: \"" << token << "\" is a global variable:\n";
    #endif
    if (auto found = globalVariables.find(token); found != globalVariables.end()) {
        #ifdef PYTHON___DEBUG
        //std::cout << "It is!\n";
        #endif
        return true;
    }
    #ifdef PYTHON___DEBUG
    //std::cout << "It is not!\n";
    #endif
    return false;
}

bool IsLocalVariable(const std::string& token) {
    #ifdef PYTHON___DEBUG
    //std::cout << "Checking if: \"" << token << "\" is a local variable:\n";
    #endif

    #ifdef PYTHON___DEBUG
    //std::cout << "Not implemented!\n";
    #endif
    return false;
}

bool IsFunction(const std::string& token) {
    #ifdef PYTHON___DEBUG
    std::cout << "Checking if: \"" << token << "\" is a function:\n";
    #endif

    #ifdef PYTHON___DEBUG
    std::cout << "It is not!\n";
    #endif
    return false;
}