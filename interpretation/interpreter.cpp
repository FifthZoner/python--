#include "interpreter.hpp"
#include "../parsing.hpp"
#include "../defines.hpp"
#include "../variables.hpp"
#include "../exceptions.hpp"

#include <iostream>
#include <unordered_map>
#include <utility>

std::unordered_map <std::string, std::unique_ptr <Variable>> globalVariables;

bool exceptionHappened = false;

void RunLine(std::string line) {
    std::unique_ptr<ParseStruct> parsed = SplitInterpreterLine(std::move(line));
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

void RunInterpreter(InterpreterInterface* interface){
    std::unique_ptr <InterpreterInterface> stream = std::unique_ptr <InterpreterInterface> (interface);

    // TODO: add some ending condition
    while (true) {
        RunLine(stream->getNextLine());
    }

}