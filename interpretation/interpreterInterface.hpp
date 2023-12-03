#ifndef PYTHON___INTERPRETER_INTERFACE_HPP
#define PYTHON___INTERPRETER_INTERFACE_HPP

#include <string>
#include <vector>
#include <cstdint>

// base class, do not use outside the pointer
struct InterpreterInterface {
    // line will not be added to history unless pushed manually, it's done that way to ensure no broken lines are added
    std::vector <std::string> lines;
    virtual std::string getNextLine();

    virtual void endMessage();
};

// gets lines from user via command line interface
struct InterpreterFromUser : InterpreterInterface {
    InterpreterFromUser();
    std::string getNextLine() override;
    void endMessage() override;
};

// gets lines from given files in given order via file stream
struct InterpreterFromFile : InterpreterInterface {
    // NOT IMPLEMENTED
};


#endif //PYTHON___INTERPRETER_INTERFACE_HPP
