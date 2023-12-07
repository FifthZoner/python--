#ifndef PYTHON___INTERPRETER_HPP
#define PYTHON___INTERPRETER_HPP

#include <memory>

#include "interpreterInterface.hpp"

enum RunLineOutput {
    error, success, returned
};

inline std::unique_ptr <InterpreterInterface> interpreterStream;
inline unsigned long long functionLine = 0;
inline bool isDefiningFunction = false;

uint8_t RunLine(std::string line, unsigned long long lineNumber);

void RunInterpreter(InterpreterInterface* interface);

#endif //PYTHON___INTERPRETER_HPP
