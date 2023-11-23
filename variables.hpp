#ifndef PYTHON___VARIABLES_HPP
#define PYTHON___VARIABLES_HPP

#include <cstdint>
#include <string>
#include <memory>

struct Variable {

    [[nodiscard]] virtual const uint8_t type() const;
    virtual std::unique_ptr<Variable> convert(const uint8_t type);

    enum {
        none, typeInt, typeString
    };

};

struct VariableString : Variable {
    std::string value;
    VariableString(std::string value);
    [[nodiscard]] const uint8_t type() const;
    std::unique_ptr<Variable> convert(const uint8_t type);
};

struct VariableInt : Variable {
    long long value;
    VariableInt(long long value);
    [[nodiscard]] const uint8_t type() const;
    std::unique_ptr<Variable> convert(const uint8_t type);
};


#endif //PYTHON___VARIABLES_HPP
