#include <unordered_map>

#include "checks.hpp"
#include "content/variables.hpp"
#include "defines.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

bool IsConvertibleToInt(const std::string& token) {
    for (auto current : token) {
        if ((current > '9' or current < '0') and current != '-'){
            return false;
        }
    }
    return true;
}

bool IsConvertibleToString(const std::string &token) {
    if (token.length() > 1 and token.starts_with('\"') and token.ends_with('\"')){
        return true;
    }
    return false;
}


bool IsGlobalVariable(const std::string& token) {
    if (auto found = globalVariables.find(token); found != globalVariables.end()) {
        return true;
    }
    return false;
}

bool IsLocalVariable(const std::string& token) {

    return false;
}

bool IsVariable(const std::string& token){
    if (IsGlobalVariable(token) or IsLocalVariable((token))){
        return true;
    }
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