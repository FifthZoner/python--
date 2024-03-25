// This program is a python-ish language interpreter
#include "parsing.hpp"
#include "interpretation/interpreter.hpp"
#include "content/pmmstdlib/pmmstdlib.hpp"

#include <iostream>

int main(int argc, char** argv) {

    PrepareStandardLibrary();

    auto arguments = ParseCommandArguments(argc, argv);
    // command line with arguments
    if (arguments.size() > 1 and !arguments[0].empty() > 0 and arguments[0][0] == "p--" and !arguments[1].empty() > 0){
        RunInterpreter((new InterpreterFromFile(arguments[1])));
    }
    // launch interpreter interface
    else {
        RunInterpreter(new InterpreterFromUser());
    }

    return 0;
}