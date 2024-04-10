#include "runtime.hpp"
#include "exceptions.hpp"

std::string RunValueReturning(ParseStruct* runnable, uint8_t type){
    switch (runnable->type()){
        case ParseStruct::operatorPlus:
            return reinterpret_cast<ParsePlus*> (runnable)->run();
        case ParseStruct::operatorMinus:
            return reinterpret_cast<ParseMinus*> (runnable)->run();
        case ParseStruct::operatorMultiply:
            return reinterpret_cast<ParseMultiply*> (runnable)->run();
        case ParseStruct::operatorDivide:
            return reinterpret_cast<ParseDivide*> (runnable)->run();
        case ParseStruct::operatorPower:
            return reinterpret_cast<ParsePower*> (runnable)->run();
        case ParseStruct::operatorFunction:
            return reinterpret_cast<ParseFunction*> (runnable)->run();
        case ParseStruct::keywordImplicit:
            return reinterpret_cast<ParseImplicit*> (runnable)->run(type);
        case ParseStruct::variableNum:
            return std::to_string(reinterpret_cast<VariableNum*>(reinterpret_cast<ParseNum*> (runnable)->run())->value);
        case ParseStruct::variableString:
            return reinterpret_cast<VariableString*>(reinterpret_cast<ParseNum*> (runnable)->run())->value;
        case ParseStruct::variableValue:
            return reinterpret_cast<ParseValue*> (runnable)->run();
        default:
            break;
    }

    if (runnable->type() == ParseStruct::variableVariable) {
        auto* var = reinterpret_cast<ParseVariable*> (runnable)->run();
        if (var == nullptr){
            InterpreterException("Variable corrupted!");
            return "0";
        }
        if (var->type() == Variable::typeNum) {
            return std::to_string(reinterpret_cast<VariableNum*>(reinterpret_cast<ParseVariable*> (runnable)->run())->value);
        }
        else if (var->type() == Variable::typeString) {
            return reinterpret_cast<VariableString*>(reinterpret_cast<ParseVariable*> (runnable)->run())->value;
        }
        else {
            InterpreterException("Wrong variable type!");
            return "0";
        }
    }

    InterpreterException("Unknown value returning candidate!");
    return "0";
}

std::vector <std::string> RunArrayValueReturning(ParseStruct* runnable) {
    switch (runnable->type()){
        case ParseStruct::variableValue:
            return reinterpret_cast<ParseValue*> (runnable)->values;
        default:
            break;
    }

    if (runnable->type() == ParseStruct::variableVariable) {
        ParserException("Taking values from other array not yet implemented!");
    }

    InterpreterException("Unknown value returning candidate!");
    return { "0" };
}