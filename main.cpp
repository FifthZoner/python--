// This program is a python-ish language interpreter
#include "parsing.hpp"
#include "interpreter.hpp"

int main(int argc, char** argv) {

    auto arguments = ParseCommandArguments(argc, argv);

    // command line with arguments
    if (arguments.size() > 1 and !arguments[0].empty() > 0 and arguments[0][0] == "p--" and !arguments[1].empty() > 0){

    }
    // launch interpreter interface
    else {

    }

    RunInterpreter();
    return 0;
}