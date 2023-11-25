#include "interpreter.hpp"
#include "parsing.hpp"
#include "defines.hpp"
#include "variables.hpp"
#include "exceptions.hpp"

#include <iostream>
#include <unordered_map>

std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

bool exceptionHappened = false;

void RunLine(std::string line) {
    std::unique_ptr<ParseStruct> parsed = SplitInterpreterLine(line);
    if (exceptionHappened){
        exceptionHappened = false;
        std::cout << "Parsing of current line has been cancelled!\n";
        return;
    }

    // actual running of the code, finally, only first level things here, the rest recursively or something
    switch (parsed->type()){
        case ParseStruct::operatorAssign:
            reinterpret_cast<ParseAssign*>(parsed->getPointer())->run();
            break;

        default:
            InterpreterException("Wrong level 1 token!");
            break;

    }

    if (exceptionHappened){
        exceptionHappened = false;
        std::cout << "Interpreting of current line has been cancelled!\n";
        return;
    }
}

void RunInterpreter(){
    RunLine("int var = 10");
    RunLine("int var2 = 20");
    std::cout << reinterpret_cast <VariableInt*>(globalVariables["var"]->getPointer())->value << "\n";
    RunLine("var = var2 + 5 + 6 + var2");
    std::cout << reinterpret_cast <VariableInt*>(globalVariables["var"]->getPointer())->value << "\n";
}