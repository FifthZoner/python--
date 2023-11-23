#include "interpreter.hpp"
#include "parsing.hpp"


#include <iostream>

void RunLine(std::string line) {
    std::unique_ptr<ParseStruct> parsed = SplitInterpreterLine(line);
    std::cout << int(parsed->getPointer()->type()) << "\n";
    std::cout << int(reinterpret_cast<ParseAssign*>(parsed->getPointer())->target->type()) << "\n";

}

void RunInterpreter(){
    RunLine("string var = 10");
}