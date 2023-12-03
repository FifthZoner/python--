#include <unordered_map>

#include "checks.hpp"
#include "content/variables.hpp"
#include "defines.hpp"
#include "content/pmmstdlib/pmmstdlib.hpp"

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
    if (auto found = functions.find(token); found != functions.end()) {
        return true;
    }
    return false;
}

bool DoesReturnValue(const std::string& token){
    if (IsConvertibleToInt(token) or IsConvertibleToString(token) or IsVariable(token)){
        return true;
    }
    if (IsFunction(token)){
        if (functions[token]->returnType() != Variable::none){
            return true;
        }
    }
    return false;
}

bool AreBracketsValid(std::pair <unsigned int, unsigned int> range){
    unsigned int bracketLevel = 0;
    for (unsigned int n = range.first; n < range.second; n++) {
        if (parsedLine[n] == "("){
            bracketLevel++;
        }
        else if (parsedLine[n] == ")") {
            if (bracketLevel == 0){
                ParserException("Mathematical operation invalid due to brackets!");
                return false;
            }
            bracketLevel--;
        }
    }
    if (bracketLevel != 0) {
        ParserException("Mathematical operation invalid due to brackets!");
        return false;
    }
    return true;
}