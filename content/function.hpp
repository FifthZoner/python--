#ifndef PYTHON___FUNCTION_HPP
#define PYTHON___FUNCTION_HPP

#include <vector>
#include <string>

#include "variables.hpp"

struct FunctionVariable {
    uint8_t type;
    bool isImplicit;
    std::string name;
    FunctionVariable(uint8_t type, const std::string& name, bool isImplicit);
};

struct Function {
    std::vector <FunctionVariable> variables;
    uint8_t returnTypeValue;
    enum {
        none, binding, defined
    };

    [[nodiscard]] virtual uint8_t type() const;
    [[nodiscard]] virtual uint8_t returnType() const;
    virtual std::string run();
};

struct FunctionBinding : Function {
    void (*functionWrapper)(std::vector <Variable*>& arguments);

    FunctionBinding(void (*functionWrapper)(std::vector <Variable*>&), uint8_t returnType, std::vector <FunctionVariable> variables);
    [[nodiscard]] uint8_t type() const override;
    [[nodiscard]] uint8_t returnType() const override;
    std::string run(std::vector <Variable*>& arguments);
};


#endif //PYTHON___FUNCTION_HPP
