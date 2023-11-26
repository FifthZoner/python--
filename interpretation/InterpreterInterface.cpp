#include <iostream>

#include "InterpreterInterface.hpp"

std::string InterpreterInterface::getNextLine() {
    return "";
}

InterpreterFromUser::InterpreterFromUser() {
    std::cout << "Welcome to Python-- interpreter interface!\n";
}
std::string InterpreterFromUser::getNextLine() {
    // TODO: add support for indentations, not needed - for now
    std::cout << "> ";
    std::string input;
    std::getline(std::cin, input);
    return input;
}