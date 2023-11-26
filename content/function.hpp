#ifndef PYTHON___FUNCTION_HPP
#define PYTHON___FUNCTION_HPP

#include <vector>
#include <string>

#include "variables.hpp"

struct FunctionVariable {
    uint8_t type;
    bool isImplicit;
    std::string name;
    FunctionVariable(const uint8_t type, const std::string& name);
};

struct Function {
    std::vector <FunctionVariable> variables;
    std::string name;
    enum {
        none, binding, defined
    };

    virtual const uint8_t type();
    virtual const uint8_t returnType();
    virtual std::string run();
};

struct FunctionBinding : Function {
    void (*functionWrapper)(std::vector <Variable*> arguments);

    const uint8_t type();
    const uint8_t returnType();
    std::string run();
};


#endif //PYTHON___FUNCTION_HPP
