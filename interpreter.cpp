#include "interpreter.hpp"
#include "parsing.hpp"


#include <iostream>
void RunLine(std::string line) {
    ParseStruct *parsed = SplitInterpreterLine(line);
}

void RunInterpreter(){
    RunLine("var = 10");
}