#include <unordered_map>

#include "checks.hpp"
#include "content/variables.hpp"
#include "defines.hpp"
#include "content/pmmstdlib/pmmstdlib.hpp"
#include "functionStack.hpp"

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

bool IsVariable(const std::string& token){
    if (!functionStack.empty()){
        if (!functionStack.top().levels.empty()){
            for (long long n = functionStack.top().levels.size() - 1; n > -1; n--){
                if (auto found = functionStack.top().levels[n].variables.find(token); found != functionStack.top().levels[n].variables.end()) {
                    return true;
                }
            }
        }
        if (auto found = functionStack.top().variables.find(token); found != functionStack.top().variables.end()) {
            return true;
        }
    }
    std::cout << globalLevels.size() << "\n";
    if (!globalLevels.empty()) {
        for (long long n = globalLevels.size() - 1; n > -1; n--) {
            std::cout << n << "\n";
            if (auto found = globalLevels[n].variables.find(token); found != globalLevels[n].variables.end()) {
                return true;
            }
        }
    }
    if (auto found = globalVariables.find(token); found != globalVariables.end()) {
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
    if (IsVariable(token) or IsConvertibleToInt(token) or IsConvertibleToString(token)){
        std::cout << "Yes: " << token << "\n";
        return true;
    }
    if (IsFunction(token)){
        if (functions[token]->returnType() != Variable::none){
            std::cout << "Yes: " << token << "\n";
            return true;
        }
    }
    std::cout << "No: " << token << "\n";
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
                ParserException("Mathematical condition invalid due to brackets!");
                return false;
            }
            bracketLevel--;
        }
    }
    if (bracketLevel != 0) {
        ParserException("Mathematical condition invalid due to brackets!");
        return false;
    }
    return true;
}

std::string CleanString(std::string input){
    // tab and space deletion from ends
    while (input.length() > 0 and (input.starts_with(' ') or input.starts_with('\t'))) {
        input.erase(0, 1);
    }
    while (input.length() > 0 and input.ends_with(' ') or input.ends_with('\t')){
        input.erase(input.length() - 1, 1);
    }
    return input;
}