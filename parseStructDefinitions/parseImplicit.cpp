#include <vector>
#include <string>

#include "parseStructs.hpp"
#include "../checks.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif

extern std::vector <std::string> parsedLine;

// parsestruct type
ParseImplicit::ParseImplicit(std::pair<unsigned int, unsigned int> range){

    // brackets check
    unsigned int bracketLevel = 0;
    for (unsigned int n = range.first; n < range.second; n++) {
        if (parsedLine[n] == "("){
            bracketLevel++;
        }
        else if (parsedLine[n] == ")") {
            if (bracketLevel == 0){
                ParserException("Implicit operation invalid due to brackets!");
                return;
            }
            bracketLevel--;
        }
    }
    if (bracketLevel != 0) {
        ParserException("Implicit operation invalid due to brackets!");
        return;
    }

    unsigned int start = 0;
    for (unsigned int n = range.first; n < range.second; n++){
        if (parsedLine[n] == "("){
            start = n;
            bracketLevel++;
            while (bracketLevel > 0){
                n++;
                if (parsedLine[n] == "("){
                    bracketLevel++;
                }
                else if (parsedLine[n] == ")") {
                    bracketLevel--;
                }
            }
            tokens.push_back(std::unique_ptr <ParseStruct> (ParseMathematicalOperation(std::pair <unsigned int, unsigned int>(start + 1, n))));
        }
        else if (IsConvertibleToInt(parsedLine[n]) or IsConvertibleToString(parsedLine[n]) or IsVariable(parsedLine[n]) or IsFunction(parsedLine[n])){
            if (IsVariable(parsedLine[n])){
                tokens.push_back(std::unique_ptr<ParseStruct>(new ParseVariable(parsedLine[n])));
            }
            else if (IsFunction(parsedLine[n])){
                // TODO: implement functions
            }
            else {
                tokens.push_back(std::unique_ptr<ParseStruct>(new ParseValue(parsedLine[n])));
            }
        }
        else if (parsedLine[n] != "+"){
            ParserException("Invalid operator \"" + parsedLine[n] + "\" in implicit variable!");
            return;
        }
    }
}
const uint8_t ParseImplicit::type() const{
    return ParseStruct::keywordImplicit;
}

std::string ParseImplicit::run(uint8_t type) const {
    if (type == ParseStruct::variableInt){
        long long value = 0;
        for (auto& n : tokens){
            switch (n->type()){
                case ParseStruct::variableVariable:
                    value += std::stoll(reinterpret_cast<ParseVariable*>(n.get())->run()->convert(Variable::typeInt));
                    break;
                case ParseStruct::variableValue:
                    value += std::stoll(reinterpret_cast<ParseValue*>(n.get())->run());
                    break;
                case ParseStruct::operatorFunction:
                    // TODO: implement
                    InterpreterException("Functions not implemented!");
                    return "";
                default:
                    InterpreterException("Wrong implicit called type!");
                    return "";
            }
        }
        return std::to_string(value);
    }
    else if (type == ParseStruct::variableString){
        std::string value;
        for (auto& n : tokens){
            switch (n->type()){
                case ParseStruct::variableVariable:
                    value += reinterpret_cast<ParseVariable*>(n.get())->run()->convert(Variable::typeString);
                    break;
                case ParseStruct::variableValue:
                    value += reinterpret_cast<ParseValue*>(n.get())->run();
                    break;
                case ParseStruct::operatorFunction:
                    // TODO: implement
                    InterpreterException("Functions not implemented!");
                    return "";
                default:
                    InterpreterException("Wrong implicit called type!");
                    return "";
            }
        }
        return value;
    }
    else {
        InterpreterException("Wrong implicit argument type!");
        return "";
    }
}