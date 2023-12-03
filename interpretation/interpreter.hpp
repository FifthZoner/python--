#ifndef PYTHON___INTERPRETER_HPP
#define PYTHON___INTERPRETER_HPP

#include <memory>

#include "interpreterInterface.hpp"

inline std::unique_ptr <InterpreterInterface> interpreterStream;

enum RunLineOutput {
    error, success, returned
};

uint8_t RunLine(std::string& line);

void RunInterpreter(InterpreterInterface* interface);

#endif //PYTHON___INTERPRETER_HPP
