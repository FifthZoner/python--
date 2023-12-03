#include <string>
#include <vector>

#include "parseStructs.hpp"
#include "../checks.hpp"
#include "../interpretation/runtime.hpp"

ParseFunction::ParseFunction(std::pair<unsigned int, unsigned int> range){
    if (range.second - range.first < 3){
        ParserException("Wrong amount of tokens passed to form a function!");
        return;
    }

    if (!IsFunction(parsedLine[range.first])){
        ParserException("Wrong beginning of a function!");
        return;
    }

    function = functions[parsedLine[range.first]].get();

    // bracket check, I think it's not always called by ParseMathematicalSomethingSomething()
    if (!AreBracketsValid(range)){
        return;
    }

    unsigned int start = range.first;
    unsigned int bracketLevel = 0;
    for (unsigned int n = range.first + 1; n < range.second; n++){
        if (parsedLine[n] == "," or (parsedLine[n] == ")" and bracketLevel == 1)){
            // push to vector
            if (n == start){
                if (!function->variables.empty()){
                    ParserException("Could not parse function arguments!");
                    return;
                }
                break;
            }
            if (function->variables.size() > tokens.size()){
                if (function->variables[tokens.size()].isImplicit){
                    tokens.push_back(std::unique_ptr<ParseStruct>(new ParseImplicit(std::pair <unsigned int, unsigned int> (start, n))));
                }
                else {
                    tokens.push_back(std::unique_ptr<ParseStruct>(ParseMathematicalOperation(std::pair <unsigned int, unsigned int> (start, n))));
                }
            }
            else {
                ParserException("Too many arguments in function!");
                return;
            }
            start = n + 1;
        }
        else if (parsedLine[n] == "("){
            bracketLevel++;
            if (bracketLevel == 1){
                start = n + 1;
            }
        }
        else if (parsedLine[n] == ")"){
            bracketLevel--;
        }
        // math thing does all the heavy lifting in terms of checking the inside
    }
}
std::string ParseFunction::run() const {

    // getting all the variables and checking with the function container
    // size
    if (tokens.size() != function->variables.size()){
        InterpreterException("Wrong argument list for function!");
        return "0";
    }
    // implicit, should never ever happen, but it's better to be safe
    for (unsigned int n = 0; n < tokens.size(); n++){
        if ((tokens[n]->type() == ParseStruct::keywordImplicit and !function->variables[n].isImplicit) or
            (tokens[n]->type() != ParseStruct::keywordImplicit and  function->variables[n].isImplicit)){
            InterpreterException("Implicit mismatch, somehow");
            return "0";
        }
    }
    // getting types of all variables into a vector
    std::vector <std::string> stringValues;
    for (auto& n : tokens){
        if (stringValues.size() == function->variables.size()){
            InterpreterException("Argument amount mismatch in function!");
            return "0";
        }
        if (function->variables[stringValues.size()].isImplicit){
            stringValues.push_back(RunValueReturning(n.get(), function->variables[stringValues.size()].type));
            continue;
        }
        stringValues.push_back(RunValueReturning(n.get(), Variable::none));
    }
    // value type check
    for (unsigned int n = 0; n < tokens.size(); n++){
        if (function->variables[n].type == Variable::typeInt){
            if (!IsConvertibleToInt(stringValues[n])){
                InterpreterException("Argument mismatch in function!");
                return "0";
            }
        }
    }

    return function->run(stringValues);
}
const uint8_t ParseFunction::type() const {
    return ParseStruct::operatorFunction;
}