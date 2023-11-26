#ifndef PYTHON___INTERPRETERINTERFACE_HPP
#define PYTHON___INTERPRETERINTERFACE_HPP

#include <string>

// base class, do not use outside the pointer
class InterpreterInterface {
public:
    virtual std::string getNextLine();
};

// gets lines from user via command line interface
class InterpreterFromUser : public InterpreterInterface {
public:
    InterpreterFromUser();
    std::string getNextLine() override;
};

// gets lines from given files in given order via file stream
class InterpreterFromFile : public InterpreterInterface {
    // NOT IMPLEMENTED
};


#endif //PYTHON___INTERPRETERINTERFACE_HPP
