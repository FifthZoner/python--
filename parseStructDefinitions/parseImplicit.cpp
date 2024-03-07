#include <vector>
#include <string>

#include "parseStructs.hpp"
#include "../interpretation/checks.hpp"

#ifdef PYTHON___DEBUG
#include <iostream>
#endif


ParseImplicit::ParseImplicit(std::pair<unsigned int, unsigned int> range){

    // brackets check
    if (!AreBracketsValid(range)) {
        ParserException("Implicit condition invalid due to brackets!");
        return;
    }
    unsigned int bracketLevel;
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
        else if (DoesReturnValue(parsedLine[n])){
            if (IsVariable(parsedLine[n])){
                tokens.push_back(std::unique_ptr<ParseStruct>(new ParseVariable(parsedLine[n])));
            }
            else if (IsFunction(parsedLine[n])){
                // looking for the brackets
                int bracketLevel2 = 0;
                for (unsigned int m = n + 1; m < range.second; m++){
                    if (parsedLine[m] == "("){
                        bracketLevel2++;
                    }
                    else if (parsedLine[m] == ")"){
                        bracketLevel2--;
                        if (bracketLevel2 < 0){
                            ParserException("Bracket error in function parsing in implicit!");
                            return;
                        }
                        if (bracketLevel2 == 0){
                            tokens.push_back(std::unique_ptr<ParseStruct>(new ParseFunction(std::pair <unsigned int, unsigned int> (n, m + 1))));
                            n = m + 1;
                            break;
                        }
                    }
                }
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

#include <iostream>

std::string ParseImplicit::run(uint8_t type) const {
    if (type == ParseStruct::variableNum){
        long double value = 0;
        for (auto& n : tokens){
            switch (n->type()){
                case ParseStruct::variableVariable:
                    value += reinterpret_cast<VariableNum*>(reinterpret_cast<ParseVariable*>(n.get())->run())->value;
                    break;
                case ParseStruct::variableValue:
                    value += std::stold(reinterpret_cast<ParseValue*>(n.get())->run());
                    break;
                case ParseStruct::operatorFunction:
                    value += std::stold(reinterpret_cast<ParseFunction*>(n.get())->run());
                    break;
                case ParseStruct::operatorPlus:
                    value += std::stold(reinterpret_cast<ParsePlus*>(n.get())->run());
                    break;
                case ParseStruct::operatorMinus:
                    value += std::stold(reinterpret_cast<ParseMinus*>(n.get())->run());
                    break;
                case ParseStruct::operatorMultiply:
                    value += std::stold(reinterpret_cast<ParseMultiply*>(n.get())->run());
                    break;
                case ParseStruct::operatorDivide:
                    value += std::stold(reinterpret_cast<ParseDivide*>(n.get())->run());
                    break;
                case ParseStruct::operatorPower:
                    value += std::stold(reinterpret_cast<ParsePower*>(n.get())->run());
                    break;
                default:
                    InterpreterException("Wrong implicit called type!");
                    return "0";
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
                    value += reinterpret_cast<ParseFunction*>(n.get())->run();
                    break;
                case ParseStruct::operatorPlus:
                    value += reinterpret_cast<ParsePlus*>(n.get())->run();
                    break;
                default:
                    InterpreterException("Wrong implicit called type!");
                    return "0";
            }
        }
        return value;
    }
    else {
        InterpreterException("Wrong implicit argument type!");
        return "0";
    }
}