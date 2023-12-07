#ifndef PYTHON___FUNCTION_HPP
#define PYTHON___FUNCTION_HPP

#include <vector>
#include <string>
#include <unordered_map>

#include "variables.hpp"
#include "pmmstdlib/pmmstdlib.hpp"

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
        none, binding, custom
    };

    [[nodiscard]] virtual uint8_t type() const;
    [[nodiscard]] virtual uint8_t returnType() const;
    virtual std::string run(std::vector <std::string>& arguments);
};

struct FunctionBinding : Function {
    void (*functionWrapper)(std::vector <std::string>& arguments);

    FunctionBinding(void (*functionWrapper)(std::vector <std::string>&), uint8_t returnType, std::vector <FunctionVariable> variables);
    [[nodiscard]] uint8_t type() const override;
    [[nodiscard]] uint8_t returnType() const override;
    std::string run(std::vector <std::string>& arguments) override;
};

struct FunctionCustom : Function {
    std::pair <uint64_t, uint64_t> range;

    FunctionCustom(std::pair <uint64_t, uint64_t> range, uint8_t returnType, std::vector <FunctionVariable> variables);
    [[nodiscard]] uint8_t type() const override;
    [[nodiscard]] uint8_t returnType() const override;
    std::string run(std::vector <std::string>& arguments) override;
};

inline std::unordered_map <std::string, std::unique_ptr <Function>> functions;

void ParseNewFunction(std::pair<unsigned int, unsigned int> range);

#endif //PYTHON___FUNCTION_HPP
